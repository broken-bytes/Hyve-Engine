@_implementationOnly import Bridge
import Foundation

public enum NativeRendering {
    public static func loadTexture(path: String) -> UInt32 {
        var buffer: UnsafeMutablePointer<UInt8>? = nil
        var size: Int = 0
        let result = IO_LoadFile(path.cString(using: .utf8), &buffer, &size)

        guard result == 0 else {
            return 0
        }

        return Rendering_LoadTexture(buffer, size)
    }

    public static func createShader(path: String, type: UInt8) -> UInt32 {
        var buffer: UnsafeMutablePointer<UInt8>? = nil
        var size: Int = 0
        let result = IO_LoadFile(path.cString(using: .utf8), &buffer, &size)

        guard result == 0 else {
            return 0
        }

        return Rendering_CreateShader(buffer, type)
    }

    public static func createMaterial(pixelShader: UInt32, vertexShader: UInt32, isInstanced: Bool) -> UInt32 {
        return Rendering_CreateMaterial(pixelShader, vertexShader, isInstanced)
    }

    public static func copyMaterial(material: UInt32) -> UInt32 {
        return Rendering_CopyMaterial(material)
    }

    public static func setMaterialTexture(material: UInt32, texture: UInt32, name: String) {
        Rendering_SetMaterialTexture(material, texture, name.cString(using: .utf8))
    }

    @inline(__always)
    public static func drawSprite(transform: [Float], material: UInt32) {
        Rendering_DrawSprite(transform, material)
    }
}