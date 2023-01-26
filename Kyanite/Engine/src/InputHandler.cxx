#include "InputHandler.hxx"

#include <map>
#include <mutex>

namespace InputHandler {
    std::map<MouseButton, ButtonState> MouseButtonStates;
    std::map<SDL_Scancode, ButtonState> KeyboardButtonStates;
    std::vector<uint32_t> RelativeMouse = { 0, 0 };
    std::vector<uint32_t> AbsoluteMouse = { 0, 0 };
    std::mutex Mutex = {};

    auto Init() -> void {
        MouseButtonStates[MouseButton::Left] = ButtonState::Up;
        MouseButtonStates[MouseButton::Right] = ButtonState::Up;
        MouseButtonStates[MouseButton::Middle] = ButtonState::Up;
        MouseButtonStates[MouseButton::Extra0] = ButtonState::Up;
        MouseButtonStates[MouseButton::Extra1] = ButtonState::Up;

        for (int x = 0; x < SDL_NUM_SCANCODES; x++) {
            KeyboardButtonStates[SDL_Scancode(x)] = ButtonState::Up;
        }
    }

    auto AddMouseClickEvent(MouseButton mouseButton, ButtonState state) -> void {
        std::scoped_lock lock{ Mutex };
        if (state == ButtonState::Down) {
            if (MouseButtonStates[mouseButton] == ButtonState::Down) {
                MouseButtonStates[mouseButton] = ButtonState::Held;
                return;
            }
        }

        if (state == ButtonState::Up) {
            if (MouseButtonStates[mouseButton] == ButtonState::Down) {
                MouseButtonStates[mouseButton] = ButtonState::Released;
                return;
            }
        }

        MouseButtonStates[mouseButton] = state;
    }

    auto AddMouseMoveEvent(uint32_t x, uint32_t y) -> void {
        std::scoped_lock lock{ Mutex };
        AbsoluteMouse = { x, y };
        RelativeMouse = { x - RelativeMouse[0], y - RelativeMouse[1] };
    }

    auto AddKeyboardEvent(SDL_Scancode code, ButtonState state) -> void {
        std::scoped_lock lock{ Mutex };
        if (state == ButtonState::Down) {
            if (KeyboardButtonStates[code] == ButtonState::Down || KeyboardButtonStates[code] == ButtonState::Held) {
                KeyboardButtonStates[code] = ButtonState::Held;
                return;
            }
        }

        if (state == ButtonState::Up) {
            if (KeyboardButtonStates[code] == ButtonState::Up) {
                KeyboardButtonStates[code] = ButtonState::Released;
                return;
            }
        }

        KeyboardButtonStates[code] = state;
    }

    auto GetKeyboardButtonState(SDL_Scancode code) -> ButtonState {
        return KeyboardButtonStates[code];
    }

    auto GetMouseButtonState(MouseButton button) -> ButtonState {
        return MouseButtonStates[button];
    }

    auto GetMouseMovement() -> std::vector<uint32_t> {
        return RelativeMouse;
    }

    auto GetMouseAbs() -> std::vector<uint32_t> {
        return AbsoluteMouse;
    }

    auto Flush() -> void {
        std::scoped_lock lock{ Mutex };
        for (auto button: KeyboardButtonStates) {
            if (KeyboardButtonStates[button.first] == ButtonState::Down) {
                KeyboardButtonStates[button.first] = ButtonState::Held;
                continue;
            }
            if (KeyboardButtonStates[button.first] == ButtonState::Up) {
                KeyboardButtonStates[button.first] = ButtonState::Released;
                continue;
            }
        }

        for (auto button : MouseButtonStates) {
            if (MouseButtonStates[button.first] == ButtonState::Down) {
                MouseButtonStates[button.first] = ButtonState::Held;
                continue;
            }
            if (MouseButtonStates[button.first] == ButtonState::Up) {
                MouseButtonStates[button.first] = ButtonState::Released;
                continue;
            }
        }
    }
}