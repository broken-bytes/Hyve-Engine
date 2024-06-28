import Foundation
@_implementationOnly import Native

public struct Texture {
    internal let resource: TrackedResource

    @MainActor
    internal init(path: String) {
        self.resource = ResourceManager.shared.loadTexture(from: path)
    }
}