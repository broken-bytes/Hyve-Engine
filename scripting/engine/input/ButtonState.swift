public enum ButtonState: UInt8, Codable, Sendable {
    case none = 0
    case pressed = 1
    case held = 2
    case released = 3
}