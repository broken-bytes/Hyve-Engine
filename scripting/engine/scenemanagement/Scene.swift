public protocol Scene {
    associatedtype Body: SceneNode
    @SceneBuilder var content: Body { get }

    func onDidLoad()
}

extension Scene {
    public func onDidLoad() {}
}

public protocol SceneNode {}

extension Entity: SceneNode {}

public struct TestScene: Scene {
    public init() {}

    public var content: some SceneNode {
        SceneGroup {
            Entity("Player") {
                TransformComponent(position: .zero, rotation: 0, scale: .zero)
            }
            Entity("NPC") {
                TransformComponent(position: .zero, rotation: 0, scale: .zero)
            }
            SceneGroup {
                Entity("Another NPC") {
                    TransformComponent(position: .zero, rotation: 0, scale: .zero)
                }
                ForEach(0..<5) { index in
                    Entity("Enemy \(index)") {
                        TransformComponent(position: .zero, rotation: 0, scale: .zero)
                    }
                }
            }
        }
    }

    public func onDidLoad() {
        print("Scene loaded")
    }
}