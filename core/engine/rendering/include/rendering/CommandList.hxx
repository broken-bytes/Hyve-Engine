#pragma once

#include "CommandAllocator.hxx"
#include "CommandListType.hxx"
#include "Vertex.hxx"
#include "IndexBuffer.hxx"
#include "PrimitiveTopology.hxx"
#include "VertexArray.hxx"
#include "VertexBuffer.hxx"
#include "Mesh.hxx"
#include "Material.hxx"

#include <glm/glm.hpp>

namespace kyanite::engine::rendering {
	class CommandList {
	public:
		CommandList(CommandListType type) : _type(type) {};
		virtual ~CommandList() = default;
		virtual auto Begin() -> void = 0;
		virtual auto Close() -> void = 0;
		virtual auto Reset(std::shared_ptr<CommandAllocator>&) -> void = 0;
		virtual auto ClearRenderTarget(glm::vec4 color) -> void = 0;
		virtual auto SetViewport(
			uint32_t x, 
			uint32_t y, 
			uint32_t width, 
			uint32_t height,
			uint32_t minDepth,
			uint32_t maxDepth
		) -> void = 0;
		virtual auto SetScissorRect(uint32_t left, uint32_t top, uint32_t right, uint32_t bottom) -> void = 0;
		virtual auto SetViewMatrix(glm::mat4 viewMatrix) -> void = 0;
		virtual auto SetProjectionMatrix(glm::mat4 projectionMatrix) -> void = 0;
		virtual auto SetPrimitiveTopology(PrimitiveTopology topology) -> void = 0;
		virtual auto SetMaterial(std::shared_ptr<Material> material) -> void = 0;
		virtual auto BindVertexArray(std::shared_ptr<VertexArray> vertexArray) -> void const = 0;
		virtual auto BindVertexBuffer(uint8_t index, std::shared_ptr<VertexBuffer> vertexBuffer) -> void const = 0;
		virtual auto BindIndexBuffer(std::shared_ptr<IndexBuffer> vertexBuffer) -> void const = 0;
		virtual auto DrawIndexed(glm::mat4 model, uint32_t numIndices, uint32_t startIndex) -> void = 0;
		virtual auto DrawIndexedInstanced(
			uint32_t numIndices,
			uint32_t instanceCount,
			uint32_t startIndexLocation,
			int32_t baseVertexLocation
		) -> void = 0;

		auto Type() const -> CommandListType { return _type; }

	private:
		CommandListType _type;
	};
}