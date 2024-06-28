#include "bundler/Bundle.hxx"
#include <io/Bridge_IO.h>
#include <crypto/Bridge_Crypto.h>
#include <nlohmann/json.hpp>
#include <minizip-ng/zip.h>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>

void ReadPackage(const std::string& path) {
    // Open the file
    std::ifstream file(path);

    nlohmann::json json_data;
    file >> json_data;

    Bundle bundle;

    bundle.name = json_data["name"];
    bundle.files = json_data["files"];

    std::stringstream ss;
    ss << bundle.name << ".bundle";

    std::filesystem::path rootPath(path);
    auto dirPath = rootPath.parent_path();
    auto bundlePath = dirPath / ss.str();

    // Remove the file if it exists
    if (std::filesystem::exists(bundlePath)) {
		std::filesystem::remove(bundlePath);
	}

    // Create the zip file
    zipFile zip = zipOpen(bundlePath.string().c_str(), APPEND_STATUS_CREATE);

    for (const auto& file : bundle.files) {
        std::ifstream file_stream(dirPath / file, std::ios::binary); // Added std::ios::binary
        if (!file_stream) {
            std::cerr << "Error opening file!" << std::endl;
            return;
        }

        // Determine file size
        file_stream.seekg(0, std::ios::end);
        std::streampos file_size = file_stream.tellg();
        file_stream.seekg(0, std::ios::beg);

        // Read data into a vector
        std::vector<char> file_contents(file_size);
        file_stream.read(file_contents.data(), file_size);
        file_stream.close();

        // Add the file to the zip
        zip_fileinfo file_info = { 0 };
        zipOpenNewFileInZip(
            zip, 
            file.c_str(), 
            &file_info, 
            nullptr, 
            0, 
            nullptr, 
            0, 
            nullptr, 
            Z_DEFLATED, 
            MZ_COMPRESS_LEVEL_BEST
        );
        zipWriteInFileInZip(zip, file_contents.data(), file_contents.size());
        zipCloseFileInZip(zip);
	}

    zipClose(zip, nullptr);
}

int main(int argc, char** argv) {
    if (argc < 2) {
		return 1;
	}

    // Get the second argument
    const char* path = argv[1];

    // Check if the file exists
    if (!std::filesystem::exists(path)) {
		return 1;
	}

    ReadPackage(path);

    return 0;
}