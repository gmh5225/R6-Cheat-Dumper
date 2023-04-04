#include "util.hpp"
#include <windows.h>
#include <iostream>
#include <Tlhelp32.h>

uint32_t util::get_pid_from_window(const char* window_name) {
	HWND windowHandle = FindWindowA(NULL, window_name);
	DWORD* processID = new DWORD;
	GetWindowThreadProcessId(windowHandle, processID);
	return *processID;
}

uint32_t util::get_pid_from_class(const char* window_class) {
	HWND windowHandle = FindWindowA(window_class, NULL);
	DWORD* processID = new DWORD;
	GetWindowThreadProcessId(windowHandle, processID);
	return *processID;
}

uint32_t util::get_pid_from_file(const char target[]) {
	HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (snap == INVALID_HANDLE_VALUE) {
		std::cout << GetLastError() << std::endl;
	}
	PROCESSENTRY32 pe32;
	pe32.dwSize = sizeof(PROCESSENTRY32);
	if (!Process32First(snap, &pe32)) {
		std::cout << "[!] Error getting Process ID: " << GetLastError() << std::endl;
		CloseHandle(snap);
		return 0;
	}
	do {
		if (strcmp(pe32.szExeFile, target) == 0) {
			return pe32.th32ProcessID;
		}
	} while (Process32Next(snap, &pe32));
	CloseHandle(snap);
	return 0;
}

bool util::is_game_open(const char* window_title, const char* window_class, const char window_file[]) {
	uint32_t window_pid = get_pid_from_window(window_title);
	uint32_t class_pid = get_pid_from_class(window_class);
	uint32_t file_pid = get_pid_from_file(window_file);

	if (window_pid != 0 && class_pid != 0 && file_pid != 0)
		if (window_pid == class_pid && file_pid == class_pid)
			return true;

	return false;
}