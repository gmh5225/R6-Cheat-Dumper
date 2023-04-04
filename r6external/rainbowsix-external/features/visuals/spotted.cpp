#include "visuals.hpp"
#include "../../offsets.hpp"
#include "../../globals.hpp"
#include "../../game/game_util.h"

void enabled_marker(uintptr_t entity_object, byte enable) {
	uintptr_t chain = globals::memory.read<uintptr_t>(entity_object + offsets::entity_entity_info);
	
	if (chain == 0)
		return;

	chain = globals::memory.read<uintptr_t>(chain + offsets::entity_info_main_component);
	
	if (chain == 0)
		return;

	for (auto current_component = 0x80; current_component < 0xF0; current_component += sizeof(std::uintptr_t)) {
		uintptr_t entity_chain = globals::memory.read<uintptr_t>(chain + current_component);
		if (entity_chain == 0)
			continue;

		if (globals::memory.read<uintptr_t>(entity_chain) != globals::module_base + offsets::vtable)
			continue;

		globals::memory.write<byte>(entity_chain + 0x552, enable);
		globals::memory.write<byte>(entity_chain + 0x554, enable);
	}
}

void visuals::spotted(bool enabled) {
	uintptr_t entity_list = globals::memory.read<uintptr_t>(globals::game_manager + offsets::game_manager_entity_list);

	if (entity_list == 0)
		return;
	
	uint8_t entity_count = globals::memory.read<uint8_t>(globals::game_manager + offsets::game_manager_entity_count);

	if (entity_count == 0)
		return;

	entity local_player = game::get_local_entity();

	if (local_player.get_obj() == 0) // if local is invalid
		return;
	
	for (int i = 0; i < entity_count; i++) {
		entity player = entity(globals::memory.read<uintptr_t>(entity_list + i * offsets::game_manager_entity));
		if (player.get_obj() == 0)
			continue;

		if (player == local_player)
			continue;

		if (player.get_team() == local_player.get_team())
			continue;
		
		enabled_marker(player.get_obj(), enabled ? 1 : 0);
	}
}