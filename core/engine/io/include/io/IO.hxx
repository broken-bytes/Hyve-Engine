#pragma once

#include <shared/Exported.hxx>

#include <cstdint>
#include <filesystem>
#include <memory>
#include <string>
#include <string_view>
#include <vector>

namespace kyanite::engine::io {
    EXPORTED auto Init(const char* path) -> uint8_t;
    EXPORTED auto LoadFileToBuffer(std::string_view path) -> std::vector<uint8_t>;
    EXPORTED auto SaveBufferToFile(std::string_view path, std::vector<uint8_t>& buffer) -> void;
    EXPORTED auto CreateDirectory(std::string_view path) -> void;
    EXPORTED auto CreateArchive(const char* path) -> int8_t;
    EXPORTED auto CreateFile(std::string path) -> void;
    EXPORTED auto RemoveFile(std::string path) -> void;
    EXPORTED auto GetFileList(std::filesystem::path path) -> std::vector<std::filesystem::directory_entry>;
    EXPORTED auto CheckIfFileExists(std::string_view filePath) -> bool;
    EXPORTED auto SaveBufferToArchive(const char* path, const char* name, const char* buffer, size_t len) -> int8_t;
    EXPORTED auto LoadFileFromArchive(
        const char* path, 
        uint8_t** buffer,
        size_t* len
    ) -> int8_t;
    EXPORTED auto CreateUUID() -> std::string;
}
