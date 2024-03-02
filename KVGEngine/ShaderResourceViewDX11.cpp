//
// Created by Brett on 2024-03-02.
//
#include "ShaderResourceViewDX11.h"

#include <utility>

KGV::Render::ShaderResourceViewDX11::ShaderResourceViewDX11(ComPtr<ID3D11ShaderResourceView> srv) {
    view = std::move(srv);
}
