#pragma once


#include <shared/Events.hxx>
#include <shared/Exported.hxx>
#include <shared/NativePointer.hxx>

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus 
extern "C" {
#endif

	// The keyboard buttons, the values are the SDL scancodes
	EXPORTED enum Input_Buttons {
		INPUT_KEYBOARD_0 = 0x1E,
		INPUT_KEYBOARD_1 = 0x30,
		INPUT_KEYBOARD_2 = 0x2E,
		INPUT_KEYBOARD_3 = 0x20,
		INPUT_KEYBOARD_4 = 0x12,
		INPUT_KEYBOARD_5 = 0x21,
		INPUT_KEYBOARD_6 = 0x22,
		INPUT_KEYBOARD_7 = 0x23,
		INPUT_KEYBOARD_8 = 0x17,
		INPUT_KEYBOARD_9 = 0x24,
		INPUT_KEYBOARD_A = 0x04,
		INPUT_KEYBOARD_B = 0x05,
		INPUT_KEYBOARD_C = 0x06,
		INPUT_KEYBOARD_D = 0x07,
		INPUT_KEYBOARD_E = 0x08,
		INPUT_KEYBOARD_F = 0x09,
		INPUT_KEYBOARD_G = 0x0A,
		INPUT_KEYBOARD_H = 0x0B,
		INPUT_KEYBOARD_I = 0x0C,
		INPUT_KEYBOARD_J = 0x0D,
		INPUT_KEYBOARD_K = 0x0E,
		INPUT_KEYBOARD_L = 0x0F,
		INPUT_KEYBOARD_M = 0x10,
		INPUT_KEYBOARD_N = 0x11,
		INPUT_KEYBOARD_O = 0x12,
		INPUT_KEYBOARD_P = 0x13,
		INPUT_KEYBOARD_Q = 0x14,
		INPUT_KEYBOARD_R = 0x15,
		INPUT_KEYBOARD_S = 0x16,
		INPUT_KEYBOARD_T = 0x17,
		INPUT_KEYBOARD_U = 0x18,
		INPUT_KEYBOARD_V = 0x19,
		INPUT_KEYBOARD_W = 0x1A,
		INPUT_KEYBOARD_X = 0x1B,
		INPUT_KEYBOARD_Y = 0x1C,
		INPUT_KEYBOARD_Z = 0x1D,
		INPUT_KEYBOARD_RETURN = 0x28,
		INPUT_KEYBOARD_ESCAPE = 0x29,
	};

	/**
	 * @brief Inits the input system
	 *
	 */
	EXPORTED void Input_Init(NativePointer imGuiContext);

	/**
	 * @brief Polls from the input system
	 *
	 */
	EXPORTED void Input_Poll();

	/**
	 * @brief Gets the state of the given keyboard button
	 * @param buttonId The Id of the button to query
	 *
	 */
	EXPORTED uint8_t Input_GetKeyboardButtonState(uint32_t buttonId);

	/**
	 * @brief Gets the state of the given mouse button
	 * @param buttonId The Id of the button to query
	 *
	 */
	EXPORTED uint8_t Input_GetMouseButtonState(uint32_t buttonId);

	/**
	 * @brief Gets the value of the given mouse axis
	 * @param buttonId The Id of the button to query
	 *
	 */
	EXPORTED float Input_GetMouseAxis(uint8_t axis);

	/**
	 * @brief Gets the state of the given controller button
	 * @param uint8_t The controller to be used
	 * @param buttonId The Id of the button to query
	 *
	 */
	EXPORTED uint8_t Input_GetControllerButtonState(uint8_t controllerId, uint8_t buttonId);

	/**
	 * @brief Gets the value of the given controller axis
	 * @param uint8_t The controller to be used
	 * @param buttonId The Id of the button to query
	 *
	 */
	EXPORTED float Input_GetControllerAxis(uint8_t controllerId, uint8_t axis);

	/**
	* @brief Subscribes to the input system events
	* @param subscription The function to be called when an event is triggered
	*
	*/
	EXPORTED void Input_Subscribe(void(*subscription)(Event* event));

#ifdef __cplusplus 
}
#endif
