#include "combat.hpp"
#include "../../globals.hpp"
#include "../../offsets.hpp"

void combat::norecoil(byte recoil, float horizontal_recoil, float vertical_recoil, float spread) {
	uintptr_t chain = globals::memory.read<uintptr_t>(globals::module_base + offsets::fov_manager); // perfect no recoil but makes it weird when aiming
	chain = globals::memory.read<uintptr_t>(chain + 0x110);
	chain = globals::memory.read<uintptr_t>(chain + 0x0);
	if (chain != 0)
		globals::memory.write<byte>(chain + 0xE2D, recoil);

	chain = globals::memory.read<uintptr_t>(globals::game_manager + 0xC8); // no recoil that fucks up a bit vertically but feels normal when aiming
	chain = globals::memory.read<uintptr_t>(chain + 0x0);
	chain = globals::memory.read<uintptr_t>(chain + 0x90);
	chain = globals::memory.read<uintptr_t>(chain + 0xC8);
	chain = globals::memory.read<uintptr_t>(chain + 0x278);
	
	if (chain != 0) {
		globals::memory.write<byte>(chain + 0x168, 0);
		globals::memory.write<float>(chain + 0x14C, horizontal_recoil);
		globals::memory.write<float>(chain + 0x15C, vertical_recoil);
		globals::memory.write<float>(chain + 0x58, spread);
	}
}
