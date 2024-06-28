#include "rendering/Rendering.hxx"
#include "rendering/Device.hxx"
#include "rendering/opengl/GLDevice.hxx"
#include "rendering/GraphicsContext.hxx"
#include "rendering/ImGuiContext.hxx"
#include "rendering/UploadContext.hxx"
#include "rendering/VertexArray.hxx"
#include "rendering/VertexBuffer.hxx"
#include "rendering/DeviceFactory.hxx"

#include <concurrentqueue/concurrentqueue.h>
#include <FreeImagePlus.h>
#include <glad/glad.h>
#include <SDL.h>
#include <imgui.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>
#include <filesystem>
#include <stdexcept>
#include <map>
#include <memory>
#include <mutex>
#include <rendering/renderdoc_app.h>

uint32_t CreateSpriteVao() {
	std::vector<float> vertices = {
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.0f, 1.0f, 1.0f,
		-0.5f, 0.5f, 0.0f, 0.0f, 1.0f
	};
	uint32_t vb = kyanite::engine::rendering::CreateVertexBuffer(vertices.data(), vertices.size(), sizeof(float));

	// Create the index buffer
	std::vector<uint32_t> indices = {
		0, 1, 2,
		2, 3, 0
	};
	uint32_t ib = kyanite::engine::rendering::CreateIndexBuffer(indices.data(), indices.size());

	// Create the vertex array
	uint32_t vao = kyanite::engine::rendering::CreateVertexArray(vb, ib);

	return vao;
}

struct DrawCall {
	glm::mat4 model;
	uint32_t vao;
	uint32_t material;
};

struct InstancedDrawCall {
	uint32_t vao;
	uint32_t material;
	std::vector<glm::mat4> models;
};

namespace kyanite::engine::rendering {
	std::shared_ptr<Device> device = nullptr;
	SDL_Window* window = nullptr;

	// Resource management
	std::map<uint64_t, std::shared_ptr<VertexBuffer>> vertexBuffers = {};
	std::map<uint64_t, std::shared_ptr<IndexBuffer>> indexBuffers = {};
	std::map<uint64_t, std::shared_ptr<VertexArray>> vertexArrays = {};
	std::map<uint32_t, std::shared_ptr<Texture>> textures = {};
	std::map<uint32_t, std::shared_ptr<Shader>> shaders = {};
	std::map<uint32_t, std::shared_ptr<Material>> materials = {};
	std::map<uint32_t, std::shared_ptr<Mesh>> meshes = {};

	std::unique_ptr<GraphicsContext> graphicsContext = nullptr;
	std::unique_ptr<ImmediateGuiContext> imguiContext = nullptr;
	std::unique_ptr<UploadContext> uploadContext = nullptr;
	std::unique_ptr<Swapchain> swapchain = nullptr;
	moodycamel::ConcurrentQueue<DrawCall> drawCalls;
	moodycamel::ConcurrentQueue<DrawCall> instancedDrawCalls;
	std::vector<DrawCall> mergedDrawCalls;
	std::vector<std::shared_ptr<VertexBuffer>> instanceBuffers;

	uint32_t spriteVao = 0;

	auto Init(NativePointer window, ImGuiContext* context) -> void {
		SDL_InitSubSystem(SDL_INIT_VIDEO);
		auto sdlWindow = reinterpret_cast<SDL_Window*>(window);
		kyanite::engine::rendering::window = sdlWindow;

		RENDERDOC_API_1_1_2* rdoc_api = NULL;

		// At init, on windows
		if (HMODULE mod = GetModuleHandleA("renderdoc.dll"))
		{
			pRENDERDOC_GetAPI RENDERDOC_GetAPI =
				(pRENDERDOC_GetAPI)GetProcAddress(mod, "RENDERDOC_GetAPI");
			int ret = RENDERDOC_GetAPI(eRENDERDOC_API_Version_1_1_2, (void**)&rdoc_api);
			assert(ret == 1);
		}

		// Create a device
		device = DeviceFactory::CreateDevice(RenderBackendType::OpenGL, sdlWindow);

		graphicsContext = device->CreateGraphicsContext();
		imguiContext = device->CreateImGuiContext(context);
		uploadContext = device->CreateUploadContext();
		swapchain = device->CreateSwapchain();

		// Register the sprite vertex array
		spriteVao = CreateSpriteVao();
	}

	auto Shutdown() -> void {
		// Cleanup
		device = nullptr;
		SDL_QuitSubSystem(SDL_INIT_VIDEO);
	}

	inline auto PreFrame() -> void {
		// ImGui new frame, resource loading, etc.
		// Start the ImGui frame
		graphicsContext->Begin();
		imguiContext->Begin();
		uploadContext->Begin();
		graphicsContext->ClearRenderTarget();
		graphicsContext->SetViewport(0, 0, 1920, 1080, 0.0, 1.0);
		graphicsContext->SetScissorRect(0, 0, 640, 360);
		// View Matrix
		glm::mat4 view = glm::lookAtLH(
			glm::vec3(0.0f, 0.0f, -1.0f),    // Camera position (Z = -1)
			glm::vec3(0.0f, 0.0f, 0.0f),     // Camera target (Z = 0)
			glm::vec3(0.0f, 1.0f, 0.0f)      // Up vector (+Y)
		);

		// Projection Matrix
		glm::mat4 projection = glm::orthoLH(
			0.0f,         // Left
			640.0f,       // Right (game world width)
			0.0f,         // Bottom
			360.0f,       // Top (game world height)
			0.1f,         // Near plane
			100.0f        // Far plane
		);
		graphicsContext->SetViewMatrix(view);
		graphicsContext->SetProjectionMatrix(projection);
		graphicsContext->SetPrimitiveTopology(PrimitiveTopology::TRIANGLE_LIST);

		// Upload all the resources before rendering
	}

	inline auto Update(float deltaTime) -> void {
		uploadContext->Finish();
	}

	inline auto PostFrame() -> void {
		uint32_t lastVao = 0;
		uint32_t lastMaterialId = 0;

		// Turn the draw calls into a vector
		mergedDrawCalls.clear();
		mergedDrawCalls.reserve(drawCalls.size_approx());

		// Turn the draw calls into a vector
		DrawCall call;
		while (drawCalls.try_dequeue(call)) {
			mergedDrawCalls.push_back(call);
		}

		if (!mergedDrawCalls.empty()) {
			// Sort the draw calls by material and then by vertex array
			std::ranges::sort(mergedDrawCalls.begin(), mergedDrawCalls.end(), [](const DrawCall& a, const DrawCall& b) {
				if (a.material == b.material) {
					return a.vao < b.vao;
				}
				return a.material < b.material;
				});

			lastMaterialId = mergedDrawCalls.front().material;
			lastVao = mergedDrawCalls.front().vao;

			graphicsContext->SetVertexArray(vertexArrays[lastVao]);
			graphicsContext->SetIndexBuffer(vertexArrays[lastVao]->IndexBuffer());
			graphicsContext->SetVertexBuffer(0, vertexArrays[lastVao]->VertexBuffer());
			graphicsContext->SetMaterial(materials[lastMaterialId]);

			glm::mat4 model = glm::mat4(1.0f);
			// Process each identical group
			for (const auto& drawCall : mergedDrawCalls) {
				model = drawCall.model;

				if (lastVao != drawCall.vao) {
					// If the vertex array has changed, we need to bind the new vertex array
					graphicsContext->SetVertexArray(vertexArrays[drawCall.vao]);
					graphicsContext->SetIndexBuffer(vertexArrays[drawCall.vao]->IndexBuffer());
					graphicsContext->SetVertexBuffer(0, vertexArrays[drawCall.vao]->VertexBuffer());
					lastVao = drawCall.vao;
				}

				if (lastMaterialId != drawCall.material) {
					// If the material has changed, we need to bind the new material
					graphicsContext->SetMaterial(materials[drawCall.material]);
					lastMaterialId = drawCall.material;
				}

				// Issue the draw call
				graphicsContext->DrawIndexed(model, vertexArrays[drawCall.vao]->Indices(), 0);
			}

			// Process instanced draw calls
			mergedDrawCalls.clear();
		}

		// Turn the draw calls into a vector
		std::vector<InstancedDrawCall> instancedDrawCallsVector;

		DrawCall instancedCall;
		while (instancedDrawCalls.try_dequeue(instancedCall)) {
			auto it = std::ranges::find_if(instancedDrawCallsVector, [&](const auto& draw) {
				return draw.material == instancedCall.material && draw.vao == instancedCall.vao;
				});

			if (it != instancedDrawCallsVector.end()) {
				// If we have a draw call with same material and vertex array, increment the number of instances
				it->models.push_back(instancedCall.model);
			}
			else {
				// If we don't have a draw call with same material and vertex array, add a new draw call with 1 instance
				instancedDrawCallsVector.push_back(
					InstancedDrawCall{ instancedCall.vao, instancedCall.material, {instancedCall.model} }
				);
			}
		}

		uint32_t instances = 0;
		// Process each identical group
		std::vector<glm::mat4> matrices;
		for (const auto& drawCall : instancedDrawCallsVector) {
			auto material = drawCall.material;
			auto modelId = drawCall.vao;

			if(lastVao != modelId) {
				// If the vertex array has changed, we need to bind the new vertex array
				vertexArrays[modelId]->Bind();
				lastVao = modelId;
			}

			if(lastMaterialId != material) {
				// If the material has changed, we need to bind the new material
				graphicsContext->SetMaterial(materials[material]);
				lastMaterialId = material;
			}

			// Bind the vertex array.
			graphicsContext->SetVertexArray(vertexArrays[modelId]);
			graphicsContext->SetIndexBuffer(vertexArrays[modelId]->IndexBuffer());
			graphicsContext->SetVertexBuffer(0, vertexArrays[modelId]->VertexBuffer());

			// Create a vector of matrices
			matrices.reserve(drawCall.models.size());
			for(const auto& model : drawCall.models) {
				matrices.push_back(model);
			}

			// Create an instance buffer
			auto instanceBuffer = device->CreateVertexBuffer(matrices.data(), drawCall.models.size(), sizeof(glm::mat4));
			instanceBuffers.push_back(instanceBuffer);

			// Bind the instance buffer
			graphicsContext->SetVertexBuffer(1, instanceBuffer);

			// Bind the material
			graphicsContext->SetMaterial(materials[material]);
			// Issue the draw call
			graphicsContext->DrawIndexedInstanced(vertexArrays[modelId]->Indices(), drawCall.models.size(), 0, 0);

			matrices.clear();
		}

		// Render the actual frame
		graphicsContext->Finish();
		imguiContext->Finish();

		// Finally, swap the buffers
		swapchain->Swap();

		// Cleanup
		instanceBuffers.clear();
	}

	auto LoadTexture(const uint8_t* data, size_t len) -> uint32_t {
		uint32_t key = 0;

		// If there are no textures, set the key to 0
		if(textures.empty()) {
			key = 0;
		} else if(auto lastTexture = --textures.end(); lastTexture != textures.end()) {
			key = lastTexture->first + 1;
		}

		// Load the texture from memory using FreeImage
		FIMEMORY* memory = FreeImage_OpenMemory((BYTE*)data, len);
		auto bitmap = FreeImage_LoadFromMemory(FIF_PNG, memory);

		// Get width, height, channels and data as RGBA
		uint32_t width = FreeImage_GetWidth(bitmap);
		uint32_t height = FreeImage_GetHeight(bitmap);
		uint32_t pitch = FreeImage_GetPitch(bitmap);
		uint8_t channels = FreeImage_GetLine(bitmap) / width;
		auto bits = FreeImage_GetBits(bitmap);

		// Create pixeldata vev<uint8_t> from bits
		std::vector<uint8_t> pixelData(bits, bits + pitch * height);

		auto texture = device->CreateTexture(width, height, channels, pixelData.data());

		textures[key] = texture;

		return key;
	}

	auto LoadShader(
		std::string code,
		ShaderType type
	) -> uint32_t {
		uint32_t key = 0;
		// If there are no shaders, set the key to 0
		if (shaders.empty()) {
			key = 0;
		}
		else if (auto lastShader = --shaders.end(); lastShader != shaders.end()) {
			key = lastShader->first + 1;
		}

		auto shader = device->CompileShader(code, type);
		shaders[key] = shader;

		return key;
	}

	auto UnloadShader(uint64_t shader) -> void {
		device->DestroyShader(shader);
	}

	auto LoadModel(std::string_view path) -> std::vector<Mesh> {
		return std::vector<Mesh>();
	}

	auto CreateMaterial(uint32_t pixelShader, uint32_t vertexShader, bool isInstanced) -> uint32_t {
		auto material = device->CreateMaterial(
			{
				{
					ShaderType::VERTEX, shaders[vertexShader]
				},
				{
					ShaderType::FRAGMENT, shaders[pixelShader]
				}
			},
			isInstanced
		);

		uint32_t key = 0;

		// If there are no materials, set the key to 0
		if (materials.empty()) {
			key = 0;
		}
		else if (auto lastMaterial = --materials.end(); lastMaterial != materials.end()) {
			key = lastMaterial->first + 1;
		}

		materials[key] = material;

		return key;
	}

	auto CopyMaterial(uint32_t materialId) -> uint32_t {
		auto material = materials[materialId];

		return CreateMaterial(
			material->shaders[ShaderType::FRAGMENT]->id, 
			material->shaders[ShaderType::VERTEX]->id,
			material->isInstanced
		);
	}

	auto CreateVertexBuffer(const void* data, size_t size, size_t elemSize) -> uint32_t {
		auto buffer = device->CreateVertexBuffer(data, size, elemSize);
		auto id = buffer->Id();

		vertexBuffers[id] = buffer;

		return id;
	}

	auto CreateIndexBuffer(const uint32_t* indices, size_t len) -> uint32_t {
		auto buffer = device->CreateIndexBuffer(indices, len);
		auto id = buffer->Id();

		indexBuffers[id] = buffer;

		return id;
	}

	auto UpdateVertexBuffer(uint32_t buffer, const void* data, size_t size) -> void {

	}

	auto UpdateIndexBuffer(uint32_t buffer, const void* data, size_t size) -> void {

	}

	auto CreateVertexArray(uint32_t vertexBufferId, uint32_t indexBufferId) -> uint32_t {
		auto buffer = device->CreateVertexArray(vertexBuffers[vertexBufferId], indexBuffers[indexBufferId]);
		auto id = buffer->Id();

		vertexArrays[id] = buffer;

		return id;
	}

	auto DrawIndexed(glm::mat4 model, uint32_t vao, uint32_t material) -> void {
		drawCalls.enqueue(DrawCall { model, vao, material });
	}

	auto DrawIndexedInstanced(glm::mat4 model, uint32_t vao, uint32_t material) -> void {
		instancedDrawCalls.enqueue(DrawCall { model, vao, material });
	}

	auto DrawSprite(glm::mat4 model, uint32_t material) -> void {
		// Check if the material is instanced
		if (materials[material]->isInstanced) {
			DrawIndexedInstanced(model, spriteVao, material);
			return;
		}
		DrawIndexed(model, spriteVao, material);
	}

	auto SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) -> void {
		// Recalculate the aspect ratio and adjust the viewport accordingly
		
		if (float aspectRatio = (float)width / (float)height; aspectRatio > 16.0f / 9.0f) {
			// If the window is wider than 16:9, adjust the width
			float newWidth = height * (16.0f / 9.0f);
			float xOffset = (width - newWidth) / 2;
			graphicsContext->SetViewport((uint32_t)xOffset, 0, (uint32_t)newWidth, height, 0.0, 1.0);
		}
		else {
			// If the window is taller than 16:9, adjust the height
			float newHeight = width / (16.0f / 9.0f);
			float yOffset = (height - newHeight) / 2;
			graphicsContext->SetViewport(0, (uint32_t)yOffset, width, (uint32_t)newHeight, 0.0, 1.0);
		}
	}

	auto SetMaterialTexture(uint32_t materialId, uint32_t textureId, const char* name) -> void {
		auto material = materials[materialId];
		auto texture = textures[textureId];

		material->textures[name] = texture;
	}
}
