#include "../../config.hpp"
#include "../../game/game_util.h"
#include "combat.hpp"
#include "../../globals.hpp"
#include "../../offsets.hpp"

void combat::aimbot() {
	if (!game::in_match() || !config::aimbot_enabled || !globals::w2s_good)
		return; // no in game / aimbot disabled

	entity local_entity = game::get_local_entity();

	if (local_entity.get_obj() == 0)
		return;

	if (local_entity.get_health() <= 0)
		return;

	if (!config::aimbot_silent_enabled) {
		if (local_entity.is_firing() != 1) {
			if (local_entity.is_firing() != 3)
				norecoil(1, 0.25f, 0.25f, 0.5f);
			globals::targeted_entity = 0;
			return;// your not shooting your gun so why tf would i aim
		}
	}
	
	uintptr_t entity_list = globals::memory.read<uintptr_t>(globals::game_manager + offsets::game_manager_entity_list);

	if (entity_list == 0)
		return; // should aways be find but why not make sure
	
	uint8_t entity_count = globals::memory.read<uint8_t>(globals::game_manager + offsets::game_manager_entity_count);

	if (entity_count == 0)
		return; // if entity list is fine this should be good

	entity closest_entity = NULL;
	float closest_bone_dist = config::aimbot_fov;
	if (config::aimbot_silent_enabled)
		closest_bone_dist = 1000000;
	int bone = 1; // set to 1 in case it somehow bugs out

	int i = 0;
	for (; i < entity_count; i++) {
		entity entity_object = globals::memory.read<uintptr_t>(entity_list + i * offsets::game_manager_entity);
		if (entity_object.get_obj() == 0)
			continue;

		if (entity_object == local_entity)
			continue;

		if (local_entity.get_team() == entity_object.get_team())
			continue;

		int closest_bone = 0;
		for (int b = 1; b <= 1 /*set to amount of bones at get_bone ha its set to 1 rn to target only head*/; b++) {
			vec3_t bone_screen_pos = game::w2s(entity_object.get_bone_pos(0xfd0));

			if (bone_screen_pos == vec3_t())
				continue;

			if (bone_screen_pos.z < 1.f && !config::aimbot_silent_enabled)
				continue;
			
			const float bone_distance = abs(bone_screen_pos.dist_to(vec3_t(globals::window_horizontal_size / 2.f, globals::window_vertical_size / 2.f, bone_screen_pos.z)));
			if (bone_distance < closest_bone_dist) {
				closest_bone_dist = bone_distance;
				closest_bone = b;
			}
		}

		if (closest_bone != 0) { // should be less retarded way of doing it
			closest_entity = entity_object;
			bone = closest_bone;
		}
	}
	
	if (closest_entity.get_obj() != 0) {
		if (closest_entity.get_obj() != globals::targeted_entity.get_obj())
			globals::targeted_entity = closest_entity;
		norecoil(0, 0.f, 0.f, 0.f);
		local_entity.aim_at_entity(closest_entity, 0xfd0);
	}
	else { // why not set smoothing value and shit back just in the case something fucked up :)
		// there is no target so do what you wish instead
		globals::targeted_entity = 0;
	}
}