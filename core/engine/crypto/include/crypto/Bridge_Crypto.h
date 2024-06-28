#pragma once

#include <stdint.h>

#ifdef __cplusplus 
extern "C" {
#endif

uint8_t Crypto_Hash(uint8_t* data, int len, uint8_t** buffer, size_t* bufferLen);

void Crypto_FreeBuffer(uint8_t* buffer);

#ifdef __cplusplus 
}
#endif