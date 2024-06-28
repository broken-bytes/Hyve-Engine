#pragma once

#include "../Device.hxx"
#include "../Shader.hxx"

#include <SDL2/SDL.h>

#include <memory>

namespace kyanite::engine::rendering::opengl {
	class GlDevice : public Device, public std::enable_shared_from_this<GlDevice> {
	public:
		GlDevice(SDL_Window* window);
		virtual ~GlDevice();
		virtual auto Shutdown() -> void override;

		// Creation work submission and synchronization
		virtual auto CreateGraphicsContext() -> std::unique_ptr<GraphicsContext> override;
		virtual auto CreateImGuiContext(ImGuiContext* context) -> 
			std::unique_ptr<ImmediateGuiContext> override;
		virtual auto CreateUploadContext() -> std::unique_ptr<UploadContext> override;
		virtual auto CreateCommandList(CommandListType type) -> std::shared_ptr<CommandList> override;
		virtual auto CreateCommandQueue(CommandListType type) -> std::shared_ptr<CommandQueue> override;
		virtual auto CreateCommandAllocator() -> std::shared_ptr<CommandAllocator> override;
		virtual auto CreateFence() -> std::shared_ptr<Fence> override;
		virtual auto CreateSwapchain() -> std::unique_ptr<Swapchain> override;

		// Creation of resources
		virtual auto CreateBuffer(uint64_t size) -> std::shared_ptr<Buffer> override;
		virtual auto CreateRenderTarget() -> std::shared_ptr<RenderTarget> override;
		virtual auto CreateMaterial(
			std::map<ShaderType, std::shared_ptr<Shader>> shaders,
			bool isInstanced
		) -> std::shared_ptr<Material> override;
		virtual auto CompileShader(
			const std::string& shaderSource,
			ShaderType type
		) -> std::shared_ptr<Shader> override;
		virtual auto CreateVertexBuffer(const void* data, uint64_t size, size_t elemSize
		) -> std::shared_ptr<VertexBuffer> override;
		virtual auto UpdateVertexBuffer(std::shared_ptr<VertexBuffer> buffer, const void* data, uint64_t size) -> void override;
		virtual auto CreateIndexBuffer(const uint32_t* indices, size_t len) -> std::shared_ptr<IndexBuffer> override;
		virtual auto UpdateIndexBuffer(std::shared_ptr<IndexBuffer> buffer, std::vector<uint32_t> indices) -> void override;
		virtual auto CreateVertexArray(
			std::shared_ptr<VertexBuffer> vertexBuffer,
			std::shared_ptr<IndexBuffer> indexBuffer
		) -> std::shared_ptr<VertexArray> override;
		auto CreateTexture(
			uint32_t width,
			uint32_t height,
			uint32_t channels,
			const uint8_t* data
		)->std::shared_ptr<Texture> override;

		//Delete resources
		virtual auto DestroyShader(uint64_t shaderHandle) -> void override;

	private:
		SDL_Window* _window;
		SDL_GLContext _glContext;
	};
}