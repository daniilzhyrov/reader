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

    fire_and_forget MainPage::OpenComicButton_Click(IInspectable const&, RoutedEventArgs const&)
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
            ContentFrame().Navigate(xaml_typename<reader::ReadingInterface>(), box_value(filePath));
        }
    }

    void MainPage::CreateComicButton_Click(IInspectable const&, RoutedEventArgs const&)
    {
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

        RecentlyOpenedListBox().Items().Clear();

        if (recentFilesValue)
        {
            auto recentFilesComposite = unbox_value<ApplicationDataCompositeValue>(recentFilesValue);
            unsigned int index = 0;
            while (recentFilesComposite.HasKey(std::to_wstring(index)))
            {
                hstring filePath = unbox_value<hstring>(recentFilesComposite.Lookup(std::to_wstring(index)));
                RecentlyOpenedListBox().Items().Append(box_value(filePath));
                ++index;
            }
        }

        UpdateEmptyMessageVisibility();
    }

    void MainPage::SaveRecentlyOpenedFile(hstring const& filePath)
    {
        auto lifetime_weak = get_weak();

        auto localSettings = ApplicationData::Current().LocalSettings();
        auto recentFilesValue = localSettings.Values().Lookup(L"RecentFiles");

        ApplicationDataCompositeValue recentFilesComposite;

        if (recentFilesValue)
        {
            recentFilesComposite = unbox_value<ApplicationDataCompositeValue>(recentFilesValue);
        }
        else
        {
            recentFilesComposite = ApplicationDataCompositeValue();
            localSettings.Values().Insert(L"RecentFiles", box_value(recentFilesComposite));
        }

        bool found = false;
        int index = -1;
        while (recentFilesComposite.HasKey(std::to_wstring(++index)))
        {
            hstring currentFilePath = unbox_value<hstring>(recentFilesComposite.Lookup(std::to_wstring(index)));
            if (currentFilePath == filePath)
            {
                found = true;
                break;
            }
        }

        if (!found)
        {
            recentFilesComposite.Insert(std::to_wstring(index), box_value(filePath));
        }

        const unsigned int maxRecentFiles = 5;
        while (index > maxRecentFiles - 1)
        {
            recentFilesComposite.Remove(std::to_wstring(index - maxRecentFiles));
            --index;
        }

        localSettings.Values().Insert(L"RecentFiles", box_value(recentFilesComposite));

        LoadRecentlyOpenedFiles();
    }

}
