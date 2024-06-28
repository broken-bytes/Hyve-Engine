#pragma once

#include "io/filesystem/FileHandle.hxx"
#include <filesystem>
#include <string>

namespace kyanite::engine::io {
	class FileSystem {
	public:
		explicit FileSystem(const std::string& path);
		explicit FileSystem(const std::filesystem::path& path);
		[[nodiscard]] bool CheckFileExists(const std::string& path) const;
		[[nodiscard]] bool CheckFileExists(const std::filesystem::path& path) const;
		[[nodiscard]] std::string ReadFile(const std::string& path) const;
		[[nodiscard]] std::string ReadFile(const std::filesystem::path& path) const;
		void WriteFile(std::string& path, std::string data) const;
		void WriteFile(std::filesystem::path& path, std::string data) const;

	private:
		std::filesystem::path _root;
		std::shared_ptr<FileHandle> _fileHandle;

		void QueryArchive(const std::string& path) const;
		void QueryArchive(const std::filesystem::path& path) const;
	};
}