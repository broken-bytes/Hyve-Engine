import Foundation
@_implementationOnly import Native

@MainActor
public class ResourceManager {
    public static let shared = ResourceManager()

    private var resources: [TrackedResource] = []

    private init() {}

    public func loadTexture(from path: String) -> TrackedResource {
        if let resource = resources.first(where: { $0.path == path && $0.type == .texture }) {
            return resource
        }

        let resource = TrackedResource(id: NativeRendering.loadTexture(path: path), type: .texture, path: path, name: path)
        resources.append(resource)

        return resource
    }

    public func loadShader(from path: String, type: UInt8) -> TrackedResource {
        if let resource = resources.first(where: { $0.path == path && $0.type == .shader }) {
            return resource
        }

        let resource = TrackedResource(id: NativeRendering.createShader(path: path, type: type), type: .shader, path: path, name: path)
        resources.append(resource)

        return resource
    }

    public func loadMaterial(name: String) -> TrackedResource? {
        if let resource = resources.first(where: { $0.name == name && $0.type == .material }) {
            return resource
        }

        return nil
    }

    public func createMaterial(from path: String) -> TrackedResource {
        // Load the material template json
        let template = IO.load(file: path)
        // Create a String from the buffer
        guard let jsonString = String(data: Data(template!), encoding: .ascii) else {
            fatalError("Failed to create a string from the buffer")
        }
        guard
            let data = jsonString.data(using: .ascii)?.filter({
                $0 != 0 && $0 != 10 && $0 != 13
            }),
            let material = try? JSONDecoder().decode(MaterialTemplate.self, from: data)
        else {
            fatalError("Failed to create data from the string")
        }

        let pixelShader = loadShader(from: material.pixelShader, type: 1)
        let vertexShader = loadShader(from: material.vertexShader, type: 0)

        // Now create the material
        let resource = TrackedResource(
            id: NativeRendering.createMaterial(pixelShader: pixelShader.id, vertexShader: vertexShader.id, isInstanced: material.isInstanced),
            type: .material, path: path, name: material.name
        )
        resources.append(resource)

        // Set the properties
        for texture in material.textures {
            let textureResource = loadTexture(from: texture.path)
            NativeRendering.setMaterialTexture(material: resource.id, texture: textureResource.id, name: texture.key)
        }

        return resource
    }

    public func copyMaterial(from material: TrackedResource) -> TrackedResource {
        let resource = TrackedResource(id: NativeRendering.copyMaterial(material: material.id), type: .material, path: material.path, name: material.name)
        resources.append(resource)

        return resource
    }
}