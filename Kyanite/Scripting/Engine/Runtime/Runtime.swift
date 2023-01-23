var engine = UnsafeMutablePointer<Engine>.allocate(capacity: 1)

@_cdecl("start") public func start(
    argumentCount: UInt32,
    argumentVector: UnsafeMutablePointer<UnsafeMutablePointer<UInt8>>,
    width: UInt32,
    height: UInt32, 
    window: UnsafeMutableRawPointer
) {
    var args: [String] = []
    engine.initialize(repeating: Engine(), count: 1)
    print(args)

    for x in 0..<argumentCount {
        let ptr = argumentVector[Int(x)]
        args.append(String(cString: ptr))
        Logger.shared.println(str: String(cString: ptr))
    }

    engine.pointee.start(args: args, width: width, height: height, window: window)

    #if _ENGINE      
        let rawPtr = UnsafeMutableRawPointer(mutating: engine)
        Assembly.shared.loadAssembly(engine: rawPtr)
    #endif
}

@_cdecl("update") public func update() {
    engine.pointee.update()
}

@_cdecl("onKeyUp") public func onKeyUpHandler(key: UInt8, name: UnsafeMutablePointer<UInt8>) {
    let str = String(cString: name)
    engine.pointee.onKeyChanged(key: key, isPressed: false, name: str)
}

@_cdecl("onKeyDown") public func onKeyDownHandler(key: UInt8, name: UnsafeMutablePointer<UInt8>) {
    let str = String(cString: name)
    engine.pointee.onKeyChanged(key: key, isPressed: true, name: str)
}

@_cdecl("onMouseButtonUp") public func onMouseButtonUp(button: UInt8) {
    engine.pointee.onMouseButtonChanged(button: button, isPressed: true)
}

@_cdecl("onMouseButtonDown") public func onMouseButtonDown(button: UInt8) {
    engine.pointee.onMouseButtonChanged(button: button, isPressed: false)
}

@_cdecl("onAxisChanged") public func onAxisChangedHandler(axis: UInt8, value: Float) {
    engine.pointee.onAxisChanged(axis: axis, value: value)
}

@_cdecl("onMouseMoved") public func onMousePositionChanged(mouseX: Int32, mouseY: Int32) {
    engine.pointee.onMousePositionChanged(mouseX: mouseX, mouseY: mouseY)
}

@_cdecl("onViewportResized") public func onViewportResized(width: UInt32, height: UInt32) {
    engine.pointee.onViewportResized(width: width, height: height)
}

