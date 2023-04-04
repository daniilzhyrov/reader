#include "pch.h"
#include "CreateNewBook.h"
#if __has_include("CreateNewBook.g.cpp")
#include "CreateNewBook.g.cpp"
#endif

using namespace winrt;
using namespace Windows::UI::Xaml;
using namespace Windows::Foundation;
using namespace Windows::UI::Core;
using namespace Windows::Storage;
using namespace Windows::Storage::Streams;
using namespace Windows::Storage::Pickers;
using namespace Windows::Storage::Compression;



namespace winrt::reader::implementation
{

    CreateNewBook::CreateNewBook()
    {
        InitializeComponent();
    }

    int32_t CreateNewBook::MyProperty()
    {
        throw hresult_not_implemented();
    }

    void CreateNewBook::MyProperty(int32_t /* value */)
    {
        throw hresult_not_implemented();
    }

    winrt::fire_and_forget CreateNewBook::Click_SelectImages(IInspectable const&, RoutedEventArgs const&)
    {
        // Create a FileOpenPicker instance
        Windows::Storage::Pickers::FileOpenPicker filePicker;
        filePicker.ViewMode(Windows::Storage::Pickers::PickerViewMode::Thumbnail);
        filePicker.SuggestedStartLocation(Windows::Storage::Pickers::PickerLocationId::PicturesLibrary);

        // Add file type filters for image files
        filePicker.FileTypeFilter().Append(L".jpg");
        filePicker.FileTypeFilter().Append(L".jpeg");
        filePicker.FileTypeFilter().Append(L".png");
        filePicker.FileTypeFilter().Append(L".bmp");

        // Set the picker to allow multiple file selection
        filePicker.CommitButtonText(L"Select Images");

        // Show the picker and handle the selected files asynchronously
        auto selectedFiles = co_await filePicker.PickMultipleFilesAsync();
        if (selectedFiles.Size() > 0)
        {
            // Handle the selected files here
            Windows::Storage::Pickers::FolderPicker folderPicker;
            folderPicker.ViewMode(Windows::Storage::Pickers::PickerViewMode::Thumbnail);
            folderPicker.SuggestedStartLocation(Windows::Storage::Pickers::PickerLocationId::PicturesLibrary);
            //folderPicker.FileTypeFilter().Append(L".cbz");

            auto folder = co_await folderPicker.PickSingleFolderAsync();
            if (folder != nullptr)
            {
                auto zipFile = co_await folder.CreateFileAsync(L"images.cbz", Windows::Storage::CreationCollisionOption::ReplaceExisting);
                auto zipStream = co_await zipFile.OpenAsync(Windows::Storage::FileAccessMode::ReadWrite);
                auto zipOutputStream = zipStream.GetOutputStreamAt(0);

                for (const auto& file : selectedFiles)
                {
                    // Open the image file for reading
                    auto stream = co_await file.OpenReadAsync();
                    auto inputStream = stream.GetInputStreamAt(0);
                    auto dataReader = Windows::Storage::Streams::DataReader(inputStream);

                    // Create a compressor for the zip archive
                    Windows::Storage::Compression::Compressor compressor(zipOutputStream, Windows::Storage::Compression::CompressAlgorithm::Lzms, 0);

                    // Read and compress the image data
                    co_await dataReader.LoadAsync(stream.Size());
                    auto imageData = dataReader.DetachBuffer();
                    auto compressedSize = co_await compressor.WriteAsync(imageData);

                    // Flush the compressor to complete the compression process
                    co_await compressor.FlushAsync();

                    // Print the file name and compressed size
                    std::wcout << L"Compressed file: " << file.Name().c_str() << L" - " << compressedSize << L" bytes" << std::endl;
                }

                co_await zipOutputStream.FlushAsync();
                zipStream.Close();
            }
        }
    }

    void CreateNewBook::Click_Save(IInspectable const&, RoutedEventArgs const&)
    {
        // Handle the "Select Images" button click event here
    }
}

