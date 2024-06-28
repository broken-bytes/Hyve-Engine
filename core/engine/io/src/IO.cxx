#include "io/IO.hxx"
#include "io/filesystem/FileSystem.hxx"
#include <crypto/Bridge_Crypto.h>
#include <physfs.h>
#include <stduuid/uuid.h>
#include <filesystem>
#include <fstream>
#include <memory>
#include <minizip-ng/zip.h>
#include <string>
#include <string_view>
#include <cassert>

std::string_view HashArchiveName(std::string& name) {
	uint8_t* buffer;
	size_t size;

	// Conver the name string to a uint8_t_ptr
	auto* bytes = reinterpret_cast<uint8_t*>(name.data());

	Crypto_Hash(bytes, name.size(), &buffer, &size);

	// Copy the buffer to a string_view
	std::string_view hash(reinterpret_cast<char*>(buffer), size);

	return hash;
}

namespace kyanite::engine::io {
	const char* BundlePath = "";
	std::unique_ptr<FileSystem> Filesystem = nullptr;

	uint8_t Init(const char* path) {
		BundlePath = path;
		Filesystem = std::make_unique<FileSystem>(std::string(path));

		return 0;
	}

	auto LoadFileToBuffer(std::string_view path) -> std::vector<uint8_t> {
		auto file = std::ifstream(std::filesystem::path(path), std::ios::binary);
		auto buffer = std::vector<uint8_t>(std::istreambuf_iterator<char>(file), {});

		return buffer;
	}

	auto SaveBufferToFile(std::string_view path, std::vector<uint8_t>& buffer) -> void {
		std::filesystem::create_directories(std::filesystem::path(path).parent_path());
		auto file = std::ofstream(std::filesystem::path(path));
		file.write(reinterpret_cast<const char*>(buffer.data()), buffer.size());
	}

	auto CreateDirectory(std::string_view path) -> void {
		std::filesystem::create_directory(std::filesystem::path(path));
	}

	auto CreateArchive(const char* path) -> int8_t {
		if (auto zip = zipOpen(path, APPEND_STATUS_CREATE); zip != nullptr) {
			zipClose(zip, nullptr);
			return 0;
		}

		printf("Failed to open archive\n");

		return 1;
	}

	auto CreateFile(std::string path) -> void {
		auto file = std::ofstream(std::filesystem::path(path));
		file.close();
	}

	auto RemoveFile(std::string path) -> void {
		std::filesystem::remove(std::filesystem::path(path));
	}

	auto GetFileList(std::filesystem::path path) -> std::vector<std::filesystem::directory_entry> {
		auto files = std::vector<std::filesystem::directory_entry>();

		for (auto& p : std::filesystem::directory_iterator(path)) {
			files.push_back(p);
		}

		return files;
	}

	auto CheckIfFileExists(std::string_view filePath) -> bool {
		// Check if the file exists
		return Filesystem->CheckFileExists(filePath);
	}

	auto SaveBufferToArchive(const char* path, const char* name, const char* buffer, size_t len) -> int8_t {
		return 0;
	}

	auto LoadFileFromArchive(
		const char* path,
		uint8_t** buffer,
		size_t* len
	) -> int8_t {
		auto file = Filesystem->ReadFile(std::string(path));

		if (file.empty()) {
			return 1;
		}

		*buffer = new uint8_t[file.size() + 1];
		*len = file.size() + 1;

		memcpy(*buffer, file.data(), file.size());
		// Null terminate the buffer
		(*buffer)[file.size()] = '\0';

		return 0;
	}

	auto CreateUUID() -> std::string {
		std::random_device rd;
		auto seed_data = std::array<int, std::mt19937::state_size> {};
		std::generate(std::begin(seed_data), std::end(seed_data), std::ref(rd));
		std::seed_seq seq(std::begin(seed_data), std::end(seed_data));
		std::mt19937 generator(seq);
		uuids::uuid_random_generator gen{ generator };

		uuids::uuid const id = gen();
		assert(!id.is_nil());
		assert(id.as_bytes().size() == 16);
		assert(id.version() == uuids::uuid_version::random_number_based);
		assert(id.variant() == uuids::uuid_variant::rfc);

		return uuids::to_string(id);
	}
}