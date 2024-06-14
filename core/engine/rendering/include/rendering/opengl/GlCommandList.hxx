#pragma once

#include "../CommandList.hxx"
#include "../CommandListType.hxx"
#include "../IndexBuffer.hxx"
#include "../PrimitiveTopology.hxx"
#include "../VertexBuffer.hxx"

#include <glad/glad.h>

#include <functional>
#include <vector>

namespace kyanite::engine::rendering::opengl {
	class GlMaterial;
	class GlCommandList: public CommandList {

	// Used by GlCommandQueue to access the _commands vector
	friend class GlCommandQueue;

	public:
		GlCommandList(CommandListType type);
		~GlCommandList();
		virtual auto Begin() -> void override;
		virtual auto Close() -> void override;
		virtual auto Reset(std::shared_ptr<CommandAllocator>&) -> void override;
		virtual auto ClearRenderTarget(glm::vec4 color) -> void override;
		virtual auto SetViewport(
			uint32_t x, 
			uint32_t y, 
			uint32_t width, 
			uint32_t height, 
			uint32_t minDepth,
			uint32_t maxDepth
		) -> void override;
		virtual auto SetScissorRect(uint32_t left, uint32_t top, uint32_t right, uint32_t bottom) -> void override;
		virtual auto SetPrimitiveTopology(PrimitiveTopology topology) -> void override;
		virtual auto SetViewMatrix(glm::mat4 viewMatrix) -> void override;
		virtual auto SetProjectionMatrix(glm::mat4 projectionMatrix) -> void override;
		virtual auto SetMaterial(std::shared_ptr<Material>& material) -> void override;
		virtual auto BindVertexBuffer(std::shared_ptr<VertexBuffer>& vertexBuffer) -> void override;
		virtual auto BindIndexBuffer(std::shared_ptr<IndexBuffer>& indexBuffer) -> void override;
		virtual auto DrawIndexed(glm::mat4 model, uint32_t numIndices, uint32_t startIndex, uint32_t startVertex) -> void override;

	private:
		std::vector<std::function<void()>> _commands;
		GLenum _primitiveTopology;
		std::shared_ptr<GlMaterial> _currentMaterial;
		glm::mat4 _viewMatrix;
		glm::mat4 _projectionMatrix;
	};
}