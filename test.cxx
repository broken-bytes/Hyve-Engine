#include "imgui/Bridge_ImGui.h"
#include "input/Bridge_Input.h"
#include <shared/NativePointer.hxx>
#include <io/Bridge_IO.h>
#include "rendering/Bridge_Rendering.h"
#include <rendering/Rendering.hxx>
#include "ecs/Bridge_ECS.h"
#include <ecs/EntityRegistry.hxx>
#include "system/Bridge_System.h"
#include <array>
#include <chrono>
#include <cstdlib> // For _putenv_s function
#include <string>
#include <fstream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <vector>
#include <Windows.h>
#include <filesystem>

void setup() {
	auto ctx = ImGui_CreateImGuiContext();
	auto window = System_CreateWindow("Dummy", 0x1FFF0000u | 0, 0x1FFF0000u | 0, 1920, 1080);

	IO_Init(std::filesystem::current_path().string().c_str());

	Input_Init(ctx);
	ImGui_Init(ctx);
	Rendering_Init(window, ctx);
#ifdef _DEBUG
	ECS_Init(nullptr, true);
#else
	ECS_Init(nullptr, false);
#endif

	Rendering_SetViewport(0, 0, 1920, 1080);

	// Load the Assembly(KyaniteEngine.dll) and call "assembly_main" function
	auto assembly = LoadLibraryA("KyaniteEngine.dll");
	if (assembly) {
		auto assembly_main = (void(*)())GetProcAddress(assembly, "assembly_main");
		if (assembly_main) {
			assembly_main();
		}
	}
	else {
		std::cout << "Failed to load the assembly" << std::endl;
		exit(1);
	}

	Input_Subscribe([](Event* event) {
		// Close the window when the escape key is pressed or the quit event is received
		if (event->type == EventType::System) {
			auto systemEvent = (SystemEvent*)event;
			if (systemEvent->type == SystemEventType::Quit) {
				exit(0);
			}
		}
	});
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
		ECS_Update(deltaTime);
		Rendering_Update(deltaTime);
		Rendering_PostFrame();

		auto end = std::chrono::high_resolution_clock::now();
		deltaTime = std::chrono::duration<float, std::chrono::seconds::period>(end - start).count();
	}
}