import Foundation
@_implementationOnly import Native

public struct Entity {
    public let id: UInt64
    public var name: String {
        ECS.name(of: self.id)!
    }

    public var children: [Entity] {
        return []
    }

    public var parent: Entity? {
        guard let id = ECS.parent(of: self.id) else {
            return nil
        }

        let name = ECS.name(of: id)

        return Entity(id: id)
    }

    // Used by the engine for creating non-world entities
    internal init(_ name: String) {
        self.id = ECS.createEntity(name: name)
    }

    internal init(id: UInt64) {
        self.id = id
    }

    public init(name: String, parent: Entity? = nil) {
        self.id = ECS.createEntity(name: name)
    }

    public init(_ name: String, parent: Entity? = nil, @EntityBuilder _ builder: () -> [any Component]) {
        self.init(name: name, parent: parent)
        var components = builder()

        for var component in components {
            // Find the component type and add it to the entity
            let componentType = type(of: component)
            ECS.addComponent(entity: self.id, component: componentType)
            ECS.setComponent(entity: self.id, component: componentType, data: &component)
        }
    }

    public func addComponent<T: Component>(_ component: T.Type) {
        ECS.addComponent(entity: self.id, component: T.self)
    }

    public func setComponent<T: Component>(_ component: T) {
        // Cast the component to a raw pointer
        withUnsafePointer(to: component) { ptr in
            ECS.setComponent(entity: self.id, component: T.self, data: UnsafeRawPointer(ptr))
        }
    }

    public func getComponent<T: Component>(_ component: T.Type) -> UnsafeMutablePointer<T>? {
        ECS.getComponent(entity: self.id, component: T.self)
    }

    public func setParent(_ parent: Entity) {
        ECS.setParent(entity: self.id, parent: parent.id)
    }

    public static func find(by name: String) -> Entity? {
        guard let id = ECS.entity(by: name) else {
            return nil
        }

        return Entity(id: id)
    }
}

extension Entity: Sendable, Identifiable, Hashable {
    public func hash(into hasher: inout Hasher) {
        hasher.combine(id)
    }

    public static func == (lhs: Entity, rhs: Entity) -> Bool {
        return lhs.id == rhs.id
    }
}