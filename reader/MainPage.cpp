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
#include <winrt/Windows.UI.ViewManagement.h>
#include <winrt/base.h>
#include <filesystem>

#include <winrt/Windows.Foundation.h>
//#include "ListItem.h"

using namespace winrt;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace winrt::Windows::UI::Xaml::Navigation;
using namespace winrt::Windows::Storage;
using namespace winrt::Windows::Storage::Pickers;
using namespace winrt::Windows::Foundation::Collections;
using namespace Windows::Storage::Streams;
using namespace Windows::UI::ViewManagement;
using namespace Windows::Foundation;

float PREFERED_WINDOW_HEIGHT = 350;
float PREFERED_WINDOW_WIDTH = 600;

namespace winrt::reader::implementation
{
    MainPage::MainPage()
    {
        Loaded({ this, &MainPage::OnLoaded });
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
            auto randomAccessStream = co_await file.OpenAsync(FileAccessMode::Read);
            SaveRecentlyOpenedFile(file.Path());

            ContentFrame().Navigate(xaml_typename<reader::ReadingInterface>(), box_value(randomAccessStream));
        }
    }

    void MainPage::OnSelectionChanged(IInspectable const& sender, SelectionChangedEventArgs const& e) {
    }

    void MainPage::OnLoaded(IInspectable const& sender, RoutedEventArgs const& args)
    {
        auto appView = winrt::Windows::UI::ViewManagement::ApplicationView::GetForCurrentView();
        appView.SetPreferredMinSize(winrt::Windows::Foundation::Size(PREFERED_WINDOW_WIDTH, PREFERED_WINDOW_HEIGHT));
        winrt::Windows::UI::ViewManagement::ApplicationView::PreferredLaunchWindowingMode(ApplicationViewWindowingMode::PreferredLaunchViewSize);
        Rect currentBounds = Window::Current().Bounds();
        Size initialSize(PREFERED_WINDOW_WIDTH, PREFERED_WINDOW_HEIGHT);

        if (currentBounds.Width != initialSize.Width || currentBounds.Height != initialSize.Height)
        {
            auto result = appView.TryResizeView(initialSize);
            currentBounds = Window::Current().Bounds();
        }

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

    winrt::hstring GetFileNameFromPath(const winrt::hstring& path)
    {
        // Convert the hstring to std::wstring
        std::wstring wstrPath(path);

        // Use the std::filesystem::path class to get the filename
        std::filesystem::path fsPath(wstrPath);
        std::wstring fileName = fsPath.filename().wstring();

        // Convert the std::wstring back to an hstring
        return winrt::hstring(fileName);
    }

    void MainPage::LoadRecentlyOpenedFiles()
    {
        auto localSettings = ApplicationData::Current().LocalSettings();
        auto recentFilesValue = localSettings.Values().Lookup(L"RecentFiles");

        RecentlyOpenedListBox().Items().Clear();
        recentItems = single_threaded_vector<hstring>();
        if (recentFilesValue)
        {
            auto recentFilesComposite = unbox_value<ApplicationDataCompositeValue>(recentFilesValue);
            unsigned int index = 0;
            while (recentFilesComposite.HasKey(std::to_wstring(index)))
            {
                hstring filePath = unbox_value<hstring>(recentFilesComposite.Lookup(std::to_wstring(index)));
                recentItems.Append(filePath);
                PropertySet item;
                item.Insert(L"Title", box_value(GetFileNameFromPath(filePath)));
                item.Insert(L"Subtitle", box_value(filePath));
                RecentlyOpenedListBox().Items().Append(item);
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
        recentFilesComposite = ApplicationDataCompositeValue();
        localSettings.Values().Insert(L"RecentFiles", box_value(recentFilesComposite));

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
