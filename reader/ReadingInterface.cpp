#include "pch.h"
#include "ReadingInterface.h"
#include "ReadingInterface.g.cpp"
#include <winrt/Windows.UI.ViewManagement.h>
#include <winrt/Windows.Storage.Streams.h>
#include <winrt/Windows.UI.Xaml.Media.Imaging.h>
#include <winrt/Windows.Foundation.h>
#include "winrt/Windows.UI.Xaml.Navigation.h"
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.ApplicationModel.Core.h>
#include <winrt/Windows.UI.Core.h>
#include <winrt/base.h>
#include <iostream>
#include <cerrno>
#include <future>
#include <zip.h>


using namespace winrt;
using namespace Windows::UI::Xaml;
using namespace Windows::Storage::Streams;
using namespace Windows::UI::ViewManagement;
using namespace Windows::UI::Xaml::Media::Imaging;
using namespace Windows::UI::Xaml::Media;
using namespace winrt::Windows::UI::Xaml::Navigation;
using namespace winrt::Windows::Foundation::Collections;
using namespace winrt::Windows::UI::Xaml::Controls;
using namespace winrt::Windows::UI::Core;
using namespace winrt::Windows::ApplicationModel::Core;


using namespace Windows::Foundation;

namespace winrt::reader::implementation
{    
    IAsyncOperation<IVector<BitmapImage>> ReadImagesFromZipArchive(IRandomAccessStream const& randomAccessStream)
    {
        // Create a temporary file to store the archive data
        auto tempFolder = Windows::Storage::ApplicationData::Current().TemporaryFolder();
        auto tempFile = co_await tempFolder.CreateFileAsync(L"temp.zip", Windows::Storage::CreationCollisionOption::ReplaceExisting);

        // Write the archive data to the temporary file
        auto fileStream = co_await tempFile.OpenAsync(Windows::Storage::FileAccessMode::ReadWrite);
        
        if (!randomAccessStream) {
            std::cerr << "Error: randomAccessStream is not valid" << std::endl;
            co_return {};
        }

        auto streamSize = randomAccessStream.Size();

        // Create a buffer with the capacity set to the stream size
        Windows::Storage::Streams::Buffer buffer(static_cast<uint32_t>(streamSize));

        // Read the data from the randomAccessStream into the buffer
        co_await randomAccessStream.ReadAsync(buffer, static_cast<uint32_t>(streamSize), InputStreamOptions::None);
        co_await fileStream.WriteAsync(buffer);
        co_await fileStream.FlushAsync();

        // Open the temporary zip archive
        auto zipPath = tempFile.Path();
        std::wstring wstrZipPath(zipPath);
        std::string strZipPath(wstrZipPath.begin(), wstrZipPath.end());

        int error;
        zip_t* archive = zip_open(strZipPath.c_str(), ZIP_RDONLY, &error);
        if (archive == nullptr)
        {
            char error_message[256];
            strerror_s(error_message, sizeof(error_message), errno);
            std::cerr << "Error opening zip archive: " << error << std::endl;
            co_return {};
        }

        // Read the images from the archive
        IVector<BitmapImage> images = single_threaded_vector<BitmapImage>();
        zip_int64_t numEntries = zip_get_num_entries(archive, 0);

        for (zip_int64_t i = 0; i < numEntries; ++i)
        {
            // Get the information about the current file in the archive
            zip_stat_t file_stat;
            if (zip_stat_index(archive, i, 0, &file_stat) != 0)
            {
                std::cerr << "Error getting file information in zip archive" << std::endl;
                continue;
            }

            // Open the current file in the archive
            zip_file_t* file = zip_fopen_index(archive, i, 0);
            if (file == nullptr)
            {
                std::cerr << "Error opening file in zip archive" << std::endl;
                continue;
            }

            // Read the file data into a buffer
            zip_uint64_t fileSize = file_stat.size;
            std::vector<uint8_t> buffer(fileSize);
            zip_fread(file, buffer.data(), fileSize);

            // Close the file
            zip_fclose(file);

            InMemoryRandomAccessStream stream;
            DataWriter writer(stream);
            writer.WriteBytes(buffer);
            co_await writer.StoreAsync(); // Write the buffer to the stream

            BitmapImage image;
            stream.Seek(0);
            co_await image.SetSourceAsync(stream);

            // Add the image to the list of images
            images.Append(image);
        }

        // Close the zip archive
        zip_close(archive);

        // Delete the temporary file
        tempFile.DeleteAsync();

        co_return images;
    }

    void ReadingInterface::SetImageWithNumber(unsigned int number) {
        ComicImageOne().Source(images.GetAt(number));
        if (TwoPagesModeEnabled() && number + 1 < m_numberOfPages) {
            ComicImageTwo().Source(images.GetAt(number + 1));
            ComicImageTwo().Visibility(Visibility::Visible);
        } else {
            ComicImageTwo().Visibility(Visibility::Collapsed);
        }
    }

    void ReadingInterface::OnNextButtonClicked(IInspectable const& sender, RoutedEventArgs const&) {
        unsigned int newPageNumber = PageNumber() + (TwoPagesModeEnabled() ? 2 : 1);
        SetPageNumber(newPageNumber);
        SetImageWithNumber(PageNumber());
    }

    void ReadingInterface::OnPreviousButtonClicked(IInspectable const& sender, RoutedEventArgs const&) {
        int newPageNumber = int(PageNumber()) - (TwoPagesModeEnabled() ? 2 : 1);
        SetPageNumber(newPageNumber);
        SetImageWithNumber(PageNumber());
    }

    void ReadingInterface::FirstPageButtonClicked(IInspectable const& sender, RoutedEventArgs const&) {
        int newPageNumber = 0;
        SetPageNumber(newPageNumber);
        SetImageWithNumber(PageNumber());
    }

    void ReadingInterface::LastPageButtonClicked(IInspectable const& sender, RoutedEventArgs const&) {
        int newPageNumber = m_numberOfPages - 1;
        SetPageNumber(newPageNumber);
        SetImageWithNumber(PageNumber());
    }

    void ReadingInterface::FitImageToPageSize() {
        double windowHeight = ImageScrollViewer().ActualHeight();
        double windowWidth = ImageScrollViewer().ActualWidth();

        if (TwoPagesModeEnabled()) {
            ComicImageOne().Width(windowWidth / 2);
            ComicImageTwo().Width(windowWidth / 2);
            ComicImageOne().Height(windowHeight);
            ComicImageTwo().Height(windowHeight);
        }
        else {
            ComicImageOne().Height(windowHeight);
            ComicImageOne().Width(windowWidth);
        }
    }

    void ReadingInterface::FitPageButtonClicked(IInspectable const&s, RoutedEventArgs const&) {
        FitImageToPageSize();
    }

    void ReadingInterface::ZoomInButtonClicked(IInspectable const&, RoutedEventArgs const&) {
        double windowHeight = ImageScrollViewer().ActualHeight();
        double windowWidth = ImageScrollViewer().ActualWidth();

        double newImageHeight = ComicImageOne().ActualHeight() * 1.2;
        double newImageOneWidth = ComicImageOne().ActualWidth() * 1.2;
        double newImageTwoWidth = ComicImageTwo().ActualWidth() * 1.2;
        if (newImageHeight > windowHeight * 4 || newImageOneWidth > windowWidth * (TwoPagesModeEnabled() ? 2 : 4) ) {
            newImageHeight = windowHeight * 4;
            newImageOneWidth = windowWidth * (TwoPagesModeEnabled() ? 2 : 4);
            newImageTwoWidth = windowWidth * (TwoPagesModeEnabled() ? 2 : 4);
        }
        ComicImageOne().Height(newImageHeight);
        ComicImageOne().Width(newImageOneWidth);
        ComicImageTwo().Height(newImageHeight);
        ComicImageTwo().Width(newImageTwoWidth);
    }

    void ReadingInterface::ZoomOutButtonClicked(IInspectable const&, RoutedEventArgs const&) {
        double windowHeight = ImageScrollViewer().ActualHeight();
        double windowWidth = ImageScrollViewer().ActualWidth();

        double newImageHeight = ComicImageOne().ActualHeight() / 1.2;
        double newImageOneWidth = ComicImageOne().ActualWidth() / 1.2;
        double newImageTwoWidth = ComicImageTwo().ActualWidth() / 1.2;
        if (newImageHeight < windowHeight / 2 || newImageOneWidth < windowWidth / (TwoPagesModeEnabled() ? 4 : 2)) {
            newImageHeight = windowHeight / 2;
            newImageOneWidth = windowWidth / (TwoPagesModeEnabled() ? 4 : 2);
            newImageTwoWidth = windowWidth / (TwoPagesModeEnabled() ? 4 : 2);
        }
        ComicImageOne().Height(newImageHeight);
        ComicImageOne().Width(newImageOneWidth);
        ComicImageTwo().Height(newImageHeight);
        ComicImageTwo().Width(newImageTwoWidth);
    }

    void ReadingInterface::SingleDoublePageModeButtonClicked(IInspectable const&, RoutedEventArgs const&) {
        if (m_numberOfPages < 2)
            return;
        SetTwoPagesModeEnabled(!TwoPagesModeEnabled());
        ComicImageTwo().Visibility(TwoPagesModeEnabled() ? Visibility::Visible : Visibility::Collapsed);
        if (PageNumber() == m_numberOfPages - 1) {
            SetPageNumber(PageNumber() - 1);
        }
        SetImageWithNumber(PageNumber());
        FitImageToPageSize();
    }

    IAsyncAction InitializeNewView(CoreApplicationView newView, int32_t& newViewId)
    {
        auto dispatcher = newView.Dispatcher();

        co_await resume_foreground(dispatcher);

        auto frame = Frame();
        frame.Navigate(xaml_typename<reader::MainPage>());

        Window::Current().Content(frame);
        Window::Current().Activate();

        newViewId = ApplicationView::GetForCurrentView().Id();
    }

    IAsyncAction OpenNewWindowAndNavigateToPage()
    {
        // Create a new CoreApplicationView
        auto newView = CoreApplication::CreateNewView();
        int32_t newViewId = 0;

        // Initialize the new view
        co_await InitializeNewView(newView, newViewId);

        // Show the new window
        co_await ApplicationViewSwitcher::TryShowAsStandaloneAsync(newViewId);

        // Close the current window if needed
        // Note: Closing the current window may exit the app if there's only one window left
        // Ensure you handle app lifecycle events correctly to avoid losing user data
        auto currentViewId = ApplicationView::GetForCurrentView().Id();
        co_await ApplicationViewSwitcher::SwitchAsync(newViewId, currentViewId, ApplicationViewSwitchingOptions::ConsolidateViews);
    }


    fire_and_forget ReadingInterface::BackButtonClicked(IInspectable const&, RoutedEventArgs const&) {
        co_await OpenNewWindowAndNavigateToPage();
    }
    
    ReadingInterface::ReadingInterface()
    {
        auto appView = winrt::Windows::UI::ViewManagement::ApplicationView::GetForCurrentView();
        appView.TryEnterFullScreenMode();
        m_numberOfPages = 0;
        m_pageNumber = 0;
        m_isTwoPageMode = false;
    }

    fire_and_forget ReadingInterface::OnNavigatedTo(NavigationEventArgs const& e)
    {
        if (e.Parameter())
        {
            auto randomAccessStream = unbox_value<IRandomAccessStream>(e.Parameter());
            images = co_await ReadImagesFromZipArchive(randomAccessStream);
            SetPageNumber(0);
            ComicImageOne().Source(images.GetAt(0));
            FitImageToPageSize();
            m_numberOfPages = images.Size();
        }
        else
        {
            std::cerr << "ERROR" << std::endl;
        }
    }


    unsigned int ReadingInterface::PageNumber()
    {
        return m_pageNumber;
    }

    void ReadingInterface::SetPageNumber(int32_t value)
    {
        if (value <= 0)
            m_pageNumber = 0;
        if (value > 0 && value < m_numberOfPages)
            m_pageNumber = unsigned int(value);
    }

    bool ReadingInterface::TwoPagesModeEnabled()
    {
        return m_isTwoPageMode;
    }

    void ReadingInterface::SetTwoPagesModeEnabled(bool value)
    {
        m_isTwoPageMode = value;
    }

}