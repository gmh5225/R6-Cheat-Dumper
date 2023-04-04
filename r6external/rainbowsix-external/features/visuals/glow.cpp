#include "visuals.hpp"
#include "../../globals.hpp"

void visuals::glow(float red, float green, float blue, float alpha, float opacity, float distance) {
	uintptr_t chain = globals::memory.read<uintptr_t>(globals::glow_manager + 0xB8);
	
	if (chain != 0) {
		globals::memory.write<float>(chain + 0xD0, red);
		globals::memory.write<float>(chain + 0xD4, green);
		globals::memory.write<float>(chain + 0xD8, blue);
		globals::memory.write<float>(chain + 0x118, alpha);
		globals::memory.write<float>(chain + 0x11C, opacity);
		globals::memory.write<float>(chain + 0x110, distance); // other
		globals::memory.write<float>(chain + 0x110 + 0x4, 1.f); // local
	}
}
