#include "memory.hpp"

#include "../driver/driver.hpp"

namespace util {
	bool memory::attach(uint32_t pid) {
		this->_handle = driver::open_process(pid);
		return this->_handle != 0;
	}

	bool memory::valid() {
		return this->_handle != 0;
	}

	bool memory::read(uintptr_t address, void* buffer, size_t size) {
		return driver::read_memory(this->_handle, address, static_cast<uint8_t*>(buffer), static_cast<uint32_t>(size));
	}

	bool memory::write(uintptr_t address, const void* buffer, size_t size) {
		return driver::write_memory(this->_handle, address, static_cast<const uint8_t*>(buffer), static_cast<uint32_t>(size));
	}
}