#pragma once

#include <shared/Exported.hxx>
#include <shared/NativePointer.hxx>

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus 
extern "C" {
#endif

/**
 * @brief Inits the rendering system
 *
 */
EXPORTED void Rendering_Init(NativePointer window, NativePointer imGuiContext);

/**
 * @brief Shuts down the rendering system
 *
 */
EXPORTED void Rendering_Shutdown();

/**
 * @brief Sets the clear color of the rendering system
 * @param r The red component of the color
 * @param g The green component of the color
 * @param b The blue component of the color
 * @param a The alpha component of the color
 *
 */
EXPORTED void Rendering_SetClearColor(float r, float g, float b, float a);

/**
* @brief Sets the viewport of the rendering system
* @param x The x coordinate of the viewport
* @param y The y coordinate of the viewport
* @param width The width of the viewport
* @param height The height of the viewport
*/
EXPORTED void Rendering_SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height);

/**
 * @brief Draws a mesh
 * @param shaderId The id of the shader to use
 * @param vertexBuffer The vertex buffer to use
 * @param indexBuffer The index buffer to use
 * @param vertexCount The number of vertices to draw
 * @param indexCount The number of indices to draw
 */
EXPORTED extern void Rendering_DrawMesh(
	const float* transform,
	uint32_t materialId,
	uint32_t vaoId
);

EXPORTED extern void Rendering_DrawSprite(
	const float* transform,
	uint32_t materialId
);

/**
    @brief Starts the rendering frame
*/
EXPORTED extern void Rendering_PreFrame();

/**
 * @brief Updates the rendering system
 * @param deltaTime The time since the last frame
 *
 */
EXPORTED extern void Rendering_Update(float deltaTime);

/**
* @brief Render the frame
*/
EXPORTED extern void Rendering_PostFrame();

// Graphics pipeline functions

/**
* @brief Creates a vertex buffer
* @param vertices The vertices to use
* @param length The length of the buffer in count of vertices
* @return The id of the vertex buffer
*/
EXPORTED uint32_t Rendering_CreateVertexBuffer(const float* vertices, size_t length, size_t elemSize);

/**
* @brief Updates a vertex buffer
* @param vertexBufferId The id of the vertex buffer to update
* @param vertices The new vertices to use
*/
EXPORTED void Rendering_UpdateVertexBuffer(uint32_t vertexBufferId, NativePointer vertices, size_t length);

/**
* @brief Frees a vertex buffer
* @param vertexBufferId The id of the vertex buffer to free
*/
EXPORTED void Rendering_FreeVertexBuffer(uint32_t vertexBufferId);

/**
* @brief Loads the vertex buffer into cpu memory
* @param vertexBufferId The id of the vertex buffer to load
* @param vertices The pointer to the cpu memory
* @param length The length of the buffer in count of vertices
*/
EXPORTED void Rendering_LoadVertexBuffer(uint32_t vertexBufferId, NativePointer* vertices, size_t* length);

/**
* @brief Creates an index buffer
* @param indices The indices to use
* @param length The length of the buffer in count of indices
* @return The id of the index buffer
*/
EXPORTED uint32_t Rendering_CreateIndexBuffer(const uint32_t* indices, size_t length);

/**
* @brief Updates an index buffer
* @param indexBufferId The id of the index buffer to update
* @param indices The new indices to use
*/
EXPORTED void Rendering_UpdateIndexBuffer(uint32_t indexBufferId, NativePointer indices);

/**
* @brief Frees an index buffer
* @param indexBufferId The id of the index buffer to free
*/
EXPORTED void Rendering_FreeIndexBuffer(uint32_t indexBufferId);

/**
* @brief Creates a vertex array(VAO)
* @param vertexBufferId The id of the vertex buffer to use
* @param indexBufferId The id of the index buffer to use
* @return The id of the vertex array
*/
EXPORTED uint32_t Rendering_CreateVertexArray(uint32_t vertexBufferId, uint32_t indexBufferId);

/**
* @brief Creates a shader
* @param shader The shader code to use
* @param shaderType The type of the shader. 0 for vertex, 1 for fragment
* @return The id of the shader
*/
EXPORTED uint32_t Rendering_CreateShader(const char* shader, uint8_t shaderType);

/**
* @brief Destroys a shader
* @param shaderId The id of the shader to destroy
*/
EXPORTED void Rendering_DestroyShader(uint32_t shaderId);

/**
 * @brief Creates a texture
 * @param buffer The buffer to use
 * @param size The size of the buffer
 * @return The id of the texture
 */
EXPORTED uint32_t Rendering_LoadTexture(const uint8_t* data, size_t len);

/**
* @brief Creates a material
* @param shaderId The id of the shader to use
* @return The id of the material
*/
EXPORTED uint32_t Rendering_CreateMaterial(uint32_t pixelShader, uint32_t vertexShader, bool isInstanced);

/**
* @brief Copies a material, creating a new one with the same properties
* @param materialId The id of the material to copy
* @return The id of the new material
*/
EXPORTED uint32_t Rendering_CopyMaterial(uint32_t materialId);

/**
* @brief Sets the material texture
* @param materialId The id of the material
* @param textureId The id of the texture
* @param name The name of the texture in the shader
*/
EXPORTED void Rendering_SetMaterialTexture(uint32_t materialId, uint32_t textureId, const char* name);

#ifdef __cplusplus 
}
#endif
