#pragma once

#include "MainPage.g.h"

namespace winrt::reader::implementation
{
    struct MainPage : MainPageT<MainPage>
    {
        MainPage();

        void OnNavigatedTo(winrt::Windows::UI::Xaml::Navigation::NavigationEventArgs const& e);
        void UpdateEmptyMessageVisibility();
        void LoadRecentlyOpenedFiles();
        void SaveRecentlyOpenedFile(hstring const& filePath);
        
        fire_and_forget OpenComicButton_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e);
        void CreateComicButton_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e);
    };
}

namespace winrt::reader::factory_implementation
{
    struct MainPage : MainPageT<MainPage, implementation::MainPage>
    {
    };
}
