public struct Vec3<T>: Codable where T: Hashable, T: Comparable, T: Numeric, T: Codable {
    public var x: T
    public var y: T
    public var z: T

    public static var zero: Vec3<T> { get { Vec3(x: 0, y: 0, z: 0) } }

    public init(x: T, y: T, z: T) {
        self.x = x
        self.y = y
        self.z = z
    }

    public init(_ x: T) {
        self.x = x
        self.y = x
        self.z = x
    }

    public init(_ value: (T, T, T)) {
        self.x = value.0
        self.y = value.1
        self.z = value.2
    }

    public static func + (lhs: Vec3<T>, rhs: Vec3<T>) -> Vec3<T> {
        return Vec3(x: lhs.x + rhs.x, y: lhs.y + rhs.y, z: lhs.z + rhs.z)
    }

    public static func - (lhs: Vec3<T>, rhs: Vec3<T>) -> Vec3<T> {
        return Vec3(x: lhs.x - rhs.x, y: lhs.y - rhs.y, z: lhs.z - rhs.z)
    }

    public static func * (lhs: Vec3<T>, rhs: Vec3<T>) -> Vec3<T> {
        return Vec3(x: lhs.x * rhs.x, y: lhs.y * rhs.y, z: lhs.z * rhs.z)
    }

    public static func / (lhs: Vec3<T>, rhs: Vec3<T>) -> Vec3<T> where T: FloatingPoint {
        return Vec3(x: lhs.x / rhs.x, y: lhs.y / rhs.y, z: lhs.z / rhs.z)
    }

    public static func += (lhs: inout Vec3<T>, rhs: Vec3<T>) {
        lhs = lhs + rhs
    }

    public static func -= (lhs: inout Vec3<T>, rhs: Vec3<T>) {
        lhs = lhs - rhs
    }

    public static func *= (lhs: inout Vec3<T>, rhs: Vec3<T>) {
        lhs = lhs * rhs
    }

    public static func /= (lhs: inout Vec3<T>, rhs: Vec3<T>) where T: FloatingPoint {
        lhs = lhs / rhs
    }
}

public typealias Vector3 = Vec3<Float>

extension Vec3: Hashable {
    public static func == (lhs: Vec3<T>, rhs: Vec3<T>) -> Bool {
        return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z
    }
}