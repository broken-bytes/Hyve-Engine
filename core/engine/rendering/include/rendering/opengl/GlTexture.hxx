#pragma once

#include "rendering/Texture.hxx"
#include "glad/glad.h"
#include <cstdint>
#include <vector>

namespace kyanite::engine::rendering::opengl {
	class GlTexture : public Texture {
	public:
		GlTexture(
			uint32_t width,
			uint32_t height,
			uint8_t channels,
			const uint8_t* data
		) {
			glGenTextures(1, &_id);
			glBindTexture(GL_TEXTURE_2D, _id);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

			glTexImage2D(
				GL_TEXTURE_2D, 
				0, 
				GL_RGBA, 
				width, 
				height, 
				0, 
				GL_BGRA, 
				GL_UNSIGNED_BYTE, 
				data
			);
			glGenerateMipmap(GL_TEXTURE_2D);

			// Unbind
			glBindTexture(GL_TEXTURE_2D, 0);
		}

		~GlTexture() {
			glDeleteTextures(1, &_id);
		}

	private:
		uint64_t _value;
	};
}