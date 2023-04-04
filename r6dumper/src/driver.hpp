#ifndef _DRIVER_HPP_
#define _DRIVER_HPP_

#include <cstdint>

namespace driver {
	bool initialize();
	bool is_driver_loaded();

	uint64_t open_process(uint32_t pid);

	bool read_memory(uint64_t handle, uint64_t address, void* buffer, uint32_t size);
	bool write_memory(uint64_t handle, uint64_t address, const void* buffer, uint32_t size);

	bool protect_virtual_memory(uint64_t handle, uint64_t address, uint32_t size, uint32_t protect, uint32_t* old_protect);

	uint64_t get_module_base(uint64_t handle, const wchar_t* dllname);

	template <typename T>
	T read(uint64_t handle, uint64_t address) {
		T result;

		read_memory(handle, address, &result, sizeof(T));

		return result;
	}

	template <typename T>
	void write(uint64_t handle, uint64_t address, const T value) {
		write_memory(handle, address, &value, sizeof(T));
	}
}

#endif