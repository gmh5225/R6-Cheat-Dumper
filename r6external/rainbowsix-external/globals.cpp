#include "globals.hpp"

namespace globals {
	uint32_t game_pid = 0;
	uintptr_t module_base = 0;

	util::memory memory;

	bool w2s_good = true; // fuck this shit
	entity targeted_entity = 0;

	uintptr_t game_manager;
	uintptr_t game_profile;
	uintptr_t round_manager;
	uintptr_t network_manager;
	uintptr_t input_manager;
	uintptr_t glow_manager;
	uintptr_t fov_manager;
	uintptr_t freeze_manager;

	uintptr_t entity_list;
	uintptr_t render;
	uintptr_t game_render;
	uintptr_t engine_link;
	uintptr_t engine;
	uintptr_t camera;
	uintptr_t vtable;

	int window_horizontal_size;
	int window_vertical_size;

	vec3_t camera_view_right;
	vec3_t camera_view_up;
	vec3_t camera_view_forward;
	vec3_t camera_view_translation;
	float camera_view_fovx;
	float camera_view_fovy;
}
