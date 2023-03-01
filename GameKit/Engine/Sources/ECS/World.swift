public extension ECS {

    struct World {

        var entities: [Entity] = []
        private var currentId: UInt64 = 0
        private var currentComponentId: UInt16 = 1
        private var numComponents: UInt16 = 0
        private var pipelines: [Pipeline] = [Pipeline.onUpdate]

        internal init() {
        }

        public mutating func registerComponent<T>(_ type: T.Type) -> UInt16 {
            let id = currentComponentId

            if let value = ComponentIdMapping.shared.mappings.first(where: { $0.value == type.self}) {
                return value.key
            }

            currentComponentId *= 2
            numComponents += 1

            ComponentIdMapping.shared.addMapping(for: T.self, index: id)

            return id
        }

        public mutating func createEntity(named: String, _ components: Any...) -> Entity {
            let typeIds = components.compactMap { item in 
                ComponentIdMapping.shared.mappings.first(where: { $0.value == type(of: item) })?.key
            }

            let bitMask = BitSet(length: numComponents)
                        
            for x in 0..<typeIds.count {
                var bitIndex: UInt16 = 0
                var shifted = typeIds[x]

                while shifted > 0 {
                    shifted = shifted >> 1
                    if shifted > 0 {
                        bitIndex += 1
                    }                
                }

                bitMask.setBit(at: UInt16(bitIndex))
            }

            let entity = Entity(id: currentId, name: named, bitMask: bitMask, components: components.compactMap { $0 })
            currentId += 1
            
            return entity
        }

        public mutating func createSystem(named: String, _ archetype: Any.Type..., pipeline: Pipeline, system: @escaping SystemIterFunc) throws -> Void {
            let id = nextId()
            var mask = BitSet(length: numComponents)
            for comp in archetype {
                var bitIndex: UInt16 = 0
                guard let item = ComponentIdMapping.shared.mappings.first(where: { $0.value == comp.self }) else {
                    throw ECSError.componentNotRegistered(type: comp.self, message: "Trying to create a system that operators on an unregistered component")
                }
                var shifted = item.key

                while shifted > 0 {
                    shifted = shifted >> 1
                    if shifted > 0 {
                        bitIndex += 1
                    }                
                }

                mask.setBit(at: UInt16(bitIndex))
            }
            pipeline.systems.append(System(id: id, name: named, archetype: mask, multithreaded: false, function: system))
        }

        public func tick() {
            for pipeline in pipelines {
                pipeline.tick()
            }
        }
    }
}

    // Extension that holds helper methods for world
fileprivate extension ECS.World {
    
    mutating func nextId() -> UInt64 {
        let id = currentId
        currentId += 1
        return id
    }
}