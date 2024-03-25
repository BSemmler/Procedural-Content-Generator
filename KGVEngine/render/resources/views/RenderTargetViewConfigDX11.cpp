//
// Created by Brett on 2024-02-19.
//

#include "RenderTargetViewConfigDX11.h"

D3D11_RENDER_TARGET_VIEW_DESC& KGV::Render::RenderTargetViewConfigDX11::getDesc() {
    return desc;
}

void KGV::Render::RenderTargetViewConfigDX11::setFormat(DXGI_FORMAT format) {
    desc.Format = format;
}

void KGV::Render::RenderTargetViewConfigDX11::setViewDimension(D3D11_RTV_DIMENSION dimension) {
    desc.ViewDimension = dimension;
}

void KGV::Render::RenderTargetViewConfigDX11::setBuffer(D3D11_BUFFER_RTV rtvDesc) {
    desc.Buffer = rtvDesc;
}

void KGV::Render::RenderTargetViewConfigDX11::setTexture1D(D3D11_TEX1D_RTV rtvDesc) {
    desc.Texture1D = rtvDesc;
}

void KGV::Render::RenderTargetViewConfigDX11::setTexture1DArray(D3D11_TEX1D_ARRAY_RTV rtvDesc) {
    desc.Texture1DArray = rtvDesc;
}

void KGV::Render::RenderTargetViewConfigDX11::setTexture2D(D3D11_TEX2D_RTV rtvDesc) {
    desc.Texture2D = rtvDesc;
}

void KGV::Render::RenderTargetViewConfigDX11::setTexture2DArray(D3D11_TEX2D_ARRAY_RTV rtvDesc) {
    desc.Texture2DArray = rtvDesc;
}

void KGV::Render::RenderTargetViewConfigDX11::setTexture2DMS(D3D11_TEX2DMS_RTV rtvDesc) {
    desc.Texture2DMS = rtvDesc;
}

void KGV::Render::RenderTargetViewConfigDX11::setTexture2DMSArray(D3D11_TEX2DMS_ARRAY_RTV rtvDesc) {
    desc.Texture2DMSArray = rtvDesc;
}

void KGV::Render::RenderTargetViewConfigDX11::setTexture3D(D3D11_TEX3D_RTV rtvDesc) {
    desc.Texture3D = rtvDesc;
}
