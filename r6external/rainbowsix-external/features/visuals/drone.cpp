#include <thread>
#include "visuals.hpp"
#include "../../game/game_util.h"
#include "../../globals.hpp"

bool visuals::state = false;
int visuals::real_team = 0;

void visuals::drone(bool enabled) {
	/*
	note to self: i talked to some dude who said he was able to get outline
	and the drone esp to show perfectly if he only changes it back to the
	real team after prep phase
	*/
	
	entity local_player = game::get_local_entity();

	if (local_player.get_obj() == 0)
		return;

	const int game_state = game::state();
	
	if (game_state == 4 && local_player.get_team() == 3) { // when on operator selection screen
		if (!state) {
			state = true;
			real_team = local_player.get_team();
		}
		if (state)
			local_player.set_team(real_team == 3 ? 4 : 3);
	}
	else if (state && game_state == 6) { // when team is switched from real and game has started
		// set outline color to whatever you would like
		uintptr_t chain = globals::memory.read<uintptr_t>(globals::game_manager + 0x250);
		chain = globals::memory.read<uintptr_t>(chain + 0xBB8);

		if (chain != 0) {
			globals::memory.write<float>(chain + 0x48, 1.f); // red
			globals::memory.write<float>(chain + 0x48 + 0x4, 0.f); // blue
			globals::memory.write<float>(chain + 0x48 + 0x8, 0.f); // green
			globals::memory.write<float>(chain + 0x48 + 0xC, 1.f); // alpha
		}
		
		if (real_team == 3) { // attacker
			while (local_player.get_health() <= 0) // causes me to be unable to spawn in when defender but works 100% of the time when attacker
				std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}
		if (real_team == 4) { // defender
			state = false; // don't do shit when defender for time being because the lobby freezes when you try spoofing team as a defender
			return;
			//std::this_thread::sleep_for(std::chrono::seconds(3)); // very inconsistent usage
		}
		local_player.set_team(real_team);
		Beep(500, 500);
		state = false;
	}
	else if (state && game_state == 0)
		state = false;
}