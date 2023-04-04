#include "overlay.hpp"

#include "minhook/MinHook.h"

#include "renderer/renderer.hpp"

#include "input/input.hpp"

#include "ck.hpp"

#include <mutex>
#include <d3d11.h>

#include "../features/visuals/visuals.hpp"
#include "../globals.hpp"
#include "imgui/font.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_impl_win32.h"

static bool _menu_drawn = false;

namespace overlay {
	static uintptr_t _CHwFullScreenRenderTarget_Present = 0;
	static uintptr_t _CHwFullScreenRenderTarget_SwapChainBase_Offset = 0;
	static uintptr_t _CDWMSwapChain_DxgiSwapChain_Offset = 0;

	static uintptr_t o_CHwFullScreenRenderTarget_Present = 0;

	using fn_CHwFullScreenRenderTarget_Present = __int64(__fastcall*)(class CHwFullScreenRenderTarget* _this, intptr_t a2, int8_t a3, const struct RenderTargetPresentParameters* a4);

	bool find_target_present() {
		auto address = find_pattern_module("dwmcore.dll", "E8 ? ? ? ? 8B D8 85 C0 78 27 44 38 3D ? ? ? ?");

		//address = address + 1;
		//address = address + *reinterpret_cast<uint32_t*>(address) + 5;
		//address = address - 0x100000000;

		address += static_cast<uintptr_t>(*reinterpret_cast<uint32_t*>(address + 0x1)) + 0x5;
		address -= 0x100000000;

		_CHwFullScreenRenderTarget_Present = address;
		_CHwFullScreenRenderTarget_SwapChainBase_Offset = *reinterpret_cast<uint8_t*>(address + 223 + 3);

		return _CHwFullScreenRenderTarget_Present != 0;
	}

	bool find_swap_chain_offset() {
		const auto address = find_pattern_module("dwmcore.dll", "40 53 48 83 EC 30 48 8B 89 ? ? ? ? 48 8B 01");

		_CDWMSwapChain_DxgiSwapChain_Offset = *reinterpret_cast<uint32_t*>(address + 9);

		return _CDWMSwapChain_DxgiSwapChain_Offset != 0;
	}

	int64_t __fastcall CHwFullScreenRenderTarget_Present_Hook(CHwFullScreenRenderTarget* _this, uint64_t a2, int8_t a3, RenderTargetPresentParameters* a4) {
		if (!renderer::initialized()) {
			const auto swap_chain_base = *reinterpret_cast<void**>(reinterpret_cast<uint8_t*>(_this) + _CHwFullScreenRenderTarget_SwapChainBase_Offset);

			if (swap_chain_base != nullptr) {
				auto swap_chain = *reinterpret_cast<IDXGISwapChain**>(static_cast<uint8_t*>(swap_chain_base) + _CDWMSwapChain_DxgiSwapChain_Offset);

				ID3D11Device* device = nullptr;

				if (swap_chain != nullptr) {
					if (swap_chain->GetDevice(__uuidof(ID3D11Device), reinterpret_cast<void**>(&device)) == S_OK) {
						renderer::create(device, swap_chain);

						ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(hck_compressed_data, hck_compressed_size, 8);
						Beep(500, 500);
					}
				}
			}
		}

		// draw here
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		if (game::update_display_size() && game::update_view_translation())
			visuals::esp();

		static bool menu_open = true;
		if (input::key_down(VK_INSERT)) {
			menu_open = !menu_open;
			if (globals::input_manager != 0) {
				if (menu_open)
					globals::memory.write<byte>(globals::input_manager + 0x79, 1);
				else
					globals::memory.write<byte>(globals::input_manager + 0x79, 0);
			}
		}

		if (menu_open) {
			ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Once);
			ImGui::Begin("rainbow-external###notifcation_window", nullptr, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
			if (!globals::memory.valid())
				ImGui::Text("waiting for game %c", "|/-\\"[(int)(ImGui::GetTime() / 0.2f) & 3]);
			else {
				ImGui::Text("game state %d", game::state());

				entity local_player = game::get_local_entity();
				ImGui::Text("local health %d", local_player.get_health());
				ImGui::Text("local firing %d", local_player.is_firing());
				ImGui::Text("local team %d", local_player.get_team());

				// drone / outline info
				ImGui::Text("spoofing %s", visuals::state ? "true" : "false");
				ImGui::Text("real team %d", visuals::real_team);
			}
			ImGui::End();
		}

		// dogshit crosshair code
		/*ImDrawList* foreground = ImGui::GetForegroundDrawList();

		foreground->AddLine(ImVec2(globals::window_horizontal_size / 2.f - 15.f, globals::window_vertical_size / 2.f), ImVec2(globals::window_horizontal_size / 2.f + 15.f, globals::window_vertical_size / 2.f), ImColor(255, 255, 255, 255));
		foreground->AddLine(ImVec2(globals::window_horizontal_size / 2.f, globals::window_vertical_size / 2.f - 15.f), ImVec2(globals::window_horizontal_size / 2.f, globals::window_vertical_size / 2.f + 15.f), ImColor(255, 255, 255, 255));*/

		// rendering
		ImGui::Render();

		renderer::context->OMSetRenderTargets(1, &renderer::target_view, nullptr);

		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

		return reinterpret_cast<fn_CHwFullScreenRenderTarget_Present>(o_CHwFullScreenRenderTarget_Present)(_this, a2, a3, a4);
	}

	void enable() {
		if (!find_target_present() || !find_swap_chain_offset())
			exit(1);

		input::enable();

		MH_Initialize();

		MH_CreateHook(reinterpret_cast<void*>(_CHwFullScreenRenderTarget_Present), &CHwFullScreenRenderTarget_Present_Hook, reinterpret_cast<void**>(&o_CHwFullScreenRenderTarget_Present));

		MH_EnableHook(MH_ALL_HOOKS);
	}

	void disable() {
		input::disable();

		MH_DisableHook(MH_ALL_HOOKS);

		renderer::destroy();

		MH_Uninitialize();
	}
}