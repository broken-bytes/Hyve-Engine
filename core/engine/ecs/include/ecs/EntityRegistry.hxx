#pragma once 

#define FLECS_CORE
#define FLECS_SYSTEM
#define FLECS_PIPELINE
#define FLECS_META
#define FLECS_META_C
#define FLECS_PIPELINE
#define FLECS_HTTP
#define FLECS_TIMER
#define FLECS_REST
#define FLECS_STATS
#define FLECS_MONITOR
#define FLECS_CPP
#include <flecs.h>

#include <stdint.h>
#include <string>
#include <vector>

namespace ecs::EntityRegistry {
	/**
	* @brief Initializes the entity registry
	*/
	auto extern Init(bool debugServer) -> void;

	/**
	* @brief Gets the entity registry
	* @return The entity registry
	*/
	auto extern GetRegistry() -> ecs_world_t*;

	/**
	* @brief Creates an entity
	* @return The created entity
	*/
	auto extern CreateEntity(std::string name) -> ecs_entity_t;

	/**
	* @brief Sets the parent of an entity
	* @param entity The entity to set the parent of
	* @param parent The parent to set
	*/
	auto extern SetParent(ecs_entity_t entity, ecs_entity_t parent) -> void;

	/**
	* @brief Destroys an entity
	* @param entity The entity to destroy
	*/
	auto extern DestroyEntity(ecs_entity_t entity) -> void;

	/**
	* @brief Creates a component
	* @param size The size of the component
	* @param alignment The alignment of the component
	* @return The created component
	*/
	auto extern CreateComponent(
		const uint64_t uuid,
		std::string name, 
		size_t size, 
		size_t alignment
	) -> ecs_entity_t;

	/**
	* @brief Adds a component to an entity
	* @param entity The entity to add the component to
	* @param type The uuid of the component to add
	*/
	auto extern AddComponent(ecs_entity_t entity, uint64_t component) -> void;

	/**
	* @brief Sets a component on an entity
	* @param entity The entity to set the component on
	* @param component The component uuid to set
	* @param data The data to set the component to
	*/
	auto extern SetComponent(ecs_entity_t entity, uint64_t component, const void* data) -> void;

	/**
	* @brief Removes a component via uuid from an entity
	* @param entity The entity to remove the component from
	*/
	auto extern RemoveComponent(ecs_entity_t entity, uint64_t component) -> void;

	/**
	* @brief Gets a component from an entity
	* @param entity The entity to get the component from
	* @param component The component uuid
	* @return The component
	*/
	auto extern GetComponent(ecs_entity_t entity, uint64_t component) -> const void*;

	/**
	* @brief Checks if an entity has a component
	* @param entity The entity to check
	* @param component The component uuid to check
	* @return True if the entity has the component, false otherwise
	*/
	auto extern HasComponent(ecs_entity_t entity, uint64_t component) -> bool;

	/**
	* @brief Gets all the components of an entity
	* @param entity The entity to get the components of
	* @param An iterator that is called for each component
	* @note This function is slow and should be used sparingly
	*/
	auto extern GetEntityComponents(
		ecs_entity_t entity, 
		uint64_t index,
		uint64_t* typeId,
		const void** data
	) -> void;

	/**
	* @brief Advances the ecs world by delta time
	* @param delta The delta time
	*/
	auto extern Update(float delta) -> void;

	/**
	* @brief Registers a system
	* @param func The function to register
	*/
	auto extern RegisterSystem(
		std::string name, 
		std::vector<ecs_entity_t> filter, 
		bool isParallel,
		void (*func)(ecs_iter_t* it)
	) -> ecs_entity_t;

	/**
	* @brief Gets all the components of an entity for a given index
	* @param iter The iterator
	* @param componentSize The size of the component
	* @param index The index of the component
	* @return The components
	*/
	auto extern GetComponentBuffer(ecs_iter_t* iter, uint32_t index, size_t componentSize) -> void*;

	/**
	* @brief Gets an entity from an iterator
	* @param iter The iterator
	* @param index The index of the entity
	* @return The entities
	*/
	auto extern GetEntitiesFromIterator(ecs_iter_t* iter) -> const ecs_entity_t*;

	/**
	* @brief Gets the entity by name
	* @param name The name of the entity
	* @return The entity
	*/
	auto extern GetEntityByName(std::string_view name) -> uint64_t;

	/**
	* @brief Gets the name of an entity
	* @param entity The entity to get the name of
	* @return The name
	*/
	auto extern GetEntityName(ecs_entity_t entity) -> const char*;

	/**
	* @brief Gets the parent of an entity
	* @param entity The entity to get the parent of
	* @return The parent
	* @note Returns null if the entity has no parent
	*/
	auto extern GetParent(ecs_entity_t entity) -> ecs_entity_t;

	/**
	* @brief Gets the name of an entity
	* @param entity The entity to get the childs of
	* @param callback The callback to call for each child
	*/
	auto extern ForEachChild(ecs_entity_t parent, void (*callback)(const char* name, ecs_entity_t id)) -> void;

	/**
	* @brief Gets a component id from a name
	* @param name The name of the component
	* @return The component id
	*/
	auto extern GetComponentId(const char* name)->uint64_t;

	/**
	* @brief Gets a component uuid from an id
	* @param component The component id
	* @return The component uuid
	*/
	auto extern GetComponentUuid(uint64_t component)->uint64_t;
}