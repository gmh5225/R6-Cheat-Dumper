#ifndef _OVERLAY_RENDERER_RENDERER_HPP_
#define _OVERLAY_RENDERER_RENDERER_HPP_

#include <d3d11.h>

namespace overlay::renderer {
    extern  ID3D11Device* device;
    extern  ID3D11DeviceContext* context;

    extern ID3D11RenderTargetView* target_view;

    extern bool initialized();

    extern void create(ID3D11Device* device, IDXGISwapChain* swap_chain);
    extern void destroy();
}

#endif