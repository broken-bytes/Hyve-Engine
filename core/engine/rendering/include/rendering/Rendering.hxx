#pragma once

#include "Mesh.hxx"
#include "Renderer.hxx"
#include "Shader.hxx"
#include "Texture.hxx"
#include <shared/Exported.hxx>
#include <shared/NativePointer.hxx>

#include <imgui.h>

#include <string>
#include <vector>

namespace kyanite::engine::rendering {
	// Lifecycle
	auto Init(NativePointer window, ImGuiContext* imGuiContext) -> void;
	auto Shutdown() -> void;
	extern auto PreFrame() -> void;
	extern auto Update(float deltaTime) -> void;
	extern auto PostFrame() -> void;
	
	// Resource creation
	auto LoadTexture(const uint8_t* data, size_t len) -> uint32_t;
	auto LoadShader(std::string code, ShaderType type) -> uint32_t;
	auto LoadModel(std::string_view path) -> std::vector<Mesh>;
	auto CreateMaterial(uint32_t pixelShader, uint32_t vertexShader, bool isInstanced) -> uint32_t;
	auto CopyMaterial(uint32_t materialId) -> uint32_t;
	auto CreateVertexBuffer(const void* data, size_t size, size_t elemSize) -> uint32_t;
	auto CreateIndexBuffer(const uint32_t* indices, size_t len) -> uint32_t;
	auto UpdateVertexBuffer(uint32_t buffer, const void* data, size_t size) -> void;
	auto UpdateIndexBuffer(uint32_t buffer, const void* data, size_t size) -> void;
	auto CreateVertexArray(uint32_t vertexBufferId, uint32_t indexBufferId) -> uint32_t;

	// Resource destruction
	auto UnloadShader(uint64_t shader) -> void;

	// State management
	auto SetClearColor(float r, float g, float b, float a) -> void;
	auto SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) -> void;
	extern auto DrawIndexed(
		glm::mat4 model, 
		uint32_t vao,
		uint32_t material
	) -> void;
	extern auto DrawSprite(glm::mat4 model, uint32_t material) -> void;

	auto SetMaterialTexture(uint32_t materialId, uint32_t textureId, const char* name) -> void;
}