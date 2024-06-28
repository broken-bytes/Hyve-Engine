#include "rendering/opengl/GlVertexBuffer.hxx"
#include <iostream>
#include <glad/glad.h>

namespace kyanite::engine::rendering::opengl {
	GlVertexBuffer::GlVertexBuffer(
		const void* data, 
		size_t size, 
		size_t elemSize
	) : VertexBuffer(size) {
		_id = 0;
		glGenBuffers(1, &_id);
		glBindBuffer(GL_ARRAY_BUFFER, _id);
		glBufferData(GL_ARRAY_BUFFER, size * elemSize, data, GL_STATIC_DRAW);

		// Check for errors
		GLenum error = glGetError();
		if (error != GL_NO_ERROR) {
			std::cerr << "Error creating vertex buffer: " << error << std::endl;
		}

		// Unbind the buffer
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	GlVertexBuffer::~GlVertexBuffer() {
		glDeleteBuffers(1, &_id);
	}
}