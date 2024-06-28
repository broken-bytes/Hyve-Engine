import Foundation

public struct Matrix4x4 {
    private var matrix: [[Float]]

    public init() {
        self.matrix = Array(repeating: Array(repeating: 0, count: 4), count: 4)
    }

    public func transposed() -> Matrix4x4 {
        var result = Matrix4x4()
        for i in 0..<4 {
            for j in 0..<4 {
                result[i, j] = matrix[j][i]
            }
        }
        return result
    }

    public static func identity() -> Matrix4x4 {
        var result = Matrix4x4()
        for i in 0..<4 {
            result[i, i] = 1
        }
        return result
    }

    public static func translation(x: Float, y: Float, z: Float) -> Matrix4x4 {
        var result = Matrix4x4.identity()
        result[0, 3] = x
        result[1, 3] = y
        result[2, 3] = z
        return result
    }

    public static func translation(vector: Vector3) -> Matrix4x4 {
        return translation(x: vector.x, y: vector.y, z: vector.z)
    }

    public static func rotationX(angle: Float) -> Matrix4x4 {
        var result = Matrix4x4.identity()
        let rad = angle * Float.pi / 180
        result[1, 1] = Float(cos(rad))
        result[1, 2] = -Float(sin(rad))
        result[2, 1] = Float(sin(rad))
        result[2, 2] = Float(cos(rad))
        return result
    }

    public static func rotationY(angle: Float) -> Matrix4x4 {
        var result = Matrix4x4.identity()
        let rad = angle * Float.pi / 180
        result[0, 0] = Float(cos(rad))
        result[0, 2] = Float(sin(rad))
        result[2, 0] = -Float(sin(rad))
        result[2, 2] = Float(cos(rad))
        return result
    }

    public static func rotationZ(angle: Float) -> Matrix4x4 {
        var result = Matrix4x4.identity()
        let rad = angle * Float.pi / 180
        result[0, 0] = Float(cos(rad))
        result[0, 1] = -Float(sin(rad))
        result[1, 0] = Float(sin(rad))
        result[1, 1] = Float(cos(rad))
        return result
    }

    public static func scale(x: Float, y: Float, z: Float) -> Matrix4x4 {
        var result = Matrix4x4.identity()
        result[0, 0] = x
        result[1, 1] = y
        result[2, 2] = z
        return result
    }

    public static func scale(vector: Vector3) -> Matrix4x4 {
        return scale(x: vector.x, y: vector.y, z: vector.z)
    }

    public static func * (lhs: Matrix4x4, rhs: Matrix4x4) -> Matrix4x4 {
        var result = Matrix4x4()
        for i in 0..<4 {
            for j in 0..<4 {
                for k in 0..<4 {
                    result[i, j] += lhs[i, k] * rhs[k, j]
                }
            }
        }
        return result
    }

    public func translated(x: Float, y: Float, z: Float) -> Matrix4x4 {
        return self * Matrix4x4.translation(x: x, y: y, z: z)
    }

    public func rotatedX(angle: Float) -> Matrix4x4 {
        return self * Matrix4x4.rotationX(angle: angle)
    }

    public func rotatedY(angle: Float) -> Matrix4x4 {
        return self * Matrix4x4.rotationY(angle: angle)
    }

    public func rotatedZ(angle: Float) -> Matrix4x4 {
        return self * Matrix4x4.rotationZ(angle: angle)
    }

    public func scaled(x: Float, y: Float, z: Float) -> Matrix4x4 {
        return self * Matrix4x4.scale(x: x, y: y, z: z)
    }

    public func flattened() -> [Float] {
        var flatArray: [Float] = []
        for row in matrix {
            for value in row {
                flatArray.append(Float(value))
            }
        }

        return flatArray
    }

    public subscript(row: Int, col: Int) -> Float {
        get {
            precondition(row >= 0 && row < 4 && col >= 0 && col < 4, "Index out of range")
            return matrix[row][col]
        }
        set {
            precondition(row >= 0 && row < 4 && col >= 0 && col < 4, "Index out of range")
            matrix[row][col] = newValue
        }
    }
}

extension Matrix4x4: Equatable, Hashable {
    public static func == (lhs: Matrix4x4, rhs: Matrix4x4) -> Bool {
        for i in 0..<4 {
            for j in 0..<4 {
                if lhs[i, j] != rhs[i, j] {
                    return false
                }
            }
        }
        return true
    }

    public func hash(into hasher: inout Hasher) {
        for i in 0..<4 {
            for j in 0..<4 {
                hasher.combine(matrix[i][j])
            }
        }
    }
}