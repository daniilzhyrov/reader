#include "pch.h"
#include "BookLoader.h"
#include <winrt/Windows.ApplicationModel.h>
#include <winrt/Windows.Storage.Streams.h>
#include <winrt/Windows.Graphics.Imaging.h>




using namespace winrt;
using namespace Windows::Storage;
using namespace Windows::Storage::Streams;
using namespace Windows::Graphics::Imaging;
using namespace winrt::Windows::UI::Xaml::Media::Imaging;
using namespace winrt::Windows::UI::Xaml::Controls;

void reader::implementation::BookLoader::LoadImage(Windows::UI::Xaml::Controls::Page const& page)
{
    //// Load the CBR file from the assets folder
    //StorageFile cbrFile = ApplicationData::Current().LocalFolder().GetFileAsync(L"comic.cbr").get();
    //IRandomAccessStream cbrStream = cbrFile.OpenAsync(FileAccessMode::Read).get();
    //DataReader cbrReader = DataReader::FromStream(cbrStream);
    //IBuffer cbrBuffer = cbrReader.ReadBufferAsync(cbrFile.Size()).get();

    //// Create a decoder to decode the first image in the CBR file
    //BitmapDecoder decoder = BitmapDecoder::CreateAsync(cbrStream).get();

    //// Get the first frame of the image and create a BitmapImage from it
    //BitmapFrame frame = decoder.GetFrameAsync(0).get();
    //BitmapImage bitmap;
    //InMemoryRandomAccessStream memoryStream;
    //BitmapEncoder::CreateAsync(BitmapEncoder::PngEncoderId(), memoryStream).get();
    //memoryStream.Seek(0);
    //bitmap.SetSourceAsync(memoryStream).get();

    //// Set the image source to the ComicImage control
    //auto comicImage = page.FindName(L"ComicImage").as<Image>();
    //comicImage.Source(bitmap);
}