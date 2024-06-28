#include "rendering/Bridge_Rendering.h"
#include "rendering/Rendering.hxx"
#include "rendering/Shader.hxx"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>
#include <imgui_internal.h>

namespace rendering = kyanite::engine::rendering;

void Rendering_Init(NativePointer window, NativePointer imGuiContext) {
	rendering::Init(window, reinterpret_cast<ImGuiContext*>(imGuiContext));
}

void Rendering_Shutdown() {
	rendering::Shutdown();
}

inline void Rendering_PreFrame() {
	rendering::PreFrame();
}

inline void Rendering_Update(float deltaTime) {
	rendering::Update(deltaTime);
}

inline void Rendering_PostFrame() {
	rendering::PostFrame();
}


inline void Rendering_DrawMesh(
	const float* transform,
	uint32_t materialId,
	uint32_t vaoId
) {
	// Create the glm ::mat4 from the float array
	glm::mat4 transformMatrix = glm::make_mat4(transform);

	rendering::DrawIndexed(transformMatrix, vaoId, materialId);
}

inline void Rendering_DrawSprite(
	const float* transform,
	uint32_t materialId
) {
	// Create the glm ::mat4 from the float array
	glm::mat4 transformMatrix = glm::make_mat4(transform);

	rendering::DrawSprite(transformMatrix, materialId);
}

void Rendering_SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) {
	rendering::SetViewport(x, y, width, height);
}

// Graphics pipeline functions
uint32_t Rendering_CreateVertexBuffer(const float* vertices, size_t length, size_t elemSize) {
	return rendering::CreateVertexBuffer(vertices, length, elemSize);
}

void Rendering_UpdateVertexBuffer(uint32_t vertexBufferId, NativePointer vertices, size_t length) {

}

void Rendering_FreeVertexBuffer(uint32_t vertexBufferId) {

}

void Rendering_LoadVertexBuffer(uint32_t vertexBufferId, NativePointer* vertices, size_t* length) {

}

uint32_t Rendering_CreateIndexBuffer(const uint32_t* indices, size_t length) {
	return rendering::CreateIndexBuffer(indices, length);
}

void Rendering_UpdateIndexBuffer(uint32_t indexBufferId, NativePointer indices) {
	// TODO: Implement
	return;
}

void Rendering_FreeIndexBuffer(uint32_t indexBufferId);

uint32_t Rendering_CreateVertexArray(uint32_t vertexBufferId, uint32_t indexBufferId) {
	return rendering::CreateVertexArray(vertexBufferId, indexBufferId);
}

uint32_t Rendering_CreateShader(const char* shader, uint8_t shaderType) {
	return rendering::LoadShader(shader, rendering::ShaderType(shaderType));
}

void Rendering_DestroyShader(uint32_t shaderId) {
	rendering::UnloadShader(shaderId);
}

uint32_t Rendering_LoadTexture(const uint8_t* data, size_t len) {
	return rendering::LoadTexture(data, len);
}

uint32_t Rendering_CreateMaterial(uint32_t pixelShader, uint32_t vertexShader, bool isInstanced) {
	return rendering::CreateMaterial(pixelShader, vertexShader, isInstanced);
}

uint32_t Rendering_CopyMaterial(uint32_t materialId) {
	return rendering::CopyMaterial(materialId);
}

void Rendering_SetMaterialTexture(uint32_t materialId, uint32_t textureId, const char* name) {
	rendering::SetMaterialTexture(materialId, textureId, name);
}