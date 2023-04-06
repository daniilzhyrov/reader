#pragma once

#include "MainPage.g.h"

namespace winrt::reader::implementation
{
    struct MainPage : MainPageT<MainPage>
    {
        MainPage();

        void OnLoaded(winrt::Windows::Foundation::IInspectable const&, winrt::Windows::UI::Xaml::RoutedEventArgs const&);
        void UpdateEmptyMessageVisibility();
        void LoadRecentlyOpenedFiles();
        void SaveRecentlyOpenedFile(hstring const& filePath);
        
        fire_and_forget OpenComicButton_Click(winrt::Windows::Foundation::IInspectable const&, winrt::Windows::UI::Xaml::RoutedEventArgs const&);
        void OnSelectionChanged(winrt::Windows::Foundation::IInspectable const&, winrt::Windows::UI::Xaml::Controls::SelectionChangedEventArgs const&);
    private:
        winrt::Windows::Foundation::Collections::IVector<hstring> recentItems;
    };
}

namespace winrt::reader::factory_implementation
{
    struct MainPage : MainPageT<MainPage, implementation::MainPage>
    {
    };
}
