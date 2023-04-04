#pragma once

#include "MainPage.g.h"
#include "pch.h"
#include "ReadingInterface.h"
#include <winrt/Windows.UI.ViewManagement.h>
#include <winrt/Windows.Foundation.h>

using namespace winrt;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::ViewManagement;
using namespace Windows::Foundation;

namespace winrt::reader::implementation
{
    struct MainPage : MainPageT<MainPage>
    {
        MainPage()
        {
            // Xaml objects should not call InitializeComponent during construction.
            // See https://github.com/microsoft/cppwinrt/tree/master/nuget#initializecomponent
            ApplicationView::GetForCurrentView().SetPreferredMinSize(winrt::Windows::Foundation::Size(2400, 1200));
        }

        int32_t MyProperty();
        void MyProperty(int32_t value);

        void ClickHandler(Windows::Foundation::IInspectable const& sender, Windows::UI::Xaml::RoutedEventArgs const& args);

        // Add the event handler prototype here
        void Click_OpenaNewBook(Windows::Foundation::IInspectable const& sender, Windows::UI::Xaml::RoutedEventArgs const& e);
        void Click_CreateaNewBook(Windows::Foundation::IInspectable const& sender, Windows::UI::Xaml::RoutedEventArgs const& e);
    };
}

namespace winrt::reader::factory_implementation
{
    struct MainPage : MainPageT<MainPage, implementation::MainPage>
    {
    };
}
