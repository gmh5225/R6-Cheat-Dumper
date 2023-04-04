#include "movement.hpp"
#include "../../offsets.hpp"
#include "../../globals.hpp"

void movement::speed(int speed) {
	uintptr_t chain = globals::memory.read<uintptr_t>(globals::game_manager + 0xC8);
	chain = globals::memory.read<uintptr_t>(chain + 0x0);
	chain = globals::memory.read<uintptr_t>(chain + 0x30);
	chain = globals::memory.read<uintptr_t>(chain + 0x30);
	chain = globals::memory.read<uintptr_t>(chain + 0x38);
	
	if (chain != 0)
		globals::memory.write<int>(chain + 0x58, speed);
}
