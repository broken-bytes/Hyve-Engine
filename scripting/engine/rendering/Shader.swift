import Foundation
@_implementationOnly import Native

public final class Shader: Sendable {
    internal let uuid: String
    internal let handle : UInt32

    init(uuid: String, handle : UInt32) {
        self.uuid = uuid
        self.handle = handle
    }
}