#ifndef _UTIL_MEMORY_HPP_
#define _UTIL_MEMORY_HPP_

#include <iostream>
#include <memory>

namespace util {
	class memory {
	public:
		memory() = default;

		bool attach(uint32_t pid);

		bool valid();
		
		bool read(uintptr_t address, void* buffer, size_t size);
		bool write(uintptr_t address, const void* buffer, size_t size);
		
		template <typename T>
		T read(uintptr_t address, size_t size = sizeof(T)) {
			T result = {};
			this->read(address, &result, size);
			return result;
		}

		template <typename T>
		void write(uintptr_t address, const T value, size_t size = sizeof(T)) {
			this->write(address, reinterpret_cast<const void*>(&value), size);
		}

	private:
		uint64_t _handle;
	};
}

#endif