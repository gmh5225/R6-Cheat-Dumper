#include "combat.hpp"
#include "../../globals.hpp"
#include "../../offsets.hpp"
#include "../../game/game_util.h"

enum fire_type {
	full_auto = 0,
	burst2 = 1,
	burst3 = 2,
	single = 3
};

void combat::rapid_fire(bool enabled) { // write_memory<int>(chain + 0x011A, 1); // wiggles gun
	if (!enabled)
		return;
	
	uintptr_t chain = globals::memory.read<uintptr_t>(game::get_local_entity().get_obj() + 0x90);
	chain = globals::memory.read<uintptr_t>(chain + 0xC8);

	if (chain != 0)
		globals::memory.write<int>(chain + 0x108, fire_type(full_auto)); // rapid fire
}