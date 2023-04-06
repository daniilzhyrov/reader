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
    /*winrt::Windows::Storage::Streams::IBuffer CompressDataWithXpress(winrt::Windows::Storage::Streams::IBuffer data)
    {
        winrt::Windows::Storage::Streams::InMemoryRandomAccessStream inputStream;
        winrt::Windows::Storage::Streams::InMemoryRandomAccessStream outputStream;
        auto inputDataWriter = winrt::Windows::Storage::Streams::DataWriter{ inputStream.GetOutputStreamAt(0) };

        inputDataWriter.WriteBuffer(data);
        inputDataWriter.StoreAsync().get();

        auto compressor = winrt::Windows::Storage::Compression::Compressor{ outputStream, winrt::Windows::Storage::Compression::CompressAlgorithm::Xpress, 0 };
        auto dataReader = winrt::Windows::Storage::Streams::DataReader{ inputStream.GetInputStreamAt(0) };
        dataReader.LoadAsync(data.Length()).get();

        auto readBuffer = dataReader.ReadBuffer(data.Length());
        auto compressedSize = compressor.WriteAsync(readBuffer).get();
        compressor.FlushAsync().get();

        dataReader = winrt::Windows::Storage::Streams::DataReader{ outputStream.GetInputStreamAt(0) };
        dataReader.LoadAsync(compressedSize).get();
        return dataReader.ReadBuffer(compressedSize);
    }*/

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

    //winrt::fire_and_forget CreateNewBook::Click_SelectImages(IInspectable const&, RoutedEventArgs const&)
    //{
    //    // Create a FileOpenPicker instance
    //    Windows::Storage::Pickers::FileOpenPicker filePicker;
    //    filePicker.ViewMode(Windows::Storage::Pickers::PickerViewMode::Thumbnail);
    //    filePicker.SuggestedStartLocation(Windows::Storage::Pickers::PickerLocationId::PicturesLibrary);

    //    // Add file type filters for image files
    //    filePicker.FileTypeFilter().Append(L".jpg");
    //    filePicker.FileTypeFilter().Append(L".jpeg");
    //    filePicker.FileTypeFilter().Append(L".png");
    //    filePicker.FileTypeFilter().Append(L".bmp");

    //    // Set the picker to allow multiple file selection
    //    filePicker.CommitButtonText(L"Select Images");

    //    // Show the picker and handle the selected files asynchronously
    //    auto selectedFiles = co_await filePicker.PickMultipleFilesAsync();

    //    if (selectedFiles.Size() > 0)
    //    {
    //        // Handle the selected files here
    //        Windows::Storage::Pickers::FolderPicker folderPicker;
    //        folderPicker.ViewMode(Windows::Storage::Pickers::PickerViewMode::Thumbnail);
    //        folderPicker.SuggestedStartLocation(Windows::Storage::Pickers::PickerLocationId::PicturesLibrary);
    //        //folderPicker.FileTypeFilter().Append(L".cbz");

    //        auto folder = co_await folderPicker.PickSingleFolderAsync();

    //        if (folder != nullptr)
    //        {
    //            auto zipFile = co_await folder.CreateFileAsync(L"images.cbz", Windows::Storage::CreationCollisionOption::ReplaceExisting);
    //            auto zipStream = co_await zipFile.OpenAsync(Windows::Storage::FileAccessMode::ReadWrite);

    //            auto zipOutputStream = zipStream.GetOutputStreamAt(0);

    //            for (const auto& file : selectedFiles)
    //            {
    //                // Open the image file for reading
    //                auto stream = co_await file.OpenReadAsync();
    //                auto inputStream = stream.GetInputStreamAt(0);
    //                auto dataReader = Windows::Storage::Streams::DataReader(inputStream);

    //                // Create a compressor for the zip archive
    //                Windows::Storage::Compression::Compressor compressor(zipOutputStream, Windows::Storage::Compression::CompressAlgorithm::Xpress, 0);

    //                // Read and compress the image data
    //                co_await dataReader.LoadAsync(stream.Size());
    //                auto imageData = dataReader.DetachBuffer();
    //                auto compressedSize = co_await compressor.WriteAsync(imageData);

    //                // Flush the compressor to complete the compression process
    //                co_await compressor.FlushAsync();

    //                // Print the file name and compressed size
    //                std::wcout << L"Compressed file: " << file.Name().c_str() << L" - " << compressedSize << L" bytes" << std::endl;
    //            }

    //            co_await zipOutputStream.FlushAsync();
    //            zipStream.Close();
    //        }
  
    //    }
    //}


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

            auto folder = co_await folderPicker.PickSingleFolderAsync();

            if (folder != nullptr)
            {
                std::wstring zipFilePath = folder.Path().c_str() + std::wstring(L"\\images.cbz");

                // Convert wide string to narrow string using Windows functions
                int requiredSize = WideCharToMultiByte(CP_UTF8, 0, zipFilePath.c_str(), -1, nullptr, 0, nullptr, nullptr);
                std::string narrowZipFilePath(requiredSize, '\0');
                WideCharToMultiByte(CP_UTF8, 0, zipFilePath.c_str(), -1, &narrowZipFilePath[0], requiredSize, nullptr, nullptr);

#ifdef _WIN32
                _putenv_s("TMP", narrowZipFilePath.c_str());
                _putenv_s("TEMP", narrowZipFilePath.c_str());
#else
                setenv("TMP", narrowZipFilePath.c_str(), 1);
                setenv("TEMP", narrowZipFilePath.c_str(), 1);
#endif

                int error = 0;
                zip_t* archive = zip_open(narrowZipFilePath.c_str(), ZIP_CREATE | ZIP_EXCL, &error);

                if (!archive)
                {
                    winrt::hstring msg = L"Error opening archive. Error code: " + winrt::to_hstring(error);
                    winrt::Windows::UI::Xaml::Controls::ContentDialog errorDialog;
                    errorDialog.Title(box_value(L"Error"));
                    errorDialog.Content(box_value(msg));
                    errorDialog.CloseButtonText(L"OK");
                    co_await errorDialog.ShowAsync();
                    co_return;
                }

                for (const auto& file : selectedFiles)
                {
                    // Open the image file for reading
                    auto stream = co_await file.OpenReadAsync();
                    auto inputStream = stream.GetInputStreamAt(0);
                    auto dataReader = Windows::Storage::Streams::DataReader(inputStream);

                    // Read the image data
                    co_await dataReader.LoadAsync(stream.Size());
                    auto imageData = dataReader.DetachBuffer();
                    std::vector<uint8_t> buffer(imageData.Length());
                    auto dataReader2 = Windows::Storage::Streams::DataReader::FromBuffer(imageData);
                    dataReader2.ReadBytes(buffer);

                    // Convert wide string to narrow string using Windows functions
                    int requiredSize = WideCharToMultiByte(CP_UTF8, 0, file.Name().c_str(), -1, nullptr, 0, nullptr, nullptr);
                    std::string narrowFileName(requiredSize, '\0');
                    WideCharToMultiByte(CP_UTF8, 0, file.Name().c_str(), -1, &narrowFileName[0], requiredSize, nullptr, nullptr);

                    // Add a new entry to the zip archive
                    zip_source_t* source = zip_source_buffer(archive, buffer.data(), buffer.size(), 0);
                    if (source == nullptr)
                    {
                        winrt::hstring msg = L"Error creating zip source for file: " + file.Name();
                        winrt::Windows::UI::Xaml::Controls::ContentDialog errorDialog;
                        errorDialog.Title(box_value(L"Error"));
                        errorDialog.Content(box_value(msg));
                        errorDialog.CloseButtonText(L"OK");
                        co_await errorDialog.ShowAsync();
                        zip_close(archive);
                        co_return;
                    }

                    zip_int64_t index = zip_file_add(archive, narrowFileName.c_str(), source, ZIP_FL_ENC_UTF_8);
                    if (index < 0)
                    {
                        winrt::hstring msg = L"Error adding file to archive: " + file.Name();
                        winrt::Windows::UI::Xaml::Controls::ContentDialog errorDialog;
                        errorDialog.Title(box_value(L"Error"));
                        errorDialog.Content(box_value(msg));
                        errorDialog.CloseButtonText(L"OK");
                        co_await errorDialog.ShowAsync();
                        co_return;
                    }
                    zip_source_free(source);
                }

                if (archive == nullptr)
                {
                    winrt::hstring msg = L"Archive not valid";
                    winrt::Windows::UI::Xaml::Controls::ContentDialog errorDialog;
                    errorDialog.Title(box_value(L"Error"));
                    errorDialog.Content(box_value(msg));
                    errorDialog.CloseButtonText(L"OK");
                    co_await errorDialog.ShowAsync();
                    co_return;
                }

                zip_int64_t num_entries = zip_get_num_entries(archive, 0);

                int close_result = zip_close(archive);
                if (close_result == -1)
                {
                    const char* error_str = zip_strerror(archive);
                    winrt::hstring errorMsg = winrt::to_hstring(error_str);
                    winrt::hstring msg = L"Error closing archive: " + errorMsg;
                    winrt::Windows::UI::Xaml::Controls::ContentDialog errorDialog;
                    errorDialog.Title(box_value(L"Error"));
                    errorDialog.Content(box_value(msg));
                    errorDialog.CloseButtonText(L"OK"); 
                    co_await errorDialog.ShowAsync();
                    co_return;
                }
            }
        }
    }

   
}

