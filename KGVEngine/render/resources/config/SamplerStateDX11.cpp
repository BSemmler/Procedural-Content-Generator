//
// Created by Brett on 2024-03-16.
//

#include "SamplerStateDX11.h"

void KGV::Render::SamplerStateDX11::setDefaults() {
    Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    MipLODBias = 0.0f;
    MaxAnisotropy = 1;
    ComparisonFunc = D3D11_COMPARISON_ALWAYS;
    BorderColor[ 0 ] = 0;
    BorderColor[ 1 ] = 0;
    BorderColor[ 2 ] = 0;
    BorderColor[ 3 ] = 0;
    MinLOD = 0;
    MaxLOD = 0;
}
