#pragma once

#include "CreateNewBook.g.h"
#include <Windows.h>
#include <fstream>
#include <iostream>
#include <vector>
#include <filesystem>
#include <AclAPI.h>
#include <winrt/Windows.Storage.h>
#include <winrt/windows.storage.streams.h>
#include <winrt/Windows.Storage.Streams.h>
#include <winrt/Windows.Storage.Pickers.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.Storage.Compression.h>
#include "winrt/Windows.UI.Core.h"
#include <winrt/Windows.Foundation.h>
#include <zip.h>



namespace winrt::reader::implementation
{
    struct CreateNewBook : CreateNewBookT<CreateNewBook>
    {
        CreateNewBook();

        int32_t MyProperty();
        void MyProperty(int32_t value);

        //void Click_SelectImages(Windows::Foundation::IInspectable const&, Windows::UI::Xaml::RoutedEventArgs const&);
        winrt::fire_and_forget Click_SelectImages(Windows::Foundation::IInspectable const&, Windows::UI::Xaml::RoutedEventArgs const&);
        //void Click_Save(Windows::Foundation::IInspectable const&, Windows::UI::Xaml::RoutedEventArgs const&);
    };
}

namespace winrt::reader::factory_implementation
{
    struct CreateNewBook : CreateNewBookT<CreateNewBook, implementation::CreateNewBook>
    {
    };
}
