#include "pch.h"
#include "MainPage.h"
#include "MainPage.g.cpp"

#include "winrt/Windows.UI.Xaml.h"
#include "winrt/Windows.UI.ViewManagement.h"
#include "winrt/Windows.UI.Xaml.Navigation.h"
#include "winrt/Windows.Storage.h"
#include "winrt/Windows.Storage.Pickers.h"
#include "winrt/Windows.Storage.Streams.h"
#include "winrt/Windows.Foundation.Collections.h"
#include "winrt/Windows.UI.Core.h"
#include "winrt/Windows.System.h"


using namespace winrt;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace winrt::Windows::UI::Xaml::Navigation;
using namespace winrt::Windows::Storage;
using namespace winrt::Windows::Storage::Pickers;
using namespace winrt::Windows::Foundation::Collections;


namespace winrt::reader::implementation
{
    MainPage::MainPage()
    {
    }

    fire_and_forget MainPage::OpenComicAsync()
    {
        auto lifetime_token = get_weak();

        FileOpenPicker openPicker;
        openPicker.ViewMode(PickerViewMode::Thumbnail);
        openPicker.SuggestedStartLocation(PickerLocationId::DocumentsLibrary);
        openPicker.FileTypeFilter().Append(L".cbz");
        openPicker.FileTypeFilter().Append(L".cbr");

        StorageFile file = co_await openPicker.PickSingleFileAsync();
        if (file)
        {
            hstring filePath = file.Path();
            SaveRecentlyOpenedFile(filePath);
            //ContentFrame().Navigate(xaml_typename<reader::ReaderPage>(), box_value(filePath));
        }
    }

    void MainPage::OpenComicButton_Click(IInspectable const&, RoutedEventArgs const&)
    {
        //auto lifetime_token = get_strong();

        //FileOpenPicker openPicker;
        //openPicker.ViewMode(PickerViewMode::Thumbnail);
        //openPicker.SuggestedStartLocation(PickerLocationId::DocumentsLibrary);
        //openPicker.FileTypeFilter().Append(L".cbz");
        //openPicker.FileTypeFilter().Append(L".cbr");

        //StorageFile file = co_await openPicker.PickSingleFileAsync();
        //if (file)
        //{
        //    hstring filePath = file.Path();
        //    SaveRecentlyOpenedFile(filePath);
        //    LoadRecentlyOpenedFiles();
        //    //ContentFrame().Navigate(xaml_typename<reader::ReaderPage>(), box_value(filePath));
        //}
        OpenComicAsync();
    }

    void MainPage::CreateComicButton_Click(IInspectable const&, RoutedEventArgs const&)
    {
        // Navigate to Create Comic page
        //ContentFrame().Navigate(xaml_typename<reader::CreateComicPage>());
    }

    void MainPage::OnNavigatedTo(NavigationEventArgs const& e)
    {
        LoadRecentlyOpenedFiles();
    }

    void MainPage::UpdateEmptyMessageVisibility()
    {
        if (RecentlyOpenedListBox().Items().Size() == 0)
        {
            EmptyMessage().Visibility(Visibility::Visible);
        }
        else
        {
            EmptyMessage().Visibility(Visibility::Collapsed);
        }
    }

    void MainPage::LoadRecentlyOpenedFiles()
    {
        auto localSettings = ApplicationData::Current().LocalSettings();
        auto recentFilesValue = localSettings.Values().Lookup(L"RecentFiles");

        if (recentFilesValue)
        {
            IVector<IInspectable> recentFiles = unbox_value<IVector<IInspectable>>(recentFilesValue);
            RecentlyOpenedListBox().Items().Clear();

            for (unsigned int i = 0; i < recentFiles.Size(); ++i)
            {
                hstring filePath = unbox_value<hstring>(recentFiles.GetAt(i));
                RecentlyOpenedListBox().Items().Append(box_value(filePath));
            }
        }

        UpdateEmptyMessageVisibility();
    }

    fire_and_forget MainPage::SaveRecentlyOpenedFile(hstring const& filePath)
    {
        auto lifetime_weak = get_weak();

        auto localSettings = ApplicationData::Current().LocalSettings();
        auto recentFilesValue = localSettings.Values().Lookup(L"RecentFiles");

        IVector<hstring> recentFiles;

        if (recentFilesValue)
        {
            recentFiles = unbox_value<IVector<hstring>>(recentFilesValue);
        }
        else
        {
            recentFiles = single_threaded_vector<hstring>();
            localSettings.Values().Insert(L"RecentFiles", box_value(recentFiles));
        }

        /*for (unsigned int i = 0; i < recentFiles.Size(); ++i)
        {
            hstring currentFilePath = unbox_value<hstring>(recentFiles.GetAt(i));
            if (currentFilePath == filePath)
            {
                co_await resume_foreground(Dispatcher());
                if (auto lifetime_strong = lifetime_weak.get())
                {
                    RecentlyOpenedListBox().Items().RemoveAt(i);
                }
                break;
            }
        }*/

        co_await resume_foreground(Dispatcher());
        /*if (auto lifetime_strong = lifetime_weak.get())
        {
            RecentlyOpenedListBox().Items().Append(box_value(filePath));
        }

        const unsigned int maxRecentFiles = 10;
        while (recentFiles.Size() > maxRecentFiles)
        {
            recentFiles.RemoveAt(0);
        }*/
        
    }

}
