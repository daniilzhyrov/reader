#pragma once

#include "ReadingInterface.g.h"

namespace winrt::reader::implementation
{
    struct ReadingInterface : ReadingInterfaceT<ReadingInterface>
    {
        ReadingInterface();

        int32_t MyProperty();
        void MyProperty(int32_t value);

        void ClickHandler(Windows::Foundation::IInspectable const& sender, Windows::UI::Xaml::RoutedEventArgs const& args);
    };
}

namespace winrt::reader::factory_implementation
{
    struct ReadingInterface : ReadingInterfaceT<ReadingInterface, implementation::ReadingInterface>
    {
    };
}