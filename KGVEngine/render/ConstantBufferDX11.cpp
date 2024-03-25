//
// Created by Brett on 2024-03-06.
//

#include "ConstantBufferDX11.h"

KGV::Render::eResourceType KGV::Render::ConstantBufferDX11::getResourceType() {
    return eResourceType::kConstantBuffer;
}

KGV::Render::ConstantBufferDX11::ConstantBufferDX11(ComPtr<ID3D11Buffer> constantBuffer) {
    buffer = constantBuffer;
}
