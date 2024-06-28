@_implementationOnly import Bridge

public enum NativeInput {
    public static func registerListener() {
        Input_Subscribe { event in 
            print(event!.pointee)
        }
    }
}