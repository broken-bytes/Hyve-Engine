#pragma once

#include <shared/Exported.hxx>
#include <shared/NativePointer.hxx>

#ifdef __cplusplus 
extern "C" {
#endif



	/**
	 * @brief Initializes the IO system
	 * @return 0 if the system was initialized successfully, otherwise error code
	 */
	uint8_t IO_Init(const char* path);
	/**
		* @brief Loads a file into a buffer
		* @param path The path to the file
		* @param buffer The buffer to save
		* @param size The size of the buffer
		* @return 0 if the file was loaded successfully, otherwise error code
	*/
	 EXPORTED uint8_t IO_LoadFile(const char* path, uint8_t** buffer, size_t* size);

	 /**
	 * @brief Frees a buffer
	 * @param buffer The buffer to free
	 * @note This function is used to free buffers allocated by IO_LoadFile or IO_LoadFileFromArchive
	 */
	 EXPORTED void IO_FreeBuffer(uint8_t* buffer);

	/**
	 * @brief Creates an archive
	 *
	 * @param path The path to the archive
	 */
	EXPORTED int8_t IO_CreateArchive(
		const char* path
	);

	/**
	 * @brief Checks if a file exists in an archive
	 *
	 * @param archivePath The path to the archive
	 * @param filePath The path to the file
	 */
	EXPORTED bool IO_CheckIfFileExists(
		const char* filePath
	);

	/**
	* @brief Saves a buffer to an archive
	*
	* @param path The path to the archive
	* @param name The name of the file
	* @param buffer The buffer to save
	*/
	EXPORTED int8_t IO_SaveBufferToArchive(
		const char* path,
		const char* name,
		const char* buffer,
		size_t len
	);
#ifdef __cplusplus
}
#endif
