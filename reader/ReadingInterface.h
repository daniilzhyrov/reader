#pragma once

#include "ReadingInterface.g.h"

namespace winrt::reader::implementation
{
    struct ReadingInterface : ReadingInterfaceT<ReadingInterface>
    {
        ReadingInterface();
        void OnNavigatedTo(winrt::Windows::UI::Xaml::Navigation::NavigationEventArgs const& e);

    };
}

namespace winrt::reader::factory_implementation
{
    struct ReadingInterface : ReadingInterfaceT<ReadingInterface, implementation::ReadingInterface>
    {
    };
}