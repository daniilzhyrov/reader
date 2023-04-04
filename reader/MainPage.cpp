#include "pch.h"
#include "MainPage.h"
#include "MainPage.g.cpp"

using namespace winrt;
using namespace Windows::UI::Xaml;

namespace winrt::reader::implementation
{
    int32_t MainPage::MyProperty()
    {
        throw hresult_not_implemented();
    }

    void MainPage::MyProperty(int32_t /* value */)
    {
        throw hresult_not_implemented();
    }

    void MainPage::ClickHandler(IInspectable const&, RoutedEventArgs const&)
    {
        //myButton().Content(box_value(L"Clicked"));
    }

    void MainPage::Click_OpenaNewBook(IInspectable const&, RoutedEventArgs const&)
    {
        Frame().Navigate(xaml_typename<reader::ReadingInterface>());
    }

    void MainPage::Click_CreateaNewBook(IInspectable const&, RoutedEventArgs const&)
    {
        Frame().Navigate(xaml_typename<reader::CreateNewBook>());
    }
}
