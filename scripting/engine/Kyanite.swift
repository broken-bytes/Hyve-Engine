// This will be generated at some point, but for now it's manually created

import Macros
@_implementationOnly import Native

@MainActor
@_cdecl("assembly_main")
public func main(isDebug: Bool = false) {
    print("Swift Assembly Main")
    // Register the component types
    let spriteComponent = ECS.registerComponent(type: SpriteComponent.self, name: "SpriteComponent")
    let transformComponentId = ECS.registerComponent(type: TransformComponent.self, name: "TransformComponent")
    let playerComponentId = ECS.registerComponent(type: PlayerComponent.self, name: "PlayerComponent")
    let moveComponentId = ECS.registerComponent(type: MoveComponent.self, name: "MoveComponent")
    // Create the entities
    var playerMaterial = Material(path: "content/materials/female_body_1.mat")
    let texture = Texture(path: "content/textures/characters/female_body_1.png")

    var swordMaterial = Material(path: "content/materials/sword_1.mat")
    let swordTexture = Texture(path: "content/textures/weapons/sword_1.png")

    var randomX: Float = 0
    var randomY: Float = 0
    var randomXSpeed: Float = 0
    var randomYSpeed: Float = 0

    // Ground material
    var groundMaterial = Material(path: "content/materials/ground_stones_mossy.mat")

    for x in 0..<(640 / 32) + 1 {
        for y in 0..<(360 / 32) + 1 {
            let entity = Entity("Ground\(x)_\(y)")
            entity.addComponent(SpriteComponent.self)
            entity.addComponent(TransformComponent.self)
            entity.setComponent(SpriteComponent(material: groundMaterial))
            entity.setComponent(TransformComponent(position: Vector3(x: Float(x) * 32, y: Float(y) * 32, z: 0), rotation: 0, scale: Vector3(x: 32, y: 32, z: 1)))
        }
    }

    for x: UInt64 in 0..<2500 {
        randomX = Float.random(in: 200..<1200)
        randomY = Float.random(in: 200..<1200)
        randomXSpeed = Float.random(in: -0.1..<0.1)
        randomYSpeed = Float.random(in: -0.1..<0.1)
        let entity = Entity("Player\(x)")
        entity.addComponent(SpriteComponent.self)
        entity.addComponent(TransformComponent.self)
        entity.addComponent(PlayerComponent.self)
        entity.addComponent(MoveComponent.self)
        entity.setComponent(SpriteComponent(material: playerMaterial))
        entity.setComponent(TransformComponent(position: Vector3(x: 240, y: 130, z: 0), rotation: 0, scale: Vector3(x: 32, y: 32, z: 1)))
        entity.setComponent(PlayerComponent(id: x))
        entity.setComponent(MoveComponent(speed: Vector3(x: randomXSpeed, y: randomYSpeed, z: 0)))

        let swordEntity = Entity("Sword\(x)")
        swordEntity.addComponent(SpriteComponent.self)
        swordEntity.addComponent(TransformComponent.self)
        swordEntity.setComponent(SpriteComponent(material: swordMaterial))
        swordEntity.setComponent(TransformComponent(position: Vector3(x: 0, y: 0, z: 1), rotation: 0, scale: Vector3(x: 1, y: 1, z: 1)))

        swordEntity.setParent(entity)
    }

    let hierarchySystem = HierarchySystem()
    let renderSystem: RenderSystem = RenderSystem()
    let movementSystem: MovementSystem = MovementSystem()
    let inputSystem: InputSystem = InputSystem()
}

// Macros
@attached(member, names: named(init))
@attached(memberAttribute)
@attached(extension, conformances: Component, Hashable, Equatable)
public macro Component() = #externalMacro(module: "Macros", type: "ComponentMacro")

@attached(member, names: arbitrary)
public macro System() = #externalMacro(module: "Macros", type: "SystemMacro")
