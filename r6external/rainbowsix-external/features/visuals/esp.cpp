#include "visuals.hpp"

#include "../../game/game_util.h"
#include "../../globals.hpp"
#include "../../offsets.hpp"
#include "../../overlay/imgui/imgui.h"

void text_shadowed(ImDrawList* draw_list, ImVec2 position, ImColor color, const char* message) {
	draw_list->AddText(ImVec2(position.x - 1, position.y), ImColor(0, 0, 0, 255), message);
	draw_list->AddText(ImVec2(position.x - 1, position.y + 1), ImColor(0, 0, 0, 255), message);
	draw_list->AddText(ImVec2(position.x, position.y + 1), ImColor(0, 0, 0, 255), message);
	draw_list->AddText(ImVec2(position.x + 1, position.y + 1), ImColor(0, 0, 0, 255), message);
	draw_list->AddText(ImVec2(position.x + 1, position.y), ImColor(0, 0, 0, 255), message);
	draw_list->AddText(ImVec2(position.x + 1, position.y - 1), ImColor(0, 0, 0, 255), message);
	draw_list->AddText(ImVec2(position.x, position.y - 1), ImColor(0, 0, 0, 255), message);
	draw_list->AddText(ImVec2(position.x - 1, position.y - 1), ImColor(0, 0, 0, 255), message);
	draw_list->AddText(ImVec2(position.x, position.y), color, message);
}

void text_dropped(ImDrawList* draw_list, ImVec2 position, ImColor color, const char* message) {
	draw_list->AddText(ImVec2(position.x + 1, position.y), ImColor(0, 0, 0, 255), message);
	draw_list->AddText(ImVec2(position.x, position.y + 1), ImColor(0, 0, 0, 255), message);
	draw_list->AddText(ImVec2(position.x + 1, position.y + 1), ImColor(0, 0, 0, 255), message);

	draw_list->AddText(ImVec2(position.x, position.y), color, message);
}

void visuals::esp() {
	if (!game::in_match() || game::get_profile() == 0)
		return;

	entity local_player = game::get_local_entity();

	if (local_player == 0)
		return;

	uintptr_t entity_list = globals::memory.read<uintptr_t>(globals::game_manager + offsets::game_manager_entity_list);

	if (entity_list == 0)
		return;

	uint8_t entity_count = globals::memory.read<uint8_t>(globals::game_manager + offsets::game_manager_entity_count);

	if (entity_count == 0)
		return;

	ImDrawList* draw_list = ImGui::GetForegroundDrawList();

	for (int i = 0; i < entity_count; i++) {
		entity entity_object = globals::memory.read<uintptr_t>(entity_list + i * offsets::game_manager_entity);
		
		if (entity_object.get_obj() == 0)
			continue;

		if (entity_object == local_player)
			continue;

		if (local_player.get_team() == entity_object.get_team())
			continue;

		int health = entity_object.get_health();
		if (health <= 0)
			continue;

		// calculations
		vec3_t head_pos = entity_object.get_bone_pos(0xfd0); if (head_pos == vec3_t()) continue;
		vec3_t feet_pos = entity_object.get_bone_pos(0x700); if (feet_pos == vec3_t()) continue;
		vec3_t box_top_pos = entity_object.get_bone_pos(0xfd0) + vec3_t(0.f, 0.f, 0.2f); if (box_top_pos == vec3_t()) continue;

		vec3_t head_position = game::w2s(head_pos); if (head_position == vec3_t()) continue; if (head_position.z < 0.1f) continue;
		vec3_t feet_position = game::w2s(feet_pos); if (feet_position == vec3_t()) continue; if (feet_position.z < 0.1f) continue;
		vec3_t box_top = game::w2s(box_top_pos); if (box_top == vec3_t()) continue; if (box_top.z < 0.1f) continue;

		float box_height = feet_position.y - box_top.y;
		float box_width = box_height / 2.4f;

		// bad attempt at offscreen esp
		/*if (head_position.z < 0.f || feet_position.z < 0.f || box_top.z < 0.f) {
			draw_list->AddLine(ImVec2(globals::window_horizontal_size / 2.f, globals::window_vertical_size / 2.f), ImVec2(box_top.x, box_top.y + box_height), ImColor(255, 255, 0, 150));
			float angle = atan2(globals::window_vertical_size / 2.f - box_top.y + box_height, globals::window_horizontal_size / 2.f - box_top.x);
			draw_list->AddText(ImVec2(box_top.x, box_top.y + box_height), ImColor(255, 255, 255, 255), std::to_string(angle).c_str());
			continue;
		}*/

		// tracers (who likes this ugly shit?)
		//draw_list->AddLine(ImVec2(globals::window_horizontal_size / 2.f, (float)globals::window_vertical_size), ImVec2(box_top.x, box_top.y + box_height), ImColor(255, 255, 0, 150));

		// box esp
		draw_list->AddRect(ImVec2(box_top.x - box_width / 2 - 1, box_top.y - 1), ImVec2(box_top.x - box_width / 2 + box_width + 1, box_top.y + box_height + 1), ImColor(0, 0, 0, 100), 2.f);
		draw_list->AddRect(ImVec2(box_top.x - box_width / 2, box_top.y), ImVec2(box_top.x - box_width / 2 + box_width, box_top.y + box_height), ImColor(192, 56, 107, 255), 2.f); // 192, 56, 107
		draw_list->AddRect(ImVec2(box_top.x - box_width / 2 + 1, box_top.y + 1), ImVec2(box_top.x - box_width / 2 + box_width - 1, box_top.y + box_height - 1), ImColor(0, 0, 0, 100), 2.f);
		draw_list->AddRectFilled(ImVec2(box_top.x - box_width / 2 + 1, box_top.y + 1), ImVec2(box_top.x - box_width / 2 + box_width - 1, box_top.y + box_height - 1), ImColor(0, 0, 0, 15), 2.f);
		continue;

		// health bar
		float max_health = 120.f;
		if (globals::memory.read<byte>(entity_object.get_obj() + 0x58) == 0) // if bot max is 100
			max_health = 100.f;

		float g = 255.f * health / 120.f;
		float r = 255.f - g;
		ImColor fill = ImColor((int)r, (int)g, 10, 255);
		draw_list->AddRect(ImVec2(box_top.x - box_width / 2 - 7, box_top.y), ImVec2(box_top.x - box_width / 2 - 3, box_top.y + box_height), ImColor(0, 0, 0, 150));
		draw_list->AddRectFilled(ImVec2(box_top.x - box_width / 2 - 6, box_top.y + 1), ImVec2(box_top.x - box_width / 2 - 4, box_top.y + box_height - 1), ImColor(0, 0, 0, 100));
		draw_list->AddRectFilled(ImVec2(box_top.x - box_width / 2 - 6, box_top.y + box_height - health / max_health * box_height + 1), ImVec2(box_top.x - box_width / 2 - 4, box_top.y + box_height - 1), fill);

		// health bar number
		if (health < max_health) {
			std::string health_string = std::to_string(health);
			text_shadowed(draw_list, ImVec2(box_top.x - box_width / 2 - 7 - ImGui::CalcTextSize(health_string.c_str()).x, box_top.y + box_height - health / max_health * box_height - ImGui::CalcTextSize(health_string.c_str()).y / 2), ImColor(255, 255, 255, 255), health_string.c_str());
		}

		// head circle kinda want to make it look like estrogen
		float radius = box_height / 12.5f;
		draw_list->AddCircle(ImVec2(head_position.x, head_position.y), radius, ImColor(255, 0, 255, 125));
		draw_list->AddCircleFilled(ImVec2(head_position.x, head_position.y), radius, ImColor(0, 0, 0, 60));

		// distance display
		/*std::string distance = std::to_string((int)local_player.get_bone_pos(0x700).dist_to(entity_object.get_bone_pos(0x700)));
		distance += "m";
		text_shadowed(draw_list, ImVec2(box_top.x - ImGui::CalcTextSize(distance.c_str()).x / 2, box_top.y + box_height + 1), ImColor(255, 255, 255, 255), distance.c_str());*/

		// character information
		std::string character = "";
		if (globals::memory.read<byte>(entity_object.get_obj() + 0x58) == 0)
			character = "TERRORIST"; // all bots are terrorist right?
		else {
			if (health < 20)
				text_shadowed(draw_list, ImVec2(box_top.x - box_width / 2 + box_width + 3, box_top.y), ImColor(50, 158, 240, 255), "DOWNED"); // downed flag todo: make proper flag shit l8r

			character = entity_object.get_operator_name();
		}
		
		text_shadowed(draw_list, ImVec2(box_top.x - ImGui::CalcTextSize(character.c_str()).x / 2, box_top.y - 1 - ImGui::CalcTextSize(character.c_str()).y), ImColor(255, 255, 255, 255), character.c_str());

		// name esp
		/*std::string username = "TEST";
		if (globals::memory.read<byte>(entity_object.get_obj() + 0x58) == 0)
			username = "BOT";
		else
			username = entity_object.get_username();

		text_shadowed(draw_list, ImVec2(box_top.x - ImGui::CalcTextSize(username.c_str()).x / 2, box_top.y + box_height + 1), ImColor(255, 255, 255, 255), username.c_str());*/

		/*int ctu = entity_object.get_ctu();
		int hero = entity_object.get_hero();
		for (int d = 1; d < 15; d++) {
			vec3_t bone_pos = entity_object.get_bone_pos(entity_object.get_bone(ctu, hero, d)); if (bone_pos == vec3_t()) continue;
			vec3_t bone_position = game::w2s(bone_pos); if (bone_position == vec3_t()) continue; if (bone_position.z < 0.1f) continue;

			draw_list->AddCircle(ImVec2(bone_position.x, bone_position.y), 3, ImColor(255, 255, 0, 255));
		}*/
	}
	return;

	// doesn't work currently
	uintptr_t gadget_list = globals::memory.read<uintptr_t>(globals::network_manager + 0x30);
	gadget_list = globals::memory.read<uintptr_t>(gadget_list + 0x4F8);
	gadget_list = globals::memory.read<uintptr_t>(gadget_list + 0x10);
	gadget_list = globals::memory.read<uintptr_t>(gadget_list + 0xC0);

	if (gadget_list == 0)
		return;

	uint8_t gadget_count = globals::memory.read<uint8_t>(gadget_list + 0x0090);

	if (gadget_count == 0) {
		draw_list->AddText(ImVec2(400, 400), ImColor(255, 255, 255, 255), "null");
		return;
	}

	draw_list->AddText(ImVec2(400, 400), ImColor(255, 255, 255, 255), std::to_string(gadget_count).c_str());
	
	for (int i = 0; i < gadget_count; i++) {
		// mby go make a class for gadets l8r you lazy fuck
		uintptr_t gadget_object = globals::memory.read<uintptr_t>(gadget_list + i * 0x8);
		if (gadget_object == 0)
			continue;

		vec3_t gadget_pos = globals::memory.read<vec3_t>(gadget_object + 0x0030); if (gadget_pos == vec3_t()) continue;
		vec3_t gadget_w2s = game::w2s(gadget_pos); if (gadget_w2s == vec3_t()) continue; if (gadget_w2s.z < 0.1f) continue;

		text_shadowed(draw_list, ImVec2(gadget_w2s.x, gadget_w2s.y), ImColor(255, 255, 255, 255), "GADGET");
		// 0x0030 = position
	}
}