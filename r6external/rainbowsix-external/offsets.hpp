#ifndef _OFFSETS_HPP_
#define _OFFSETS_HPP_

#include <iostream>

namespace offsets {
	constexpr uintptr_t game_manager = 0x53a6868;
	constexpr uintptr_t game_profile = 0x5393ae8;
	constexpr uintptr_t round_manager = 0x5396138;
	constexpr uintptr_t network_manager = 0x5393ac0;
	constexpr uintptr_t input_manager = 0x5393960;
	constexpr uintptr_t glow_manager = 0x6089468;
	constexpr uintptr_t fov_manager = 0x53a6860;
	constexpr uintptr_t freeze_manager = 0x538fc90;

	// game -> game_profile ->
	constexpr uintptr_t game_profile_chain1 = 0x68;
	constexpr uintptr_t game_profile_chain2 = 0x0;
	constexpr uintptr_t game_profile_chain3 = 0x130;
	constexpr uintptr_t game_profile_chain4 = 0x420;

	// game -> game_profile -> camera ->
	constexpr uintptr_t camera_view_right = 0x7C0;
	constexpr uintptr_t camera_view_up = 0x7D0;
	constexpr uintptr_t camera_view_forward = 0x7E0;
	constexpr uintptr_t camera_view_translation = 0x7F0;
	constexpr uintptr_t camera_view_fovx = 0x800;
	constexpr uintptr_t camera_view_fovy = 0x814;

	// game -> game_manager ->
	constexpr uintptr_t vtable = 0x3a87220;
	constexpr uintptr_t game_manager_entity_list = 0xC8;
	constexpr uintptr_t game_manager_entity = 0x8; // size
	constexpr uintptr_t game_manager_entity_count = 0xD0;

	// game -> game_manager -> entity_list -> index -> entity ->
	constexpr uintptr_t entity_entity_info = 0x28;
	constexpr uintptr_t entity_info_main_component = 0xD8; // entity_info ->
	constexpr uintptr_t main_component_child_component = 0x8; // main_component ->
	constexpr uintptr_t child_component_health_int = 0x168; // child_component ->
	constexpr uintptr_t child_component_health_float = 0x238; // child_component ->

	// game -> game_manager -> entity_list -> index ->
	constexpr uintptr_t entity_ref = 0x20;
	constexpr uintptr_t entity_forehead = 0x6B0;
	constexpr uintptr_t entity_head = 0x6A0;
	constexpr uintptr_t entity_neck = 0x1050;
	constexpr uintptr_t entity_right_hand = 0x6A0;
	constexpr uintptr_t entity_chest = 0x1010;
	constexpr uintptr_t entity_stomach = 0xF90;
	constexpr uintptr_t entity_pelvis = 0xFD0;
	constexpr uintptr_t entity_feet = 0x700;
}

#endif