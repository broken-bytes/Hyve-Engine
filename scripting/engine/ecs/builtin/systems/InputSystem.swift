@_implementationOnly import Native

class InputSystem {
    init() {
        ECS.registerSystem(
            name: "InputSystem", 
            components: [
            ],
            isParallel: false
        ) { iterator in
            guard let iterator = iterator else {
                return
            }

            InputSystem.run()
        }
    }


    static func run() {
        print("Inputsystem tick")
    }
}