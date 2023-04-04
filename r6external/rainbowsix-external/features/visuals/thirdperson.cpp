#include "visuals.hpp"
#include "../../globals.hpp"

void visuals::thirdperson(int value) {
    uintptr_t chain = globals::memory.read<uintptr_t>(globals::fov_manager + 0x28);
    chain = globals::memory.read<uintptr_t>(chain + 0x0);

    if (chain != 0)
        globals::memory.write<int>(chain + 0x48, value);
}