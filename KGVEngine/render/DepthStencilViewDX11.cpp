//
// Created by Brett on 3/14/2024.
//

#include "DepthStencilViewDX11.h"

KGV::Render::DepthStencilViewDX11::DepthStencilViewDX11(const ComPtr<ID3D11DepthStencilView> &view) : view(view) {}

const ComPtr<ID3D11DepthStencilView> &KGV::Render::DepthStencilViewDX11::getView() const {
    return view;
}

void KGV::Render::DepthStencilViewDX11::setView(const ComPtr<ID3D11DepthStencilView> &view) {
    DepthStencilViewDX11::view = view;
}
