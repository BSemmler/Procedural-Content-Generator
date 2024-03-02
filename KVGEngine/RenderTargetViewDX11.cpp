//
// Created by Brett on 2024-03-02.
//
#include "RenderTargetViewDX11.h"

KGV::Render::RenderTargetViewDX11::RenderTargetViewDX11(ComPtr<ID3D11RenderTargetView> rtv) {
    view = std::move(rtv);
}
