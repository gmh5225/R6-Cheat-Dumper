#include "renderer.hpp"

#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_dx11.h"
#include "../imgui/imgui_impl_win32.h"

namespace overlay::renderer {
    ID3D11Device* device = nullptr;
    ID3D11DeviceContext* context = nullptr;

    ID3D11RenderTargetView* target_view = nullptr;

    bool initialized() {
        return device != nullptr;
    }

    void create(ID3D11Device* device, IDXGISwapChain* swap_chain) {
        renderer::device = device;

        device->GetImmediateContext(&context);

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();

        ImGui_ImplWin32_Init(GetDesktopWindow());
        ImGui_ImplDX11_Init(device, context);

        ImGui::GetStyle().WindowRounding = 0.f;

        ID3D11Texture2D* pBackBuffer;
        swap_chain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
        device->CreateRenderTargetView(pBackBuffer, nullptr, &target_view);
        pBackBuffer->Release();
    }

    void destroy() {
        target_view->Release();

        ImGui::DestroyContext();

        ImGui_ImplDX11_Shutdown();
        ImGui_ImplWin32_Shutdown();

        device = nullptr;
        context = nullptr;

        target_view = nullptr;
    }
}