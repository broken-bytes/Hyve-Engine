import Foundation

public struct MaterialTemplate: Codable {
    public struct Texture: Codable {
        public let path: String
        public let key: String
    }
    public let name: String
    public let isInstanced: Bool
    public let pixelShader: String
    public let vertexShader: String
    public let textures: [Texture]
    public let ints: [String: Int]
    public let floats: [String: Float]
    public let vectors2: [String: Vector2]
    public let vectors3: [String: Vector3]
    public let vectors4: [String: Vector4]

    // Init from JSON
    public init(from decoder: Decoder) throws {
        let container = try decoder.container(keyedBy: CodingKeys.self)
        // For each key, if it's not present, we set it to an empty dictionary
        name = try container.decode(String.self, forKey: .name)
        isInstanced = try container.decodeIfPresent(Bool.self, forKey: .isInstanced) ?? false
        pixelShader = try container.decode(String.self, forKey: .pixelShader)
        vertexShader = try container.decode(String.self, forKey: .vertexShader)
        textures = try container.decodeIfPresent([Texture].self, forKey: .textures) ?? []
        ints = try container.decodeIfPresent([String: Int].self, forKey: .ints) ?? [:]
        floats = try container.decodeIfPresent([String: Float].self, forKey: .floats) ?? [:]
        vectors2 = try container.decodeIfPresent([String: Vector2].self, forKey: .vectors2) ?? [:]
        vectors3 = try container.decodeIfPresent([String: Vector3].self, forKey: .vectors3) ?? [:]
        vectors4 = try container.decodeIfPresent([String: Vector4].self, forKey: .vectors4) ?? [:]
    }
}