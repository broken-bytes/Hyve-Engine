#include "ecs/Bridge_ECS.h"
#include "ecs/EntityRegistry.hxx"

#include <map>
#include <memory>
#include <stdexcept>

// Store all the component types in a map of int -> uint64_t
// The int is the hash value coming from swift and the uint64_t is the component ID
// This is so we can easily get the component ID from the hash value
std::map<int64_t, uint64_t> componentTypes;

inline void ECS_Init(NativePointer logger, bool isDebug) {
	ecs::EntityRegistry::Init(isDebug);
}

inline void ECS_Update(float delta) {
	ecs::EntityRegistry::Update(delta);
}

inline uint64_t ECS_CreateEntity(const char* name) {
	return ecs::EntityRegistry::CreateEntity(name);
}

inline void ECS_SetParent(uint64_t entity, uint64_t parent) {
	ecs::EntityRegistry::SetParent(entity, parent);
}

inline void ECS_DestroyEntity(uint64_t entity) {
	ecs::EntityRegistry::DestroyEntity(entity);
}

inline void ECS_AddComponent(uint64_t entity, uint64_t component) {
	ecs::EntityRegistry::AddComponent(entity, component);
}

inline void ECS_SetComponent(uint64_t entity, uint64_t component, const void* data) {
	ecs::EntityRegistry::SetComponent(entity, component, data);
}

inline void ECS_RemoveComponent(uint64_t entity, uint64_t component) {

}

inline NativePointer ECS_GetComponent(uint64_t entity, uint64_t component) {
	auto data = ecs::EntityRegistry::GetComponent(entity, component);
	const auto native = reinterpret_cast<const NativePointer*>(data);

	return const_cast<NativePointer*>(native);
}

inline bool ECS_HasComponent(uint64_t entity, uint64_t component) {
	return ecs::EntityRegistry::HasComponent(entity, component);
}


inline void ECS_GetAllComponents(
	uint64_t entity, 
	uint64_t index,
	uint64_t* typeId,
	const void** data
) {
	ecs::EntityRegistry::GetEntityComponents(entity, index, typeId, data);
}

inline uint64_t ECS_RegisterComponent(const uint64_t uuid, const char* name, size_t size, size_t alignment) {
	return ecs::EntityRegistry::CreateComponent(uuid, name, size, alignment);
}

inline uint64_t ECS_RegisterSystem(
	const char* name, 
	const uint64_t* filter, 
	size_t filterLen, 
	bool isParallel,
	void (*func)(NativePointer)
) {
	std::vector<uint64_t> filterVec(filter, filter + filterLen);
	return ecs::EntityRegistry::RegisterSystem(name, filterVec, isParallel, reinterpret_cast<void (*)(ecs_iter_t*)>(func));
}

inline NativePointer ECS_GetComponentsFromIterator(NativePointer iterator, uint32_t index, size_t componentSize) {
	auto iter = reinterpret_cast<ecs_iter_t*>(iterator);

	return ecs::EntityRegistry::GetComponentBuffer(iter, index, componentSize);
}

inline const uint64_t* ECS_GetEntitiesFromIterator(NativePointer iterator) {
	auto iter = reinterpret_cast<ecs_iter_t*>(iterator);

	return ecs::EntityRegistry::GetEntitiesFromIterator(iter);
}

inline size_t ECS_GetIteratorSize(NativePointer iterator) {
	return reinterpret_cast<ecs_iter_t*>(iterator)->count;
}

inline uint64_t ECS_GetEntityByName(const char* name) {
	return ecs::EntityRegistry::GetEntityByName(name);
}

inline const char* ECS_GetEntityName(uint64_t entity) {
	return ecs::EntityRegistry::GetEntityName(entity);
}

inline uint64_t ECS_GetParent(uint64_t entity) {
	return ecs::EntityRegistry::GetParent(entity);
}

inline void ECS_ForEachChild(uint64_t parent, void (*callback)(const char* name, uint64_t id)) {
	ecs::EntityRegistry::ForEachChild(parent, callback);
}

inline uint64_t ECS_GetComponentId(const char* name) {
	return ecs::EntityRegistry::GetComponentId(name);
}

inline uint64_t ECS_GetComponentUuid(uint64_t component) {
	return ecs::EntityRegistry::GetComponentUuid(component);
}

