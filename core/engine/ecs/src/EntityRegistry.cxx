#include "ecs/EntityRegistry.hxx"
#include <io/IO.hxx>

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
#include <flecs/addons/rest.h>

#include <map>
#include <memory>
#include <shared_mutex>
#include <cstdint>
#include <mutex>
#include <thread>
#include <vector>

namespace ecs::EntityRegistry {
	flecs::world world;
	std::mutex writeLock;
	std::shared_mutex readLock;
	/// Maps from script UUID to component ID
	std::map<uint64_t, uint64_t> componentMappings;
	
	inline auto Init(bool debugServer) -> void {
		// If we are in debug mode, we want to start the debug server and load all component metadata
		if (debugServer) {
			world.set<flecs::Rest>({ });
			// Get the number of threads on the system
			auto threads = std::thread::hardware_concurrency();
			world.set_threads(std::max(2, (int)threads - 2));
			world.import<flecs::monitor>();
			// Check if the meta directory exists
			if (!std::filesystem::exists("meta")) {
				std::filesystem::create_directory("meta");
			}
			auto metafiles = kyanite::engine::io::GetFileList(std::filesystem::current_path() / "meta");
			for (auto& file : metafiles) {
				auto data = kyanite::engine::io::LoadFileToBuffer(file.path().string());
				std::string value(data.begin(), data.end());
				world.plecs_from_str((file.path().stem()).string().c_str(), value.c_str());
			}
		}
	}

	inline auto GetRegistry() -> ecs_world_t* {
		std::shared_lock<std::shared_mutex> lock { readLock };
		return world;
	}

	inline auto CreateEntity(std::string name) -> ecs_entity_t {
		std::scoped_lock lock { writeLock };
		
		return world.entity(name.c_str()).id();
	}

	inline auto SetParent(ecs_entity_t entity, ecs_entity_t parent) -> void {
		std::scoped_lock lock { writeLock };
		// Remove the current parent
		world.entity(entity).remove(flecs::ChildOf);
		// Make entity a child of the new parent
		world.entity(entity).add(flecs::ChildOf, parent);
		// Now make parent the parent of entity
		world.entity(parent).add(flecs::Parent, entity);
	}

	inline auto DestroyEntity(ecs_entity_t entity) -> void {
		std::scoped_lock lock{ writeLock };
		world.entity(entity).destruct();
	}

	inline auto CreateComponent(
		const uint64_t uuid, 
		std::string name, 
		size_t size, 
		size_t alignment
	) -> ecs_entity_t {
		auto desc = ecs_component_desc_t {};
		desc.type = {};
		desc.type.size = size;
		desc.type.alignment = alignment;
		desc.type.name = name.c_str();

		ecs_entity_desc_t entityDesc = {};
		entityDesc.name = name.c_str();

		std::scoped_lock lock { writeLock };

		desc.entity = ecs_entity_init(world, &entityDesc);
		
		auto id = ecs_component_init(world, &desc);

		componentMappings[uuid] = id;

		return id;
	}

	inline auto AddComponent(ecs_entity_t entity, uint64_t component) -> void {
		std::scoped_lock lock { writeLock };
		auto componentId = componentMappings[component];

		ecs_add_id(world, entity, componentId);
	}

	inline auto SetComponent(ecs_entity_t entity, uint64_t component, const void* data) -> void {
		std::scoped_lock lock { writeLock };
		auto componentId = componentMappings[component];
		world.entity(entity).set_ptr(componentId, data);
	}

	inline auto RemoveComponent(ecs_entity_t entity, uint64_t component) -> void {
		std::scoped_lock lock { writeLock };
		auto componentId = componentMappings[component];
		ecs_remove_id(world, entity, componentId);
	}

	inline auto GetComponent(ecs_entity_t entity, uint64_t component) -> const void* {
		std::shared_lock<std::shared_mutex> lock{ readLock };
		// Get the component ID from the UUID
		auto componentId = componentMappings[component];
		return world.entity(entity).get(componentId);
	}

	inline auto HasComponent(ecs_entity_t entity, uint64_t component) -> bool {
		std::shared_lock<std::shared_mutex> lock { readLock };
		auto componentId = componentMappings[component];
		return world.entity(entity).has(componentId);
	}

	inline auto GetEntityComponents(
		ecs_entity_t entity, 
		uint64_t index,
		uint64_t* typeId,
		const void** data
	) -> void {
		std::shared_lock<std::shared_mutex> lock { readLock };

		auto entityRef = world.entity(entity);
		auto count = entityRef.type().count();

		if(index >= count) {
			*typeId = 0;
			*data = nullptr;
			return;
		}

		auto componentId = entityRef.type().array()[index];
		auto dataPtr = entityRef.get(componentId);

		*typeId = componentId;
		*data = dataPtr;
	}

	inline auto Update(float delta) -> void {
		std::scoped_lock lock { writeLock };
		world.progress();
	}

	inline auto RegisterSystem(
		std::string name, 
		std::vector<ecs_entity_t> filter, 
		bool isParallel,
		void (*func)(ecs_iter_t* it)
	) -> ecs_entity_t {
		ecs_system_desc_t desc = {};
		ecs_entity_desc_t entityDesc = {};
		entityDesc.name = name.c_str();

		std::scoped_lock lock { writeLock };
		entityDesc.add[0] = ecs_pair(EcsDependsOn, EcsOnUpdate);
		ecs_entity_t system = ecs_entity_init(world, &entityDesc);
		desc.entity = system;
		desc.callback = func;
		desc.multi_threaded = isParallel;
		for (int x = 0; x < filter.size(); x++) {
			// Get the component ID from the UUID
			auto componentId = componentMappings[filter[x]];
			desc.query.filter.terms[x].id = componentId;
			desc.query.filter.terms[x].oper = EcsAnd;
		}

		auto sysId = ecs_system_init(world, &desc);

		return sysId;
	}

	inline auto GetComponentBuffer(ecs_iter_t* iter, uint32_t index, size_t componentSize) -> void* {
		auto ptr = ecs_field_w_size(iter, componentSize, index);

		return ptr;
	}

	inline auto GetEntitiesFromIterator(ecs_iter_t* iter) -> const ecs_entity_t* {
		return iter->entities;
	}

	inline auto GetEntityByName(std::string_view name) -> uint64_t {
		std::shared_lock<std::shared_mutex> lock { readLock };
		auto entity = world.lookup(name.data());
		if (entity) {
			return entity.id();
		}

		return 0;
	}

	inline auto GetEntityName(ecs_entity_t entity) -> const char* {
		std::shared_lock<std::shared_mutex> lock { readLock };

		return world.entity(entity).name();
	}

	inline auto GetParent(ecs_entity_t entity) -> ecs_entity_t {
		std::shared_lock<std::shared_mutex> lock { readLock };
		
		return world.entity(entity).parent();
	}

	inline auto ForEachChild(ecs_entity_t parent, void (*callback)(const char* name, ecs_entity_t id)) -> void {
		std::shared_lock<std::shared_mutex> lock { readLock };

		world.entity(parent).children([&callback](flecs::entity child) {
			callback(child.name(), child.id());
		});

	}

	inline auto GetComponentId(const char* name) -> uint64_t {
		return world.lookup(name).id();
	}

	inline auto GetComponentUuid(uint64_t component) -> uint64_t {
		for (auto& [key, value] : componentMappings) {
			if (value == component) {
				return key;
			}
		}

		return 0;
	}
}