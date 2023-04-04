#include "visuals.hpp"
#include "../../offsets.hpp"
#include "../../globals.hpp"

void visuals::gravity(bool enable) {
	uintptr_t chain = globals::memory.read<uintptr_t>(globals::network_manager); // network manager
	chain = globals::memory.read<uintptr_t>(chain + 0xF8); // network manager connection
	chain = globals::memory.read<uintptr_t>(chain + 0x8); // network manager connection game

	if (chain != 0)
		globals::memory.write<float>(chain + 0x760, enable ? 1.5f : 0.f);
}
