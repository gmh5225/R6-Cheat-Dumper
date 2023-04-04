#include "driver.hpp"

#include <Windows.h>
#include <winioctl.h>
#include <winternl.h>

#pragma comment(lib, "ntdll.lib")

namespace driver {
	void* _handle = nullptr;

	constexpr auto SYSCALL_UNIQUE = 0x133;

	struct open_handle_data {
		uint32_t pid;
		uint64_t handle;
	};

	struct read_write_data {
		uint64_t handle;
		uint64_t address;
		uint64_t buffer;

		uint32_t size;
	};

	struct virtual_protect_data {
		uint64_t handle;
		uint64_t address;
		uint32_t size;
		uint32_t protection;
		uint32_t old_protection;
	};

	struct get_module_base_data {
		uint64_t handle;
		uint64_t dllname;
		uint32_t dllname_length;

		uint64_t module_base;
	};

	enum struct DriverCall {
		IsLoaded,
		OpenProcess,
		ReadProcessMemory,
		WriteProcessMemory,
		AllocateVirtualMemory,
		FreeVirtualMemory,
		ProtectVirtualMemory,
		GetModuleBase
	};

	struct syscall_data {
		uint32_t pad_0;
		uint32_t pad_1;
		uint32_t magic;
		uint32_t syscall;
		uint64_t arguments;
	} SYSCALL_DATA, * PSYSCALL_DATA;

	NTSTATUS execute_syscall(DriverCall syscall, void* args) {
		syscall_data data;

		data.pad_0 = 100;
		data.pad_1 = 100;

		data.magic = SYSCALL_UNIQUE;
		data.syscall = static_cast<std::uint32_t>(syscall);
		data.arguments = reinterpret_cast<uint64_t>(args);

		return DeviceIoControl(
			_handle,
			CTL_CODE(FILE_DEVICE_BEEP, 0, METHOD_BUFFERED, FILE_SPECIAL_ACCESS),
			&data,
			sizeof(data),
			&data,
			sizeof(data),
			nullptr,
			nullptr
		);
	}

	bool initialize() {
		UNICODE_STRING device_name;
		OBJECT_ATTRIBUTES attributes;

		RtlInitUnicodeString(&device_name, L"\\Device\\Beep");

		InitializeObjectAttributes(&attributes, &device_name, 0, NULL, NULL);

		IO_STATUS_BLOCK IoStatus;

		if (!NT_SUCCESS(NtCreateFile(&_handle, 0x3, &attributes, &IoStatus, nullptr, 0, 0x3, 0x3, 0, nullptr, 0)))
			return false;

		return _handle != nullptr;
	}

	bool is_driver_loaded() {
		bool status = false;

		(void)execute_syscall(DriverCall::IsLoaded, &status);

		return status;
	}

	uint64_t open_process(uint32_t pid) {
		open_handle_data data = {};

		data.pid = pid;

		(void)execute_syscall(DriverCall::OpenProcess, &data);

		return data.handle;
	}

	bool read_memory(uint64_t handle, uint64_t address, void* buffer, uint32_t size) {
		read_write_data data = {};

		data.handle = handle;
		data.address = address;
		data.buffer = reinterpret_cast<uint64_t>(buffer);
		data.size = size;

		return execute_syscall(DriverCall::ReadProcessMemory, &data) == 1;
	}

	bool write_memory(uint64_t handle, uint64_t address, const void* buffer, uint32_t size) {
		read_write_data data = {};

		data.handle = handle;
		data.address = address;
		data.buffer = reinterpret_cast<uint64_t>(buffer);
		data.size = size;

		return execute_syscall(DriverCall::WriteProcessMemory, &data) == 1;
	}

	bool protect_virtual_memory(uint64_t handle, uint64_t address, uint32_t size, uint32_t protect, uint32_t* old_protect) {
		virtual_protect_data data = {};

		data.handle = handle;
		data.address = address;
		data.size = size;
		data.protection = protect;

		*old_protect = protect;

		return execute_syscall(DriverCall::ProtectVirtualMemory, &data) == 1;
	}

	uint64_t get_module_base(uint64_t handle, const wchar_t* dllname) {
		get_module_base_data data = {};

		data.handle = handle;
		data.dllname = reinterpret_cast<uint64_t>(dllname);
		data.dllname_length = lstrlenW(dllname);

		(void)execute_syscall(DriverCall::GetModuleBase, &data);

		return data.module_base;
	}
}