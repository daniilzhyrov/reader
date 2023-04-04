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

    ReadingInterface::ReadingInterface()
    {
        ApplicationView::GetForCurrentView().SetPreferredMinSize(winrt::Windows::Foundation::Size(1800, 1200));
    }

    int32_t ReadingInterface::MyProperty()
    {
        throw hresult_not_implemented();
    }

    void ReadingInterface::MyProperty(int32_t /* value */)
    {
        throw hresult_not_implemented();
    }

    void ReadingInterface::ClickHandler(IInspectable const&, RoutedEventArgs const&)
    {
        //Button().Content(box_value(L"Clicked"));
    }
}