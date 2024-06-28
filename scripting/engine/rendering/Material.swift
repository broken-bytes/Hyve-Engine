import Foundation
@_implementationOnly import Native

public struct Material: Equatable, Hashable {
    internal let resource: TrackedResource

    @MainActor
    public init(path: String) {
        self.resource = ResourceManager.shared.createMaterial(from: path)
    }

    @MainActor
    public init(other: Material) {
        // Create a new material from an existing one. Create a new one and set the same properties
        self.resource = ResourceManager.shared.copyMaterial(from: other.resource)
    }

    @MainActor
    public init(name: String) {
        guard let resource = ResourceManager.shared.loadMaterial(name: name) else {
            fatalError("Failed to load material with name \(name)")
        }

        self.resource = resource
    }

    @MainActor
    public func setTexture(for name: String, texture: Texture) {
        NativeRendering.setMaterialTexture(material: resource.id, texture: texture.resource.id, name: name)
    }

    public static func == (lhs: Material, rhs: Material) -> Bool {
        return lhs.resource == rhs.resource
    }

    public func hash(into hasher: inout Hasher) {
        hasher.combine(resource)
    }
}