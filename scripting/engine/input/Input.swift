@_implementationOnly import Native

enum Input {
    public static func start() {
        NativeInput.registerListener()
    }
}