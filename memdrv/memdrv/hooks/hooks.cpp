#include "hooks.hpp"

#include "../defs.hpp"

#include "../util/memory.hpp"

#include "../routines/routines.hpp"

#include "../util/process.hpp"

#include "../util/hk.hpp"

#include <ndis.h>

#define SYSCALL_UNIQUE (0x133)

namespace hooks {
    using fnBeepDeviceControl = int64_t(__fastcall*)(PDEVICE_OBJECT, PIRP);
    fnBeepDeviceControl oBeepDeviceControl = nullptr;

    using fnEnumerateDebuggingDevicesOriginal = int64_t(__stdcall*)(void*, void*);
    fnEnumerateDebuggingDevicesOriginal oEnumerateDebuggingDevices;

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

    struct dispatch_routine {
        DriverCall id;
        void* dispatch;
        uint32_t argsize;
    };

    dispatch_routine routines[] = {
        {
            DriverCall::IsLoaded,
            &routines::routine_is_loaded,
            sizeof(routines::is_loaded_data)
        },
        {
            DriverCall::OpenProcess,
            &routines::routine_open_handle,
            sizeof(routines::open_handle_data)
        },
        {
            DriverCall::ReadProcessMemory,
            &routines::routine_read_process_memory,
            sizeof(routines::read_write_data)
        },
        {
            DriverCall::WriteProcessMemory,
            &routines::routine_write_process_memory,
            sizeof(routines::read_write_data)
        },
        {
            DriverCall::AllocateVirtualMemory,
            &routines::routine_allocate_virtual_memory,
            sizeof(routines::allocate_virtual_memory_data)
        },
        {
            DriverCall::FreeVirtualMemory,
            &routines::routine_free_virtual_memory,
            sizeof(routines::free_virtual_memory_data)
        },
        {
            DriverCall::ProtectVirtualMemory,
            &routines::routine_protect_virtual_memory,
            sizeof(routines::virtual_protect_data)
        },
        {
            DriverCall::GetModuleBase,
            &routines::routine_get_module_base,
            sizeof(routines::get_module_base_data)
        }
    };

    NTSTATUS __fastcall BeepDeviceControl_Hook(PDEVICE_OBJECT device_object, PIRP irp) {
        const auto execute_dispatch_routine([](void* dispatch, void* args, util::PROCESS_HANDLE source_process, int64_t* status) -> void {
            return static_cast<void(*)(void*, util::PROCESS_HANDLE, int64_t*)>(dispatch)(args, source_process, status);
        });

        /*
         * METHOD_BUFFERED
         *
         *    Input Buffer = Irp->AssociatedIrp.SystemBuffer
         *    Ouput Buffer = Irp->AssociatedIrp.SystemBuffer
         *
         *    Input Size   =  Parameters.DeviceIoControl.InputBufferLength
         *    Output Size  =  Parameters.DeviceIoControl.OutputBufferLength
         *
         *    Since they both use the same location so the "buffer" allocated by the I/O
         *    manager is the size of the larger value (Output vs. Input)
         */

        int64_t status = STATUS_SUCCESS;

        // if (ExGetPreviousMode() != UserMode)
        //     return oBeepDeviceControl(device_object, irp);

        const auto* stack = IoGetCurrentIrpStackLocation(irp);

        if (stack == nullptr)
            return STATUS_ACCESS_VIOLATION;

        auto* input_buffer = static_cast<PSYSCALL_DATA>(irp->AssociatedIrp.SystemBuffer);
        auto* output_buffer = static_cast<PSYSCALL_DATA>(irp->AssociatedIrp.SystemBuffer);

        if (input_buffer != nullptr && output_buffer != nullptr) {
            if (input_buffer->magic != SYSCALL_UNIQUE)
                return oBeepDeviceControl(device_object, irp);

            bool dispatched = false;

            for (auto routine : routines) {
                if (routine.id == static_cast<DriverCall>(input_buffer->syscall)) {
                    void* args = ExAllocatePoolWithTag(PagedPool, routine.argsize, 'OwO');

                    if (!util::safe_memcpy(args, input_buffer->arguments, routine.argsize)) {
                        status = STATUS_ACCESS_VIOLATION;

                        ExFreePoolWithTag(args, 'OwO');
                        dispatched = true;
                        break;
                    }

                    execute_dispatch_routine(routine.dispatch, args, util::encode_handle(reinterpret_cast<uint32_t>(PsGetCurrentProcessId())), &status);

                    if (!util::safe_memcpy(input_buffer->arguments, args, routine.argsize)) {
                        status = STATUS_ACCESS_VIOLATION;
                    }

                    ExFreePoolWithTag(args, 'OwO');
                    dispatched = true;
                    break;
                }
            }

            if (!dispatched)
                status = STATUS_NOT_IMPLEMENTED;
        }

        // KfRaiseIrql(DISPATCH_LEVEL);

        irp->IoStatus.Status = status;
        irp->IoStatus.Information = 0;

        if (status != 0) {
            return oBeepDeviceControl(device_object, irp);
        }

        IoCompleteRequest(irp, 0);

        return status;
    }

    bool init() {
        const uintptr_t base = util::get_module_base("beep.sys");

        if (base == 0)
            return false;

        // setup hook
        // 40 53 48 83 EC 20 4C 8B 82 ? ? ? ?
        const auto func = util::find_pattern_image(base, "\x40\x53\x48\x83\xEC\x20\x4C\x8B\x82\x00\x00\x00\x00", "xxxxxxxxx????");

        if (func == 0)
            return false;

        HkDetourFunction(reinterpret_cast<void*>(func), &BeepDeviceControl_Hook, 25, reinterpret_cast<void**>(&oBeepDeviceControl));

        return true;
    }

    bool free() {
        HkRestoreFunction(&BeepDeviceControl_Hook, oBeepDeviceControl);

        return true;
    }
}