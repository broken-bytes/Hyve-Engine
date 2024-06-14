#include "rendering/GraphicsContext.hxx"
#include <logger/Logger.hxx>


namespace kyanite::engine::rendering {
    auto GraphicsContext::Begin() -> void {
		_commandList->Reset(_commandAllocator);
	}

    auto GraphicsContext::Finish() -> void {
        _commandQueue->Execute({ _commandList });
    }

    auto GraphicsContext::ClearRenderTarget() -> void {
        _commandList->ClearRenderTarget({ 0.2f, 0.1f, 0.1f, 1.f });
    }

    auto GraphicsContext::SetRenderTarget(std::shared_ptr<RenderTarget> target) -> void {

    }

    auto GraphicsContext::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height, uint32_t minDepth, uint32_t maxDepth) -> void {
        _commandList->SetViewport(x, y, width, height, minDepth, maxDepth);
    }

    auto GraphicsContext::SetScissorRect(long left, long top, long right, long bottom) -> void {
        _commandList->SetScissorRect(left, top, right, bottom);
    }

    auto GraphicsContext::SetPrimitiveTopology(PrimitiveTopology topology) -> void {
        _commandList->SetPrimitiveTopology(topology);
    }

    auto GraphicsContext::SetViewMatrix(glm::mat4 view) -> void {
		_commandList->SetViewMatrix(view);
	}

    auto GraphicsContext::SetProjectionMatrix(glm::mat4 projection) -> void {
        _commandList->SetProjectionMatrix(projection);
    }

    auto GraphicsContext::SetVertexBuffer(uint8_t index, std::shared_ptr<VertexBuffer>& buffer) -> void {
        _commandList->BindVertexBuffer(buffer);
    }

    auto GraphicsContext::SetIndexBuffer(std::shared_ptr<IndexBuffer>& buffer) -> void {
        _commandList->BindIndexBuffer(buffer);
    }

    auto GraphicsContext::SetMaterial(std::shared_ptr<Material>& material) -> void {
        _commandList->SetMaterial(material);
    }

    auto GraphicsContext::DrawIndexed(glm::mat4& model, uint32_t indexCount, uint32_t startIndexLocation, int32_t baseVertexLocation) -> void {
        _commandList->DrawIndexed(model, indexCount, startIndexLocation, baseVertexLocation);
    }
}