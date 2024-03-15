//
// Created by Brett on 3/14/2024.
//

#include "DepthStencilViewConfigDX11.h"

D3D11_DEPTH_STENCIL_VIEW_DESC &KGV::Render::DepthStencilViewConfigDX11::getDesc() {
    return desc;
}

KGV::Render::DepthStencilViewConfigDX11::DepthStencilViewConfigDX11() {
    setDefaults();
}

void KGV::Render::DepthStencilViewConfigDX11::setDefaults() {
    desc.Flags = 0;
    desc.Format = DXGI_FORMAT_UNKNOWN;
    desc.ViewDimension = D3D11_DSV_DIMENSION_UNKNOWN;
}

void KGV::Render::DepthStencilViewConfigDX11::setFormat(DXGI_FORMAT dxgiFormat) {
    desc.Format = dxgiFormat;
}

void KGV::Render::DepthStencilViewConfigDX11::setViewDimension(D3D11_DSV_DIMENSION dimension) {
    desc.ViewDimension = dimension;
}

void KGV::Render::DepthStencilViewConfigDX11::SetTexture1D(D3D11_TEX1D_DSV state) {
    desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE1D;
    desc.Texture1D = state;
}

void KGV::Render::DepthStencilViewConfigDX11::SetTexture1DArray(D3D11_TEX1D_ARRAY_DSV state) {
    desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE1DARRAY;
    desc.Texture1DArray = state;
}

void KGV::Render::DepthStencilViewConfigDX11::SetTexture2D(D3D11_TEX2D_DSV state) {
    desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    desc.Texture2D = state;
}

void KGV::Render::DepthStencilViewConfigDX11::SetTexture2DArray(D3D11_TEX2D_ARRAY_DSV state) {
    desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DARRAY;
    desc.Texture2DArray = state;
}

void KGV::Render::DepthStencilViewConfigDX11::SetTexture2DMS(D3D11_TEX2DMS_DSV state) {
    desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
    desc.Texture2DMS = state;
}

void KGV::Render::DepthStencilViewConfigDX11::SetTexture2DMSArray(D3D11_TEX2DMS_ARRAY_DSV state) {
    desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMSARRAY;
    desc.Texture2DMSArray = state;
}


