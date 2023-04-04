#ifndef _GLOBALS_HPP_
#define _GLOBALS_HPP_

#include <iostream>
#include <memory>

#include "game/game_util.h"
#include "util/math/vector.hpp"
#include "util/memory.hpp"

namespace globals {
	extern util::memory memory;
	
	extern uint32_t game_pid;
	extern uintptr_t module_base;

	extern bool w2s_good;
	extern entity targeted_entity;

	extern uintptr_t game_manager;
	extern uintptr_t game_manager;
	extern uintptr_t game_profile;
	extern uintptr_t network_manager;
	extern uintptr_t input_manager;
	extern uintptr_t round_manager;
	extern uintptr_t glow_manager;
	extern uintptr_t fov_manager;
	extern uintptr_t freeze_manager;

	extern uintptr_t entity_list;
	extern uintptr_t render;
	extern uintptr_t game_render;
	extern uintptr_t engine_link;
	extern uintptr_t engine;
	extern uintptr_t camera;
	extern uintptr_t vtable;

	extern int window_horizontal_size;
	extern int window_vertical_size;

	extern vec3_t camera_view_right;
	extern vec3_t camera_view_up;
	extern vec3_t camera_view_forward;
	extern vec3_t camera_view_translation;
	extern float camera_view_fovx;
	extern float camera_view_fovy;
}

#endif