#include "visuals.hpp"
#include "../../offsets.hpp"
#include "../../globals.hpp"

void visuals::player_fov(float fov) {
	uintptr_t chain = globals::memory.read<uintptr_t>(globals::fov_manager + 0x28);
	chain = globals::memory.read<uintptr_t>(chain + 0x0);
	if (chain != 0)
		globals::memory.write<float>(chain + 0x38, fov);
}

void visuals::gunmodel_fov(float fov) {
	uintptr_t chain = globals::memory.read<uintptr_t>(globals::fov_manager + 0x28);
	chain = globals::memory.read<uintptr_t>(chain + 0x0);
	if (chain != 0)
		globals::memory.write<float>(chain + 0x3C, fov);
}
