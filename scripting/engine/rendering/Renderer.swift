import Foundation
@_implementationOnly import Native

public enum Renderer {
    public static func drawSprite(position: Vector3, rotation: Float, scale: Vector3, material: Material) {
        // Create a 4x4 matrix as a float array from the position, rotation and scale
        let transform = [
            scale.x * cos(rotation), -scale.y * sin(rotation), 0, 0,  // Rotate and scale on X and Y
            scale.x * sin(rotation), scale.y * cos(rotation), 0, 0,   // Rotate and scale on X and Y
            0, 0, scale.z, 0,                                   // Scale Z
            position.x, position.y, position.z, 1              // Translate
        ]
        NativeRendering.drawSprite(transform: transform, material: material.resource.id)
    }
}