import Foundation
import Core

public final class Entity {
    public var name: String
    public let uuid: String

    private let internalId: UInt64

    public init(name: String) {
        self.internalId = NativeCore.shared.createNewEntity(name: name)
        self.name = name
        self.uuid = UUID().uuidString
        World.activeWorld.addEntity(entity: self)
    }

    public convenience init(_ name: String, @EntityBuilder components: () -> ComponentContent) {
        self.init(name: name)
        EntityBuilder.processEntity(entity: self, components: components)
    }

    deinit {
        NativeCore.shared.deleteEntity(id: internalId)
    }

    public func add(@EntityBuilder components: () -> ComponentContent) {
        EntityBuilder.processEntity(entity: self, components: components)
    }

    public func addComponent<T: Component>(component: T) {
        print("Adding \(T.self) to \(self)")
        let typeId = try! ComponentRegistry.shared.resolve(component: T.self)
        let ptr = UnsafeMutablePointer<T>.allocate(capacity: 1)
        ptr.initialize(to: component)
        let rawPtr = UnsafeMutableRawPointer(mutating: ptr)
        _ = NativeCore.shared.addComponentToEntity(entity: self.internalId, component: typeId, data: rawPtr, type: T.self)
    }

    public func component<T>(of type: T.Type) -> UnsafeMutablePointer<T>? {
        let typeId = try! ComponentRegistry.shared.resolve(component: T.self)
        let rawPtr = NativeCore.shared.getComponentFromEntity(entity: self.internalId, component: typeId)
        
        return rawPtr.bindMemory(to: T.self, capacity: 1)
    }
}
