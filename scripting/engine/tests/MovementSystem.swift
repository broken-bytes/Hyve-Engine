@_implementationOnly import Native

class MovementSystem {
    init() {
        ECS.registerSystem(
            name: "MovementSystem", 
            components: [
                ECS.getComponentId(type: TransformComponent.self),
                ECS.getComponentId(type: MoveComponent.self)
            ],
            isParallel: true
        ) { iterator in
            guard let iterator = iterator else {
                return
            }
            // Get the size of the buffer
            let transforms = ECS.getIteratorData(it: iterator, index: 0, type: TransformComponent.self)
            let movement = ECS.getIteratorData(it: iterator, index: 1, type: MoveComponent.self)
            MovementSystem.run(transforms: transforms!, movement: movement!)
        }
    }

    @inline(__always)
    static func run(
        transforms: UnsafeMutableBufferPointer<TransformComponent>,
        movement: UnsafeMutableBufferPointer<MoveComponent>
    ) {
        // Draw each sprite
        for x in 0..<transforms.count {
            transforms[x].position += movement[x].speed
        }
    }
}