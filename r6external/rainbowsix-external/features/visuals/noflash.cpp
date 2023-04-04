#include "visuals.hpp"
#include "../../globals.hpp"

void visuals::noflash(byte value) {
	uintptr_t chain = globals::memory.read<uintptr_t>(globals::game_profile + 0x78);
	chain = globals::memory.read<uintptr_t>(chain + 0x0);
	chain = globals::memory.read<uintptr_t>(chain + 0x28);
	chain = globals::memory.read<uintptr_t>(chain + 0x30);
	chain = globals::memory.read<uintptr_t>(chain + 0x30);
	chain = globals::memory.read<uintptr_t>(chain + 0x28);
	
	if (chain != 0)
		globals::memory.write<byte>(chain + 0x40, value);
}
