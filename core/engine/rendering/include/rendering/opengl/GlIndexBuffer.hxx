#pragma once

#include "../IndexBuffer.hxx"

#include <glad/glad.h>

#include <cstdint>
#include <stdexcept>
#include <vector>

namespace kyanite::engine::rendering::opengl {
	class GlIndexBuffer : public IndexBuffer {
	public:
		GlIndexBuffer(const uint32_t* indices, size_t len);
		~GlIndexBuffer();

		auto Bind() const -> void override {
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _id);

			if (glGetError() != GL_NO_ERROR) {
				throw std::runtime_error("Failed to bind index buffer");
			}
		}

		auto SetData(const void* data, size_t size) -> void override {
			Bind();
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
		};

		auto Id() const -> uint64_t { return _id; }

	private:
		GLuint _id;
	};
}