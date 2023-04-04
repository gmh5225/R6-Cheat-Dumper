#ifndef _DRIVER_DRIVER_HPP_
#define _DRIVER_DRIVER_HPP_

#include <iostream>

namespace driver {
	extern bool is_driver_loaded();
	
	extern uint64_t open_process(uint32_t pid);

	extern bool read_memory(uint64_t handle, uintptr_t address, uint8_t* buffer, uint32_t size);
	extern bool write_memory(uint64_t handle, uintptr_t address, const uint8_t* buffer, uint32_t size);

	extern bool protect_virtual_memory(uint64_t handle, uintptr_t address, uint32_t size, uint32_t protect, uint32_t* old_protect);

	extern uintptr_t get_module_base(uint64_t handle, const wchar_t* dllname);
}
	
#endif