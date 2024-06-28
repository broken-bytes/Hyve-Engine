#include "io/filesystem/FileSystem.hxx"
#include "io/filesystem/FileHandle.hxx"
#include <filesystem>
#include <iostream>
#include <memory>
#include <minizip-ng/mz.h>
#include <minizip-ng/unzip.h>
#include <minizip-ng/zip.h>

namespace kyanite::engine::io {
	FileSystem::FileSystem(const std::string& path) : FileSystem(std::filesystem::path(path)) {}

	FileSystem::FileSystem(const std::filesystem::path& path) : _root(path) {
		// Create a root file handle "/"
		_fileHandle = std::make_shared<FileHandle>("/");

		// If the path does not exist, create it
		if (!std::filesystem::exists(path)) {
			std::filesystem::create_directories(path);
		}

		// If the path is a file we throw an exception
		if (std::filesystem::is_regular_file(path)) {
			throw std::runtime_error("Path is a file, not a directory");
		}

		// Go over all files in the directory and assume they are zip files
		for (const auto& entry : std::filesystem::directory_iterator(path / "bundles")) {
			QueryArchive(entry.path());
		}
	}

	bool FileSystem::CheckFileExists(const std::string& path) const {
		return CheckFileExists(std::filesystem::path::path(path));
	}

	bool FileSystem::CheckFileExists(const std::filesystem::path& path) const {
		// Split the path by '/'
		auto splitPath = path.string();

		std::string delimiter = "/";
		size_t pos = 0;
		std::string token;
		auto currentFileHandle = _fileHandle;

		while ((pos = splitPath.find(delimiter)) != std::string::npos) {
			token = splitPath.substr(0, pos);

			// Check if the current file handle has a child named like this one
			if (auto found = currentFileHandle->FindChild(token)) {
				currentFileHandle = found;
			}
			else {
				// If not, the file does not exist
				return false;
			}

			// Erase the token from the path, so we can continue with the next one
			// NOTE: The last part of the path is the filename and will not be erased
			splitPath.erase(0, pos + delimiter.length());
		}

		// Check the last part of the path, which is the filename, if it exists
		if (!splitPath.empty()) {
			// We also need to check if the file already exists
			if (auto found = currentFileHandle->FindChild(splitPath)) {
				// If it does, we need to update the file handle
				// Since this is a file, we assume that the other file has priority and ignore this one
				return true;
			}
		}

		return false;
	}

	std::string FileSystem::ReadFile(const std::string& path) const {
		return ReadFile(std::filesystem::path(path));
	}

	std::string FileSystem::ReadFile(const std::filesystem::path& path) const {
		// First split the path by '/'
		auto splitPath = path.string();

		std::string delimiter = "/";
		size_t pos = 0;
		std::string token;
		auto currentFileHandle = _fileHandle;

		while ((pos = splitPath.find(delimiter)) != std::string::npos) {
			token = splitPath.substr(0, pos);

			// Check if the current file handle has a child named like this one
			if (auto found = currentFileHandle->FindChild(token)) {
				currentFileHandle = found;
			}

			// Erase the token from the path, so we can continue with the next one
			// NOTE: The last part of the path is the filename and will not be erased
			splitPath.erase(0, pos + delimiter.length());
		}

		// Check the last part of the path, which is the filename, if it exists
		if (!splitPath.empty()) {
			// We also need to check if the file already exists
			if (auto found = currentFileHandle->FindChild(splitPath)) {
				// We found the file handle, now we need to read the file
				// The handle holds the archive in which the file is
				// We need to open the archive and read the file

				auto zip = unzOpen(found->MountPoint.c_str());

				// Read the file from the archive
				if (unzLocateFile(zip, path.string().c_str(), 0) != UNZ_OK) {
					std::cerr << "Failed to locate file in archive" << std::endl;
					return "";
				}

				// We know the file exists, so we can read it
				unz_file_info fileInfo;
				if (unzGetCurrentFileInfo(zip, &fileInfo, nullptr, 0, nullptr, 0, nullptr, 0) != UNZ_OK) {
					std::cerr << "Failed to get current file info" << std::endl;
					return "";
				}

				// Now we need to read the file
				if (unzOpenCurrentFile(zip) != UNZ_OK) {
					std::cerr << "Failed to open current file" << std::endl;
					return "";
				}

				// Use a vector<char> to store binary data
				std::vector<char> fileContent(fileInfo.uncompressed_size);
				unzReadCurrentFile(zip, fileContent.data(), fileInfo.uncompressed_size);

				unzCloseCurrentFile(zip);
				unzClose(zip);

				// Convert vector<char> to std::string if needed (but usually not recommended for binary data)
				std::string strFileContent(fileContent.begin(), fileContent.end());
				return strFileContent;
			}
			else {
				// If it does not, the file does not exist
				return "";
			}
		}

		return "";
	}

	void FileSystem::WriteFile(std::string& path, std::string data) const {
	}

	void FileSystem::WriteFile(std::filesystem::path& path, std::string data) const {

	}

	void FileSystem::QueryArchive(const std::string& path) const {
		return QueryArchive(std::filesystem::path(path));
	}

	void FileSystem::QueryArchive(const std::filesystem::path& path) const {
		unzFile zipfile = unzOpen(path.string().c_str());
		if (zipfile == nullptr) {
			std::cerr << "Failed to open ZIP file: " << path << std::endl;
			return;
		}

		if (unzGoToFirstFile(zipfile) != UNZ_OK) {
			std::cerr << "Failed to go to first file in ZIP file: " << path << std::endl;
			unzClose(zipfile);
			return;
		}

		auto currentFileHandle = _fileHandle;

		do {
			currentFileHandle = _fileHandle;
			char filename[1024];
			unz_file_info fileInfo;
			if (unzGetCurrentFileInfo(zipfile, &fileInfo, filename, sizeof(filename), nullptr, 0, nullptr, 0) != UNZ_OK) {
				std::cerr << "Failed to get current file info" << std::endl;
				break;
			}

			// We can, by splitting the filename by '/', get the path to the file and check the file handle
			auto splitPath = std::string(filename);

			std::string delimiter = "/";

			size_t pos = 0;
			std::string token;
			while ((pos = splitPath.find(delimiter)) != std::string::npos) {
				token = splitPath.substr(0, pos);

				// Print the token
				std::cout << token << std::endl;
				
				// Check if the current file handle has a child named like this one
				if (auto found = currentFileHandle->FindChild(token)) {
					currentFileHandle = found;
				} else {
					// If not, create a new file handle and add it to the current file handle
					auto newFileHandle = std::make_shared<FileHandle>(token);
					newFileHandle->MountPoint = path.string();
					currentFileHandle->AddChild(newFileHandle);
					currentFileHandle = newFileHandle;
				}

				// Erase the token from the path, so we can continue with the next one
				// NOTE: The last part of the path is the filename and will not be erased
				splitPath.erase(0, pos + delimiter.length());
			}

			// Check the last part of the path, which is the filename, if it exists
			if (!splitPath.empty()) {
				std::cout << splitPath << std::endl;
				// We also need to check if the file already exists
				if (auto found = currentFileHandle->FindChild(splitPath)) {
					// If it does, we need to update the file handle
					// Since this is a file, we assume that the other file has priority and ignore this one
					continue;
				}
				else {
					// If it does not, we need to create a new file handle
					auto newFileHandle = std::make_shared<FileHandle>(splitPath);
					newFileHandle->MountPoint = path.string();
					currentFileHandle->AddChild(newFileHandle);
				}
			}

		} while (unzGoToNextFile(zipfile) == UNZ_OK);

		unzClose(zipfile);

		return;
	}
}