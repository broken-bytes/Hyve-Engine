#include "rendering/opengl/GlIndexBuffer.hxx"
#include <iostream>

namespace kyanite::engine::rendering::opengl {
	GlIndexBuffer::GlIndexBuffer(const uint32_t* indices, size_t len) : IndexBuffer(len) {
		_id = 0;
		glGenBuffers(1, &_id);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _id);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, len * sizeof(uint32_t), indices, GL_STATIC_DRAW);

		// Check for errors
		GLenum error = glGetError();
		if (error != GL_NO_ERROR) {
			std::cerr << "Error creating vertex buffer: " << error << std::endl;
		}

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	GlIndexBuffer::~GlIndexBuffer() {
		glDeleteBuffers(1, &_id);
	}
}