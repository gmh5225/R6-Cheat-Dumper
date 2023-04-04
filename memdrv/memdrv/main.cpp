#include "defs.hpp"

#include "hooks/hooks.hpp"
#include "util/clean.hpp"

NTSTATUS DriverEntry(PDRIVER_OBJECT driverObject, PUNICODE_STRING registryPath) {
	UNREFERENCED_PARAMETER(driverObject);
	UNREFERENCED_PARAMETER(registryPath);

	DbgPrint("[+] hooking!");

	if (!hooks::init())
		return STATUS_FAILED_DRIVER_ENTRY;

	util::clean_cache();

	DbgPrint("[+] hooked!");

	return STATUS_SUCCESS;
}