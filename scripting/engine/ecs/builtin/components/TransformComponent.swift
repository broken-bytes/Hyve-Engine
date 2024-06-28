@Component
public struct TransformComponent {
    public var localPosition: Vector3
    public var localRotation: Float
    public var localScale: Vector3
    public var position: Vector3
    public var rotation: Float
    public var scale: Vector3

    public init(position: Vector3 = .zero, rotation: Float = 0, scale: Vector3 = .zero) {
        self.localPosition = position
        self.localRotation = rotation
        self.localScale = scale

        self.position = position
        self.rotation = rotation
        self.scale = scale
    }
}