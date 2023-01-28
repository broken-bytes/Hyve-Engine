import Foundation
import Core

internal typealias CreateEntityFunc     = @convention(c) (UnsafePointer<Int8>) -> UInt64
internal typealias DeleteEntityFunc     = @convention(c) (UInt64) -> Void
internal typealias AddComponentFunc     = @convention(c) (UInt64, UInt64, UInt64, UnsafeMutableRawPointer) -> Void
internal typealias GetComponentFunc     = @convention(c) (UInt64, UInt64) -> UnsafeMutableRawPointer
internal typealias AddRigidBody         = @convention(c) (UInt64, Float, Bool) -> Void 
internal typealias AddBoxCollider       = @convention(c) (UInt64, Float, Float, Float) -> Void 
internal typealias AddSphereCollider    = @convention(c) (UInt64, Float) -> Void 
internal typealias AddCapsuleCollider   = @convention(c) (UInt64, Float, Float) -> Void 

public final class Entity {
    public var name: String
    public let uuid: String

    private let internalId: UInt64

    private static let lib = Core.Runtime.library
    private static let createEntity: CreateEntityFunc = lib.loadFunc(named: "ECS_CreateEntity")
    private static let deleteEntity: DeleteEntityFunc = lib.loadFunc(named: "ECS_DeleteEntity")
    private static let addComponent: AddComponentFunc = lib.loadFunc(named: "ECS_AddComponent")
    private static let getComponent: GetComponentFunc = lib.loadFunc(named: "ECS_GetComponent")
    private static let addRigidBody: AddRigidBody = lib.loadFunc(named: "Physics_AddRigidBody")
    private static let addBoxCollider: AddBoxCollider = lib.loadFunc(named: "Physics_BoxCollider")
    private static let addSphereCollider: AddSphereCollider = lib.loadFunc(named: "Physics_AddSphereCollider")
    private static let addCapsuleCollider: AddCapsuleCollider = lib.loadFunc(named: "Physics_AddCapsuleCollider")

    public init(name: String) {
        self.internalId = name.withCString {
            return Entity.createEntity($0)
        }
        self.name = name
        self.uuid = UUID().uuidString
        World.activeWorld.addEntity(entity: self)
    }

    public convenience init(_ name: String, @EntityBuilder components: () -> ComponentContent) {
        self.init(name: name)
        EntityBuilder.processEntity(entity: self, components: components)
    }

    deinit {
        Entity.deleteEntity(self.internalId)
    }

    public func delete() {
        Entity.deleteEntity(self.internalId)
    }

    public static func delete(entity: Entity) {
        Entity.deleteEntity(entity.internalId)
    }

    public func add(@EntityBuilder components: () -> ComponentContent) {
        EntityBuilder.processEntity(entity: self, components: components)
    }

    public func addComponent<T: Component>(component: T) {
        let typeId = try! ComponentRegistry.shared.resolve(component: T.self)
        let ptr = UnsafeMutablePointer<T>.allocate(capacity: 1)
        ptr.initialize(to: component)
        let rawPtr = UnsafeMutableRawPointer(mutating: ptr)
        Entity.addComponent(self.internalId, typeId, UInt64(MemoryLayout<T>.size), rawPtr)

        if let rigidBody = component as? RigidBodyComponent {
            Entity.addRigidBody(self.internalId, rigidBody.mass, rigidBody.isStatic)
        }

        if let box = component as? BoxColliderComponent {
            Entity.addBoxCollider(self.internalId, box.bounds.x, box.bounds.y, box.bounds.z)
        }

        if let sphere = component as? SphereColliderComponent {
            Entity.addSphereCollider(self.internalId, sphere.radius)
        }

        if let capsule = component as? CapsuleColliderComponent {
            Entity.addCapsuleCollider(self.internalId, capsule.radius, capsule.halfHeight)
        }
    }

    public func component<T>(of type: T.Type) -> UnsafeMutablePointer<T>? {
        let typeId = try! ComponentRegistry.shared.resolve(component: T.self)
        let rawPtr = Entity.getComponent(self.internalId, typeId)
        
        return rawPtr.bindMemory(to: T.self, capacity: 1)
    }
}
