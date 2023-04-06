#pragma once

#include "ReadingInterface.g.h"

namespace winrt::reader::implementation
{
    struct ReadingInterface : ReadingInterfaceT<ReadingInterface>
    {
        ReadingInterface();

        unsigned int PageNumber();
        void SetPageNumber(int32_t);

        bool TwoPagesModeEnabled();
        void SetTwoPagesModeEnabled(bool);

        void FitImageToPageSize();

        void OnNextButtonClicked(winrt::Windows::Foundation::IInspectable const&, winrt::Windows::UI::Xaml::RoutedEventArgs const&);
        void OnPreviousButtonClicked(winrt::Windows::Foundation::IInspectable const&, winrt::Windows::UI::Xaml::RoutedEventArgs const&);
        void FirstPageButtonClicked(winrt::Windows::Foundation::IInspectable const&, winrt::Windows::UI::Xaml::RoutedEventArgs const&);
        void LastPageButtonClicked(winrt::Windows::Foundation::IInspectable const&, winrt::Windows::UI::Xaml::RoutedEventArgs const&);
        void FitPageButtonClicked(winrt::Windows::Foundation::IInspectable const&, winrt::Windows::UI::Xaml::RoutedEventArgs const&);
        void ZoomInButtonClicked(winrt::Windows::Foundation::IInspectable const&, winrt::Windows::UI::Xaml::RoutedEventArgs const&);
        void ZoomOutButtonClicked(winrt::Windows::Foundation::IInspectable const&, winrt::Windows::UI::Xaml::RoutedEventArgs const&);
        void SingleDoublePageModeButtonClicked(winrt::Windows::Foundation::IInspectable const&, winrt::Windows::UI::Xaml::RoutedEventArgs const&);
        fire_and_forget BackButtonClicked(winrt::Windows::Foundation::IInspectable const&, winrt::Windows::UI::Xaml::RoutedEventArgs const&);
        
        fire_and_forget OnNavigatedTo(winrt::Windows::UI::Xaml::Navigation::NavigationEventArgs const& e);

    private:
        unsigned int m_pageNumber = 0;
        unsigned int m_numberOfPages = 0;
        bool m_isTwoPageMode = false;
        winrt::Windows::Foundation::Collections::IVector<Windows::UI::Xaml::Media::Imaging::BitmapImage> images;

        void SetImageWithNumber(unsigned int number);
    };
}

namespace winrt::reader::factory_implementation
{
    struct ReadingInterface : ReadingInterfaceT<ReadingInterface, implementation::ReadingInterface>
    {
    };
}