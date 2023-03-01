internal extension ECS {

    class ComponentIdMapping {

        static let shared = ComponentIdMapping()

        internal private(set) var mappings: [UInt16:Any.Type] = [:]
        internal private(set) var sizes: [UInt16:Int] = [:]

        private init() {

        }

        func clear() {
            mappings = [:]
        }

        func addMapping<T>(for type: T.Type, index: UInt16) {
            mappings[index] = type
            sizes[index] = MemoryLayout<T>.stride
        }
    }
}