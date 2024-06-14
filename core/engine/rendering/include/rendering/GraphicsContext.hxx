#pragma once

#include "CommandListType.hxx"
#include "Context.hxx"
#include "Device.hxx"
#include "IndexBuffer.hxx"
#include "VertexBuffer.hxx"
#include "PrimitiveTopology.hxx"

#include <memory>

namespace kyanite::engine::rendering {
    class RenderTarget;

    class GraphicsContext: public Context {
    public:
        GraphicsContext(
            const std::shared_ptr<Device>& device, 
            std::shared_ptr<CommandQueue> queue
        ) : Context(CommandListType::Graphics, device, queue) { }
        virtual ~GraphicsContext() = default;
        virtual auto Begin() -> void override;
        virtual auto Finish() -> void override;
        virtual auto ClearRenderTarget() -> void;
        virtual auto SetRenderTarget(std::shared_ptr<RenderTarget> target) -> void;
        virtual auto SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height, uint32_t minDepth, uint32_t maxDepth) -> void;
        virtual auto SetScissorRect(long left, long top, long right, long bottom) -> void;
        virtual auto SetPrimitiveTopology(PrimitiveTopology topology) -> void;
        virtual auto SetViewMatrix(glm::mat4 view) -> void;
        virtual auto SetProjectionMatrix(glm::mat4 projection) -> void;
        virtual auto SetVertexBuffer(uint8_t index, std::shared_ptr<VertexBuffer>& buffer) -> void;
        virtual auto SetIndexBuffer(std::shared_ptr<IndexBuffer>& buffer) -> void;
        virtual auto SetMaterial(std::shared_ptr<Material>& material) -> void;
        virtual auto DrawIndexed(glm::mat4& model, uint32_t indexCount, uint32_t startIndexLocation, int32_t baseVertexLocation) -> void;
    };
}