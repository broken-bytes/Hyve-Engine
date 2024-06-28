#include "rendering/IndexBuffer.hxx"
#include "rendering/PrimitiveTopology.hxx"
#include "rendering/VertexBuffer.hxx"
#include "rendering/opengl/GlCommandList.hxx"
#include "rendering/opengl/GlIndexBuffer.hxx"
#include "rendering/opengl/GlMaterial.hxx"
#include "rendering/opengl/GlVertexBuffer.hxx"

#include "glad/glad.h"

#include <glm/gtc/type_ptr.hpp>
#include <stdexcept>

namespace kyanite::engine::rendering::opengl {
	GlCommandList::GlCommandList(CommandListType type) : CommandList(type) {

	}

	GlCommandList::~GlCommandList() {

	}

	auto GlCommandList::Begin() -> void {
		if (!_commands.empty()) {
			throw std::runtime_error("Cannot begin a commandlist that is already in recording state");
		}
	}

	auto GlCommandList::Close() -> void {
		// NOTE: This is a NOP for OpenGL
	}

	auto GlCommandList::Reset(std::shared_ptr<CommandAllocator>& allocator) -> void {
		_commands.clear();
	}

	auto GlCommandList::ClearRenderTarget(glm::vec4 color) -> void {
		_commands.push_back([color]() {
			glClearColor(color.r, color.g, color.b, color.a);
			glClear(GL_COLOR_BUFFER_BIT);
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		});
	}

	auto GlCommandList::SetViewport(
		uint32_t x,
		uint32_t y,
		uint32_t width,
		uint32_t height,
		uint32_t minDepth,
		uint32_t maxDepth
	) -> void {
		_commands.push_back([x, y, width, height]() {
			glViewport(x, y, width, height);
		});
	}

	auto GlCommandList::SetScissorRect(uint32_t left, uint32_t top, uint32_t right, uint32_t bottom) -> void {
		_commands.push_back([left, top, right, bottom]() {
			glScissor(left, top, right, bottom);
		});
	}

	auto GlCommandList::SetPrimitiveTopology(PrimitiveTopology topology) -> void {
			_commands.push_back([this, topology]() {
			switch (topology) {
			case PrimitiveTopology::TRIANGLE_LIST:
				_primitiveTopology = GL_TRIANGLES;
				break;
			case PrimitiveTopology::TRIANGLE_STRIP:
				_primitiveTopology = GL_TRIANGLE_STRIP;
				break;
			case PrimitiveTopology::LINE_LIST:
				_primitiveTopology = GL_LINES;
				break;
			case PrimitiveTopology::LINE_STRIP:
				_primitiveTopology = GL_LINE_STRIP;
				break;
			default:
				throw std::runtime_error("Unsupported primitive topology");
			}
		});
	}

	auto GlCommandList::SetViewMatrix(glm::mat4 viewMatrix) -> void {
		_commands.push_back([this, viewMatrix]() {
			_viewMatrix = viewMatrix;
		});
	}

	auto GlCommandList::SetProjectionMatrix(glm::mat4 projectionMatrix) -> void {
		_commands.push_back([this, projectionMatrix]() {
			_projectionMatrix = projectionMatrix;
		});
	}

	auto GlCommandList::BindVertexArray(std::shared_ptr<VertexArray> vertexArray) -> void const {
		_commands.push_back([this, vertexArray]() {
			vertexArray->Bind();
		});
	}

	auto GlCommandList::SetMaterial(std::shared_ptr<Material> material) -> void {
		_commands.push_back([this, material]() {
			auto glMaterial = std::static_pointer_cast<GlMaterial>(material);
			_currentMaterial = glMaterial;
			_currentMaterial->Bind();
		});

	}

	auto GlCommandList::BindVertexBuffer(uint8_t index, std::shared_ptr<VertexBuffer> vertexBuffer) -> void const {
		_commands.push_back([this, vertexBuffer, index]() {
			vertexBuffer->Bind();

			if (index == 0) {
				glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
				glEnableVertexAttribArray(0);

				// Texture coordinate attribute
				glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uvs));
				glEnableVertexAttribArray(1);
			}
			else {
				// Set up instance attributes (model matrix as 4 vec4s)
				glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
				glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
				glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
				glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));
				glEnableVertexAttribArray(2);
				glEnableVertexAttribArray(3);
				glEnableVertexAttribArray(4);
				glEnableVertexAttribArray(5);

				// Set divisors for instanced attributes
				glVertexAttribDivisor(2, 1);
				glVertexAttribDivisor(3, 1);
				glVertexAttribDivisor(4, 1);
				glVertexAttribDivisor(5, 1);
			}
		});
	}

	auto GlCommandList::BindIndexBuffer(std::shared_ptr<IndexBuffer> indexBuffer) -> void const {
		_commands.push_back([this, indexBuffer]() {
			indexBuffer->Bind();
		});
	}

	auto GlCommandList::DrawIndexed(glm::mat4 model, uint32_t numIndices, uint32_t startIndex) -> void {
		_commands.emplace_back([this, numIndices, startIndex, model]() {
			_currentMaterial->SetBuiltins(model, _viewMatrix, _projectionMatrix);

			GLenum error = glGetError();
			if (error != GL_NO_ERROR) {
				std::cerr << "OpenGL Error: " << error << std::endl;
			}

			glDrawElements(_primitiveTopology, numIndices, GL_UNSIGNED_INT, (void*)(startIndex * sizeof(uint32_t)));
		});
	}

	auto GlCommandList::DrawIndexedInstanced(
		uint32_t numIndices,
		uint32_t instanceCount,
		uint32_t startIndexLocation,
		int32_t baseVertexLocation
	) -> void {
		_commands.emplace_back([this, numIndices, instanceCount, startIndexLocation]() {
			_currentMaterial->SetBuiltins(glm::mat4(1), _viewMatrix, _projectionMatrix);

			GLenum error = glGetError();
			if (error != GL_NO_ERROR) {
				std::cerr << "OpenGL Error: " << error << std::endl;
			}

			glDrawElementsInstanced(_primitiveTopology, numIndices, GL_UNSIGNED_INT, (void*)(startIndexLocation * sizeof(uint32_t)), instanceCount);

			error = glGetError();
			if (error != GL_NO_ERROR) {
				std::cerr << "OpenGL Error: " << error << std::endl;
			}
		});
	}
}