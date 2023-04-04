#ifndef _OVERLAY_CK_HPP_
#define _OVERLAY_CK_HPP_

#include <iostream>
#include <algorithm>
#include <vector>

#include <Windows.h>

namespace overlay {
	static uintptr_t find_pattern(const std::string& pattern, const uintptr_t base_address, const size_t length) {
		static auto pattern_to_byte = [](const std::string& pattern) {
			auto bytes = std::vector<int>{};

			const auto start = const_cast<char*>(pattern.c_str());
			const auto end = start + pattern.length();

			for (auto current = start; current < end; ++current) {
				if (*current == '?') {
					++current;
					if (*current == '?')
						++current;
					bytes.push_back(-1);
				}
				else {
					bytes.push_back(strtoul(current, &current, 16));
				}
			}
			return bytes;
		};

		const auto pattern_bytes = pattern_to_byte(pattern);
		const auto scan_bytes = reinterpret_cast<std::uint8_t*>(base_address);

		const auto pattern_size = pattern_bytes.size();
		const auto pattern_data = pattern_bytes.data();

		for (auto i = 0ul; i < length - pattern_size; ++i) {
			bool found = true;

			for (auto j = 0ul; j < pattern_size; ++j) {
				if (scan_bytes[i + j] != pattern_data[j] && pattern_data[j] != -1) {
					found = false;
					break;
				}
			}

			if (found) {
				return reinterpret_cast<uintptr_t>(&scan_bytes[i]);
			}
		}

		return 0;
	}

	static uintptr_t find_pattern(const std::string& pattern, const uintptr_t module_base) {
		const auto dos_headers = reinterpret_cast<PIMAGE_DOS_HEADER>(module_base);
		const auto nt_headers = reinterpret_cast<PIMAGE_NT_HEADERS>(reinterpret_cast<std::uint8_t*>(module_base) + dos_headers->e_lfanew);

		return find_pattern(pattern, module_base, nt_headers->OptionalHeader.SizeOfImage);
	}

	static uintptr_t find_pattern_module(const std::string& module_name, const std::string& pattern, uint32_t offset = 0, bool rip_relative = false, uint32_t rip_relative_offset = 0) {
		const auto module_base = reinterpret_cast<uintptr_t>(GetModuleHandleA(module_name.c_str()));

		const auto dos_header = reinterpret_cast<PIMAGE_DOS_HEADER>(module_base);
		const auto nt_headers = reinterpret_cast<PIMAGE_NT_HEADERS>(reinterpret_cast<std::uint8_t*>(module_base) + dos_header->e_lfanew);

		const auto section_header_address = module_base + dos_header->e_lfanew + offsetof(IMAGE_NT_HEADERS, OptionalHeader) + nt_headers->FileHeader.SizeOfOptionalHeader;

		for (uint32_t i = 0; i < nt_headers->FileHeader.NumberOfSections; ++i) {
			const auto section_header = reinterpret_cast<PIMAGE_SECTION_HEADER>(section_header_address + i * sizeof(IMAGE_SECTION_HEADER));

			size_t size = section_header->SizeOfRawData;

			if (size == 0) {
				const auto next_header = reinterpret_cast<PIMAGE_SECTION_HEADER>(section_header_address + (i + 1) * sizeof(IMAGE_SECTION_HEADER));
			    size = next_header->VirtualAddress - section_header->VirtualAddress;
			}

			auto address = find_pattern(pattern, module_base + section_header->VirtualAddress, size);

			if (address != 0) {
				if (rip_relative) {
					address = address + rip_relative_offset;
					address = address + *reinterpret_cast<uint32_t*>(address) + 4;
					// address = address - module_base;
				}

				address = address + offset;

				return address;
			}
		}

		return 0;
	}

}

#endif