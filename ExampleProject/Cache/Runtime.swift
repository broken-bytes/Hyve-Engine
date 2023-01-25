import Userland
import Engine
import Foundation
import Math
import Core

internal typealias Start = @convention(c) (UInt32, UInt32, UnsafeMutableRawPointer) -> Void
internal typealias Update = @convention(c) (Float) -> Void
internal typealias SetRootDir = @convention(c) (UnsafePointer<Int8>) -> Void

// Pointer to the engine, Ptr to component register func

var lib: Library!
var updateNative: Update!

@_cdecl("start") public func start(
    argumentCount: UInt32,
    argumentVector: UnsafeMutablePointer<UnsafePointer<Int8>>,
    width: UInt32, 
    height: UInt32, 
    window: UnsafeMutableRawPointer
) {
    lib = Library.loadLibrary(at: "./Kyanite-Runtime.dll")
    let start: Start = lib.loadFunc(named: "Engine_Init")
    let setRootDir: SetRootDir = lib.loadFunc(named: "Engine_SetRootDir")
    updateNative = lib.loadFunc(named: "Engine_Update")
    
    start(width, height, window)

    setRootDir(argumentVector[1])

    try! ComponentRegistry.shared.register(component: TransformComponent.self)
    try! ComponentRegistry.shared.register(component: MeshComponent.self)
    try! ComponentRegistry.shared.register(component: MaterialComponent.self)
    try! ComponentRegistry.shared.register(component: MoveComponent.self)
    try! ComponentRegistry.shared.register(component: PlayerComponent.self)

    World("Test")

    for x in 0..<10000 {
        Entity("Test\(x)") { 
            TransformComponent(
                position: Vector3(x: 0, y: 0, z: 0),
                scale: Vector3(x: 0, y: 0, z: 0),
                rotation: Vector3(x: 0, y: 0, z: 0)
            )
            MoveComponent(
                movement: Vector3(x: Float.random(in: -1..<1), y: Float.random(in: -1..<1), z: Float.random(in: -1..<1))
            )
        }
    }

    System("MoveSystem") { 
        let delta: Float = $0
        let trans: UnsafeMutablePointer<TransformComponent> = $1
        let move: UnsafeMutablePointer<MoveComponent> = $2

        trans.pointee.position = add(left: trans.pointee.position, right: mul(vector: move.pointee.movement, value: delta))
    }
}

@_cdecl("update") public func update(frameTime: Float) {
    updateNative(frameTime)
}