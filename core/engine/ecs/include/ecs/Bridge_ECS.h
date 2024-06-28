#pragma once

#include <shared/Exported.hxx>
#include <shared/NativePointer.hxx>

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus 
extern "C" {
#endif
	/**
	 * @brief Initializes the ECS
	 */
	EXPORTED extern void ECS_Init(NativePointer logger, bool isDebug);

	/**
	* @brief Updates the engine
	* @param delta The time since the last frame
	*/
	EXPORTED extern void ECS_Update(float delta);

	/**
	 * @brief Creates a new entity
	 *
	 * @return The new entity
	 */
	EXPORTED extern uint64_t ECS_CreateEntity(const char* name);

	/**
	* @brief Sets the parent of an entity
	* @param entity The entity to set the parent of
	* @param parent The parent to set
	*/
	EXPORTED extern void ECS_SetParent(uint64_t entity, uint64_t parent);

	/**
	 * @brief Destroys an entity
	 *
	 * @param entity The entity to destroy
	 */
	EXPORTED extern void ECS_DestroyEntity(uint64_t entity);

	/**
	* @brief Adds a component to an entity
	* @param entity The entity to add the component to
	* @param component The component uuid
	*/
	EXPORTED extern void ECS_AddComponent(uint64_t entity, uint64_t component);

	/**
	* @brief Sets a component of an entity
	* @param entity The entity to use
	* @param component The component to set
	* @param data The component uuid to set
	*/
	EXPORTED extern void ECS_SetComponent(uint64_t entity, uint64_t component, const void* data);

	/**
	* @brief Removes a component from an entity
	* @param entity The entity to remove the component from
	* @param component The component uuid to remove
	*/
	EXPORTED extern void ECS_RemoveComponent(uint64_t entity, uint64_t component);

	/**
	* @brief Gets a component from an entity
	* @param entity The entity to get the component from
	* @param component The component uuid to get
	* @return The component
	*/
	EXPORTED extern NativePointer ECS_GetComponent(uint64_t entity, uint64_t component);

	/**
	* @brief Checks if an entity has a component
	* @param entity The entity to check
	* @param component The component to check
	* @return If the entity has the component
	*/
	EXPORTED extern bool ECS_HasComponent(uint64_t entity, uint64_t component);

	/**
	* @brief Gets a component from an entity
	* @param entity The entity to get the component from
	* @param iterator An iterator that is called for each component
	* @note Caller is responsible for freeing the memory
	*/
	EXPORTED extern void ECS_GetAllComponents(
		uint64_t entity, 
		uint64_t index,
		uint64_t* typeId,
		const void** data
	);

	/**
	* @brief Registers a component type
	* @param uuid The uuid of the component type on the script side
	* @param size The size of the component type
	* @param alignment The alignment of the component type
	* @return The component type
	*/
	EXPORTED extern uint64_t ECS_RegisterComponent(
		const uint64_t uuid,
		const char* name, 
		size_t size, 
		size_t alignment
	);

	/**
	* @brief Registers a system
	* @param funcPtr The function pointer to the system. Takes the components and the number of components as arguments
	*/
	EXPORTED extern uint64_t ECS_RegisterSystem(
		const char* name, 
		const uint64_t* filter, 
		size_t filterLen, 
		bool isParallel,
		void (*func)(NativePointer)
	);

	/**
	* @brief Gets components for an iterator
	* @param iterator The iterator to get the components from
	* @param index The index of the component
	* @param componentSize The size of the component
	* @return The components
	*/
	EXPORTED extern NativePointer ECS_GetComponentsFromIterator(NativePointer iterator, uint32_t index, size_t componentSize);

	/**
	* @brief Gets entities for an iterator
	* @param iterator The iterator to get the entities from
	* @return The entities
	*/
	EXPORTED extern const uint64_t* ECS_GetEntitiesFromIterator(NativePointer iterator);

	/**
	* @brief Gets the number of components for an iterator
	* @param iterator The iterator to get the components from
	* @return The number of components
	*/
	EXPORTED extern size_t ECS_GetIteratorSize(NativePointer iterator);

	/**
	* Get entity by name
	* @param name The name of the entity
	* @param entity The entity if found, otherwise null
	*/
	EXPORTED extern uint64_t ECS_GetEntityByName(const char* name);

	/**
	* @brief Gets the name of an entity
	* @param entity The entity to get the name of
	* @return The name of the entity
	*/
	EXPORTED extern const char* ECS_GetEntityName(uint64_t entity);

	/**
	* @brief Gets the parent of an entity
	* @param entity The entity to get the parent of
	* @return The parent of the entity or 0 if it has no parent
	*/
	EXPORTED extern uint64_t ECS_GetParent(uint64_t entity);

	/**
	* @brief Gets the name of an entity
	* @param entity The entity to get the childs of
	* @param callback The callback to call for each child
	*/
	EXPORTED extern void ECS_ForEachChild(uint64_t parent, void (*callback)(const char* name, uint64_t id));

	/**
	* @brief Gets the id of a component
	* @param name The name of the component
	* @return The id of the component
	*/
	EXPORTED extern uint64_t ECS_GetComponentId(const char* name);

	/**
	* @brief Gets the uuid of a component
	* @param component The component id
	* @return The uuid of the component
	*/
	EXPORTED extern uint64_t ECS_GetComponentUuid(uint64_t component);
#ifdef __cplusplus 
}
#endif