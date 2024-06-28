@_implementationOnly import Native

class PlayerSystem {
    init() {
        ECS.registerSystem(
            name: "PlayerSystem", 
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
            let players = ECS.getIteratorData(it: iterator, index: 0, type: PlayerComponent.self)
            let movement = ECS.getIteratorData(it: iterator, index: 1, type: MoveComponent.self)

            PlayerSystem.run(players: players!, movement: movement!)
        }
    }

    @inline(__always)
    static func run(
        players: UnsafeMutableBufferPointer<PlayerComponent>,
        movement: UnsafeMutableBufferPointer<MoveComponent>
    ) {
       
    }
}