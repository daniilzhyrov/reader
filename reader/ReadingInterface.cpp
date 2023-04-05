#include "pch.h"
#include "ReadingInterface.h"
#include "ReadingInterface.g.cpp"
#include <winrt/Windows.UI.ViewManagement.h>
#include <winrt/Windows.Storage.Streams.h>
#include <winrt/Windows.UI.Xaml.Media.Imaging.h>
#include <winrt/Windows.Foundation.h>
#include "winrt/Windows.UI.Xaml.Navigation.h"
#include <iostream>
#include <zip.h>

using namespace winrt;
using namespace Windows::UI::Xaml;
using namespace Windows::Storage::Streams;
using namespace Windows::UI::ViewManagement;
using namespace Windows::UI::Xaml::Media::Imaging;
using namespace winrt::Windows::UI::Xaml::Navigation;
using namespace Windows::Foundation;

namespace winrt::reader::implementation
{
    std::vector<BitmapImage> ReadImagesFromZipArchive(hstring const& zipPath)
    {
        // Convert hstring to std::wstring and then to std::string
        std::wstring wstrZipPath(zipPath);
        std::string strZipPath(wstrZipPath.begin(), wstrZipPath.end());

        // Open the zip archive
        int error;
        zip_t* archive = zip_open(strZipPath.c_str(), ZIP_RDONLY, &error);
        if (archive == nullptr)
        {
            std::cerr << "Error opening zip archive: " << error << std::endl;
            return {};
        }

        // Read the images from the archive
        std::vector<BitmapImage> images;
        zip_int64_t numEntries = zip_get_num_entries(archive, 0);

        for (zip_int64_t i = 0; i < numEntries; ++i)
        {
            // Open the current file in the archive
            zip_file_t* file = zip_fopen_index(archive, i, 0);
            if (file == nullptr)
            {
                std::cerr << "Error opening file in zip archive" << std::endl;
                continue;
            }

            // Read the file data into a buffer
            zip_uint64_t fileSize = zip_fread(file, nullptr, 0);
            std::vector<uint8_t> buffer(fileSize);
            zip_fread(file, buffer.data(), fileSize);

            // Close the file
            zip_fclose(file);

            // Create a BitmapImage from the buffer
            InMemoryRandomAccessStream stream;
            DataWriter writer(stream);
            writer.WriteBytes(buffer);
            writer.StoreAsync().get();

            BitmapImage image;
            image.SetSourceAsync(stream).get();

            // Add the image to the list of images
            images.push_back(image);
        }

        // Close the zip archive
        zip_close(archive);

        return images;
    }

    ReadingInterface::ReadingInterface()
    {
        auto appView = winrt::Windows::UI::ViewManagement::ApplicationView::GetForCurrentView();
        appView.SetPreferredMinSize(winrt::Windows::Foundation::Size(1000, 600));
        appView.TryResizeView(Size(1000, 600));
    }

    void ReadingInterface::OnNavigatedTo(NavigationEventArgs const& e)
    {
        if (e.Parameter())
        {
            hstring filePath = unbox_value<hstring>(e.Parameter());
            auto images = ReadImagesFromZipArchive(filePath);

            for (const auto& image : images)
            {
                ComicImage().Source(image);
            }
        } else {
            std::cerr << "ERROR" << std::endl;
        }
    }
}