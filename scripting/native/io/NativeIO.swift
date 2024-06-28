@_implementationOnly import Bridge
import Foundation

public enum IO {
    public static func load(file path: String) -> UnsafeBufferPointer<UInt8>? {
        var buffer: UnsafeMutablePointer<UInt8>? = nil
        var size: Int = 0
        let result = IO_LoadFile(path.cString(using: .utf8), &buffer, &size)
        guard result == 0 else {
            return nil
        }

        return UnsafeBufferPointer(start: buffer, count: size)
    }
}
