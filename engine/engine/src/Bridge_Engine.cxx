#include "engine/Bridge_Engine.h"
#include "engine/Engine.hxx"
#include "engine/ecs/EntityRegistry.hxx"
#include <assetpackages/AssetPackage.hxx>
#include <assetpackages/AssetPackages.hxx>
#include <assetpackages/IAssetLoader.hxx>
#include <audio/AudioClip.hxx>
#include <rendering/IMeshRenderer.hxx>
#include <rendering/Rendering.hxx>
#include <rendering/Mesh.hxx>
#include <core/Core.hxx>
#include <core/Logger.hxx>
#include <core/ILogger.hxx>

#include <SDL2/SDL.h>

#include <map>
#include <memory>

std::unique_ptr<kyanite::engine::Engine> engine;

// Store all the component types in a map of int -> uint64_t
// The int is the hash value coming from swift and the uint64_t is the component ID
// This is so we can easily get the component ID from the hash value
std::map<int64_t, uint64_t> componentTypes;

void Bridge_Engine_Init(NativePointer window, NativePointer assetLoader, enum Mode mode, NativePointer logger) {
	engine = std::make_unique<kyanite::engine::Engine>();
	if(window != nullptr) {
		engine->window = window;
	} else {
		SDL_CreateWindow("Kyanite", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1280, 720, SDL_WINDOW_OPENGL);
	}
	auto assetLoaderPtr = reinterpret_cast<kyanite::engine::assetpackages::IAssetLoader*>(assetLoader);
	auto loggerPtr = reinterpret_cast<kyanite::engine::core::ILogger*>(logger);

	engine->assetLoader = std::shared_ptr<kyanite::engine::assetpackages::IAssetLoader>(assetLoaderPtr);
	engine->logger = std::shared_ptr<kyanite::engine::core::ILogger>(loggerPtr);
	engine->renderer = std::make_unique<rendering::Renderer>();

	// Initilize subsystems
	kyanite::engine::core::InitCore();
	kyanite::engine::core::logger::SetLogger(engine->logger);
	kyanite::engine::assetpackages::Initialize(engine->assetLoader);
	kyanite::engine::rendering::Init(window);

	auto meshRendererPtr = reinterpret_cast<kyanite::engine::rendering::IMeshRenderer*>(engine->renderer.get());
	ecs::EntityRegistry::Init(meshRendererPtr, mode == Mode::EDITOR);
}

void Bridge_Engine_Update(float delta) {
	ecs::EntityRegistry::Update(delta);
}

uint64_t Bridge_Engine_CreateEntity(const char* name) {
	return ecs::EntityRegistry::CreateEntity(name);
}

void Bridge_Engine_DestroyEntity(uint64_t entity) {
	ecs::EntityRegistry::DestroyEntity(entity);
}

void Bridge_Engine_AddComponent(uint64_t entity, uint64_t component) {
	ecs::EntityRegistry::AddComponent(entity, component);
}

void Bridge_Engine_SetComponent(uint64_t entity, uint64_t component, void* data) {
	ecs::EntityRegistry::SetComponent(entity, component, data);
}

void Bridge_Engine_RemoveComponent(uint64_t entity, uint64_t component) {

}

uint64_t Bridge_Engine_RegisterComponent(const char* name, size_t size, size_t alignment) {
	return ecs::EntityRegistry::CreateComponent(name, size, alignment);
}

uint64_t Bridge_Engine_RegisterSystem(const char* name, uint64_t* filter, size_t filterLen, void (*func)(NativePointer)) {
	std::vector<uint64_t> filterVec(filter, filter + filterLen);
	return ecs::EntityRegistry::RegisterSystem(name, filterVec, reinterpret_cast<void (*)(ecs_iter_t*)>(func));
}

NativePointer Bridge_Engine_GetComponentsFromIterator(NativePointer iterator, uint8_t index, size_t componentSize) {
	auto iter = reinterpret_cast<ecs_iter_t*>(iterator);

	return ecs::EntityRegistry::GetComponentBuffer(iter, index, componentSize);
}

size_t Bridge_Engine_GetIteratorSize(NativePointer iterator) {
	return reinterpret_cast<ecs_iter_t*>(iterator)->count;
}


// --- Asset loading ---
NativePointer Bridge_Engine_LoadAssetPackages(const char* path, size_t* numPackages) {
	auto packages = engine->assetLoader->LoadPackageList(path);
	*numPackages = packages.size();

	kyanite::engine::assetpackages::AssetPackage** packagesPtr = new kyanite::engine::assetpackages::AssetPackage*[*numPackages];

	for(size_t x = 0; x < *numPackages; x++) {
		packagesPtr[x] = packages[x];
	}

	return reinterpret_cast<NativePointer>(packagesPtr);
}

NativePointer Bridge_Engine_LoadTexture(NativePointer assetPackage, const char* uuid) {
	auto package = reinterpret_cast<kyanite::engine::assetpackages::AssetPackage*>(assetPackage);
	auto buffer = engine->assetLoader->LoadAsset(package, uuid);
	// Convert the buffer to a mesh
	std::stringstream ss;
	ss.write(reinterpret_cast<const char*>(buffer.data()), buffer.size());
	kyanite::engine::rendering::Texture texture;
	cereal::BinaryInputArchive archive(ss);
	archive(texture);

	kyanite::engine::rendering::Texture* texturePtr = new kyanite::engine::rendering::Texture;
	*texturePtr = texture;

	return reinterpret_cast<NativePointer>(texturePtr);
}

NativePointer Bridge_Engine_LoadMesh(NativePointer assetPackage, const char* uuid) {
	auto package = reinterpret_cast<kyanite::engine::assetpackages::AssetPackage*>(assetPackage);
	auto buffer = engine->assetLoader->LoadAsset(package, uuid);
	// Convert the buffer to a mesh
	std::stringstream ss;
	ss.write(reinterpret_cast<const char*>(buffer.data()), buffer.size());
	kyanite::engine::rendering::Mesh mesh;
	cereal::BinaryInputArchive archive(ss);
	archive(mesh);

	kyanite::engine::rendering::Mesh* meshPtr = new kyanite::engine::rendering::Mesh;
	*meshPtr = mesh;

	return reinterpret_cast<NativePointer>(meshPtr);
}

NativePointer Bridge_Engine_LoadAudioClip(NativePointer assetPackage, const char* uuid) {
	auto package = reinterpret_cast<kyanite::engine::assetpackages::AssetPackage*>(assetPackage);
	auto buffer = engine->assetLoader->LoadAsset(package, uuid);
	// Convert the buffer to a mesh
	std::stringstream ss;
	ss.write(reinterpret_cast<const char*>(buffer.data()), buffer.size());
	kyanite::engine::audio::AudioClip clip;
	cereal::BinaryInputArchive archive(ss);
	archive(clip);

	kyanite::engine::audio::AudioClip* clipPtr = new kyanite::engine::audio::AudioClip;
	*clipPtr = clip;

	return reinterpret_cast<NativePointer>(clipPtr);
}

NativePointer Bridge_Engine_LoadMaterial(NativePointer assetPackage, const char* uuid) {
	return nullptr;
}

NativePointer Bridge_Engine_LoadShader(NativePointer assetPackage, const char* uuid) {
	return nullptr;
}

void Bridge_Engine_DisposeAsset(NativePointer asset) {
	delete asset;
}