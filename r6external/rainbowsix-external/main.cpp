#include <iostream>
#include <thread>
#include <Windows.h>

#include "util/xorstr.hpp"

#include "driver/driver.hpp"
#include "features/cheat.hpp"
#include "features/visuals/visuals.hpp"
#include "game/game_util.h"
#include "globals.hpp"
#include "overlay/input/input.hpp"
#include "overlay/overlay.hpp"

#include "util/util.hpp"
#include "util/memory.hpp"

void entry() {
	if (!driver::is_driver_loaded()) {
		MessageBoxA(nullptr, "Driver not loaded!", "Error!", 0);
		exit(1);
	}

	overlay::enable();

	while (!util::is_game_open("Rainbow Six", "R6Game", "RainbowSix.exe")) {
		if (overlay::input::key_pressed(VK_DELETE))
			exit(0);
		
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}

	globals::game_pid = util::get_pid_from_file("RainbowSix.exe");

	if (globals::game_pid == 0) {
		MessageBoxA(nullptr, "Invalid process id", "Error!", 0);
		exit(1);
	}
	
	if (!globals::memory.attach(globals::game_pid)) {
		MessageBoxA(nullptr, "Invalid handle", "Error!", 0);
		exit(1);
	}

	globals::module_base = driver::get_module_base(globals::game_pid, L"RainbowSix.exe");

	if (globals::module_base == 0) {
		MessageBoxA(nullptr, "Failed to get module base address", "Error!", 0);
		exit(1);
	}

	Beep(500, 500);

	while (!game::update_addresses())
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	
	while (overlay::input::key_pressed(VK_DELETE) == 0) {
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
		
		//visuals::drone(true);

		if (!game::in_match() || game::get_profile() == 0) {
			std::this_thread::sleep_for(std::chrono::milliseconds(1)); // why not save some cpu?
			continue;
		}

		cheat::run();
	}

	cheat::restore();

	overlay::disable();

	Beep(500, 500);
	
	exit(0);
}

BOOL WINAPI DllMain(HINSTANCE hmodule, DWORD dwreason, LPVOID lpreserved) {
	DisableThreadLibraryCalls(hmodule);

	if (dwreason == DLL_PROCESS_ATTACH) {
		if (const auto thread = CreateThread(nullptr, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(entry), nullptr, 0, nullptr))
			CloseHandle(thread);
	}
	
	return TRUE;
}