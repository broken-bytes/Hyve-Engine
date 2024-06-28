@_implementationOnly import Bridge

public enum ECS {
    public static func createEntity(name: String) -> UInt64 {
        ECS_CreateEntity(name.cString(using: .utf8))
    }

    public static func destroyEntity(entity: UInt64) {
        ECS_DestroyEntity(entity)
    }

    public static func name(of entity: UInt64) -> String? {
        String(cString: ECS_GetEntityName(entity), encoding: .utf8)
    }

    public static func entity(by name: String) -> UInt64? {
        ECS_GetEntityByName(name.cString(using: .utf8))
    }

    public static func parent(of entity: UInt64) -> UInt64? {
        ECS_GetParent(entity)
    }

    public static func setParent(entity: UInt64, parent: UInt64) {
        ECS_SetParent(entity, parent)
    }

    public static func registerComponent<T>(type: T.Type, name: String) -> UInt64 {
        ECS_RegisterComponent(
            id(for: T.self),
            name.cString(using: .utf8),
            MemoryLayout<T>.size,
            MemoryLayout<T>.alignment
        )
    }

    public static func addComponent<T>(entity: UInt64, component: T.Type) {
        ECS_AddComponent(entity, id(for: T.self))
    }

    public static func setComponent<T>(entity: UInt64, component: T.Type, data: UnsafeRawPointer) {
        ECS_SetComponent(entity, id(for: T.self), data)
    }

    public static func removeComponent<T>(entity: UInt64, component: T.Type) {
        ECS_RemoveComponent(entity, id(for: T.self))
    }

    public static func hasComponent<T>(entity: UInt64, component: T.Type) -> Bool {
        ECS_HasComponent(entity, id(for: T.self))
    }

    public static func getComponent<T>(entity: UInt64, component: T.Type) -> UnsafeMutablePointer<T>? {
        ECS_GetComponent(entity, id(for: T.self)).assumingMemoryBound(to: T.self)
    }

    public static func registerSystem(name: String, components: [UInt64], isParallel: Bool, block: (@convention(c) (UnsafeMutableRawPointer?) -> Void)?) -> UInt64 {
        ECS_RegisterSystem(
            name.cString(using: .utf8),
            components,
            Int(components.count),
            isParallel,
            block
        )
    }

    public static func getComponentId<T>(type: T.Type) -> UInt64 {
        id(for: T.self)
    }

    public static func getIteratorSize(it: UnsafeMutableRawPointer) -> Int {
        ECS_GetIteratorSize(it)
    }

    public static func getIteratorData<T>(it: UnsafeMutableRawPointer, index: UInt32, type: T.Type) -> UnsafeMutableBufferPointer<T>? {
        // Get the size of the buffer
        let size = ECS_GetIteratorSize(it)
        // Get the pointer to the buffer
        guard let ptr = ECS_GetComponentsFromIterator(it, index + 1, MemoryLayout<T>.size) else {
            return nil
        }
        // Cast the pointer to the correct type
        return UnsafeMutableBufferPointer(start: ptr.assumingMemoryBound(to: T.self), count: size)
    }

    private static func id<T>(for type: T.Type) -> UInt64 {
        let typePointer = unsafeBitCast(type, to: UInt64.self)
        return typePointer
    }

    public static func getIteratorEntities(it: UnsafeMutableRawPointer) -> UnsafeBufferPointer<UInt64>? {
        var buff = ECS_GetEntitiesFromIterator(it)
        let size = ECS_GetIteratorSize(it)

        return UnsafeBufferPointer(start: buff!, count: size)        // Create a mutable buffer pointer from the pointer
    }
}