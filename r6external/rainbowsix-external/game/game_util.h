#ifndef _GAME_GAME_UTIL_HPP_
#define _GAME_GAME_UTIL_HPP_

#include <string>

#include "../util/math/vector.hpp"

class entity {
public:
	entity(uintptr_t obj);
	bool operator ==(const entity& other);

	uintptr_t get_obj();

	int get_team();
	void set_team(int value);
	int get_health();

	int get_ctu();
	int get_hero();

	int get_bone(int ctu, int hero, int id);
	std::string get_operator_name();
	std::string get_username();

	vec3_t get_bone_pos(uintptr_t bone);
	vec4_t get_view_angles();
	void set_view_angles(vec4_t angles);
	vec4_t get_gun_angles();
	void set_gun_angles(vec4_t angles);
	void aim_at_entity(entity target, uintptr_t bone);
	int is_firing();

private:
	uintptr_t _obj;
};

namespace game {
	bool in_match();
	int gamestate();
	int state();

	entity get_local_entity();

	uint64_t get_profile();

	bool update_addresses();

	vec3_t get_angle_to(vec3_t enemy_position, vec3_t local_position);
	void clamp_angles(vec3_t& angle);
	vec4_t create_from_yaw_pitch_roll(float yaw, float pitch, float roll);
	bool skip_bad_angle(vec4_t target_angle, vec4_t local_angle);

	bool update_display_size();
	bool update_view_translation();
	vec3_t w2s(vec3_t position);
}

#endif