#include "imgui/Bridge_ImGui.h"
#include "input/Bridge_Input.h"
#include "rendering/Bridge_Rendering.h"
#include "system/Bridge_System.h"
#include <chrono>
#include <cstdlib> // For _putenv_s function
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <Windows.h>
#include <filesystem>


void eventListener(Event* event) {
	// Handle events
	if (event->type == EventType::System) {
		auto systemEvent = (SystemEvent*)event;
		if(systemEvent->type == SystemEventType::Quit) {
			exit(0);
		}
	}
}

void setup() {
	auto ctx = ImGui_CreateImGuiContext();
	auto window = System_CreateWindow("Dummy", 0x1FFF0000u | 0, 0x1FFF0000u | 0, 1920, 1080);

	Input_Init(ctx);
	ImGui_Init(ctx);
	Rendering_Init(window, ctx);

	Rendering_SetViewport(0, 0, 1920, 1080);

	// Subscribe to events
	Input_Subscribe(&eventListener);
}

int main() {
	setup();

	float deltaTime = 0.0f;
	while (true) {
		// Calculate delta time
		auto start = std::chrono::high_resolution_clock::now();
		// Parse SDL events
		Rendering_PreFrame();
		Input_Poll();
		Rendering_Update(deltaTime);
		// Draw a dummy box	
		Rendering_PostFrame();
		auto end = std::chrono::high_resolution_clock::now();
		deltaTime = std::chrono::duration<float, std::chrono::seconds::period>(end - start).count();
	}
}