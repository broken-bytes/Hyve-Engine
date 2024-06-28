@_implementationOnly import Bridge

public enum System {
    public static func createWindow(title: String, posx: Int32, posY: Int32, width: Int32, height: Int32) -> UnsafeMutableRawPointer? {
        System_CreateWindow(title, posx, posY, width, height)
    }
}