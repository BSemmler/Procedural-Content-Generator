//
// Created by Brett on 2024-03-03.
//

#include "SwapChainDX11.h"

namespace KGV::Render {
    SwapChainDX11::SwapChainDX11(const ComPtr<IDXGISwapChain1> &swapChain, const std::shared_ptr<ResourceViewDX11> &resource) : swapChain(
            swapChain), resource(resource) {}

    const ComPtr<IDXGISwapChain1> &SwapChainDX11::getSwapChain() const {
        return swapChain;
    }

    const std::shared_ptr<ResourceViewDX11> &SwapChainDX11::getResource() const {
        return resource;
    }
} // KGV