#include "movement.hpp"
#include "../../globals.hpp"
#include "../../offsets.hpp"

void movement::noclip(float value) {
	uintptr_t chain = globals::memory.read<uintptr_t>(globals::network_manager + 0xF8);
	chain = globals::memory.read<uintptr_t>(chain + 0x8);

	if (chain != 0) { // todo read this shit normally so i can unset it
		globals::memory.write<float>(chain + 0x7F0, -1.0f);
		globals::memory.write<float>(chain + 0x7F4, -1.0f);
		globals::memory.write<float>(chain + 0x7F8, -1.0f);
		globals::memory.write<float>(chain + 0x7FC, -1.0f);
	}
	
	/*uintptr_t chain = globals::memory.read<uintptr_t>(offsets::module_base + 0x53903c0); // noclip when prone
	chain = globals::memory.read<uintptr_t>(chain + 0xF8);
	chain = globals::memory.read<uintptr_t>(chain + 0x8);
	
	if (chain != 0)
		globals::memory.write<float>(chain + 0x560, value);*/
}


void movement::nocollision(float value) {
	uintptr_t chain = globals::memory.read<uintptr_t>(globals::network_manager + 0xF8);
	chain = globals::memory.read<uintptr_t>(chain + 0x8);

	if (chain != 0)
		globals::memory.write<float>(chain + 540, value);
}
