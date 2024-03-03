//
// Created by Brett on 2024-03-03.
//

#ifndef ENGINE_SWAPCHAIN_DX11_H
#define ENGINE_SWAPCHAIN_DX11_H

#include "pch.h"
#include "ResourceViewDX11.h"

namespace KGV::Render {
    class SwapChainDX11 {
    public:
        SwapChainDX11(const ComPtr<IDXGISwapChain1> &swapChain, const std::shared_ptr<ResourceViewDX11> &resource);

        const ComPtr<IDXGISwapChain1> &getSwapChain() const;

        const std::shared_ptr<ResourceViewDX11> &getResource() const;

    protected:
        ComPtr<IDXGISwapChain1> swapChain;
        std::shared_ptr<ResourceViewDX11> resource;
    };

}

#endif // ENGINE_SWAPCHAIN_DX11_H
