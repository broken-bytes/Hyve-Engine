@Component
struct MoveComponent {
    var speed: Vector3

    init(speed: Vector3 = .zero) {
        self.speed = speed
    }
}