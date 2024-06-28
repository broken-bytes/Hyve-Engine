#include "io/Bridge_IO.h"
#include "io/IO.hxx"

#include <shared/NativePointer.hxx>

#include <iostream>

uint8_t IO_Init(const char* path) {
	return kyanite::engine::io::Init(path);
}

void IO_FreeBuffer(uint8_t* buffer) {
	delete[] buffer;
}

bool IO_CheckIfFileExists(const char* filePath) {
	return kyanite::engine::io::CheckIfFileExists(filePath);
}

int8_t IO_SaveBufferToArchive(const char* path, const char* name, const char* buffer, size_t len) {
	return kyanite::engine::io::SaveBufferToArchive(path, name, buffer, len);
}

uint8_t IO_LoadFile(const char* path, uint8_t** buffer, size_t* size) {
	return kyanite::engine::io::LoadFileFromArchive(path, buffer, size);
}
