@_implementationOnly import Native

class HierarchySystem {
    init() {
        ECS.registerSystem(
            name: "HierarchySystem", 
            components: [
                ECS.getComponentId(type: TransformComponent.self)
            ],
            isParallel: false
        ) { iterator in
            guard let iterator = iterator else {
                return
            }
            // Get the size of the buffer
            let transforms = ECS.getIteratorData(it: iterator, index: 0, type: TransformComponent.self)
            var entities = ECS.getIteratorEntities(it: iterator)

            HierarchySystem.run(transforms: transforms!, entities: entities!)
        }
    }


    static func run(transforms: UnsafeMutableBufferPointer<TransformComponent>, entities: UnsafeBufferPointer<UInt64>) {
        for x in 0..<transforms.count {
            // Check if the entity has a parent
            if
                let parent = ECS.parent(of: entities[x]),
                parent != 0,
                let parentTransformPtr: UnsafeMutablePointer<TransformComponent> = ECS.getComponent(
                    entity: parent,
                    component: TransformComponent.self
                )
            {
                let parentTransform = parentTransformPtr.pointee
                var transform = transforms[x]
                // Get the parent transform
                transforms[x].position = parentTransform.position + transform.localPosition
                transforms[x].rotation = parentTransform.rotation + transform.localRotation
                transforms[x].scale = parentTransform.scale * transform.localScale
            }
        }
    }
}