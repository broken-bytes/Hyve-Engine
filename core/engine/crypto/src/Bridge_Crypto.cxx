#include "crypto/Bridge_Crypto.h"
#include <sodium/crypto_hash_sha512.h>

uint8_t Crypto_Hash(uint8_t* data, int len, uint8_t** buffer, size_t* bufferLen) {
	crypto_hash_sha512_state state;
	auto* hash = new uint8_t[crypto_hash_sha512_BYTES];
	crypto_hash_sha512_init(&state);
	crypto_hash_sha512_update(&state, data, len);
	crypto_hash_sha512_final(&state, hash);

	*buffer = hash;
	*bufferLen = crypto_hash_sha512_BYTES;

	return 0;
}

void Crypto_FreeBuffer(uint8_t* buffer) {
	delete[] buffer;
}
