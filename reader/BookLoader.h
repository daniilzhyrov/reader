#pragma once

#include <winrt/Windows.UI.Xaml.Controls.h>

namespace winrt::reader::implementation
{
	class BookLoader
	{
	public:
		static void LoadImage(Windows::UI::Xaml::Controls::Page const& page);
	};
}


