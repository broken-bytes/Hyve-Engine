/*@System class RenderSystem {
    static func run(meshes: UnsafeMutableBufferPointer<MeshComponent>, transforms: UnsafeMutableBufferPointer<TransformComponent>) async {
        // COmbine the mesh and transform components into a single buffer and iterate over it and change the position of the transform
       for x in 0..<transforms.count {
           transforms[x].position.x += 0.01
       }
    }
}
*/

@_implementationOnly import Native

class RenderSystem {

    init() {
        ECS.registerSystem(
            name: "RenderSystem", 
            components: [
                ECS.getComponentId(type: SpriteComponent.self), 
                ECS.getComponentId(type: TransformComponent.self)
            ],
            isParallel: true
        ) { iterator in
            guard let iterator = iterator else {
                return
            }
            // Get the size of the buffer
            let sprites = ECS.getIteratorData(it: iterator, index: 0, type: SpriteComponent.self)
            let transforms = ECS.getIteratorData(it: iterator, index: 1, type: TransformComponent.self)

            RenderSystem.run(sprites: sprites!, transforms: transforms!)
        }
    }

    @inline(__always)
    static func run(sprites: UnsafeMutableBufferPointer<SpriteComponent>, transforms: UnsafeMutableBufferPointer<TransformComponent>) {
        // Draw each sprite
        for x in 0..<sprites.count {
            let sprite = sprites[x]
            let transform = transforms[x]

            Renderer.drawSprite(position: transform.position, rotation: transform.rotation, scale: transform.scale, material: sprite.material)
        }
    }
}