public enum ResourceType {
    case texture
    case shader
    case material
    case font
    case sound
}

public class TrackedResource: Equatable, Hashable {
    public let id: UInt32
    public let type: ResourceType
    public let path: String
    public let name: String

    public init(id: UInt32, type: ResourceType, path: String, name: String) {
        self.id = id
        self.type = type
        self.path = path
        self.name = name
    }

    public static func == (lhs: TrackedResource, rhs: TrackedResource) -> Bool {
        return lhs.id == rhs.id && lhs.type == rhs.type && lhs.name == rhs.name
    }

    public func hash(into hasher: inout Hasher) {
        hasher.combine(id)
        hasher.combine(type)
        hasher.combine(name)
    }
}