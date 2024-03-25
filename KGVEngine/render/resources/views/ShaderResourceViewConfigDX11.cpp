//
// Created by Brett on 2024-02-19.
//

#include "ShaderResourceViewConfigDX11.h"

D3D11_SHADER_RESOURCE_VIEW_DESC& KGV::Render::ShaderResourceViewConfigDX11::getDesc() {
    return desc;
}

void KGV::Render::ShaderResourceViewConfigDX11::setFormat(DXGI_FORMAT format) {
    desc.Format = format;
}

void KGV::Render::ShaderResourceViewConfigDX11::setViewDimension(D3D11_SRV_DIMENSION dimension) {
    desc.ViewDimension = dimension;
}

void KGV::Render::ShaderResourceViewConfigDX11::setBuffer(D3D11_BUFFER_SRV srvDesc) {
    desc.Buffer = srvDesc;
}

void KGV::Render::ShaderResourceViewConfigDX11::setBufferEx(D3D11_BUFFEREX_SRV srvDesc) {
    desc.BufferEx = srvDesc;
}

void KGV::Render::ShaderResourceViewConfigDX11::setTexture1D(D3D11_TEX1D_SRV srvDesc) {
    desc.Texture1D = srvDesc;
    desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE1D;
}

void KGV::Render::ShaderResourceViewConfigDX11::setTexture1DArray(D3D11_TEX1D_ARRAY_SRV srvDesc) {
    desc.Texture1DArray = srvDesc;
    desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE1DARRAY;
}

void KGV::Render::ShaderResourceViewConfigDX11::setTexture2D(D3D11_TEX2D_SRV srvDesc) {
    desc.Texture2D = srvDesc;
    desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
}

void KGV::Render::ShaderResourceViewConfigDX11::setTexture2DArray(D3D11_TEX2D_ARRAY_SRV srvDesc) {
    desc.Texture2DArray = srvDesc;
    desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
}

void KGV::Render::ShaderResourceViewConfigDX11::setTexture2DMS(D3D11_TEX2DMS_SRV srvDesc) {
    desc.Texture2DMS = srvDesc;
    desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DMS;
}

void KGV::Render::ShaderResourceViewConfigDX11::setTexture2DMSArray(D3D11_TEX2DMS_ARRAY_SRV srvDesc) {
    desc.Texture2DMSArray = srvDesc;
    desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DMSARRAY;
}

void KGV::Render::ShaderResourceViewConfigDX11::setTexture3D(D3D11_TEX3D_SRV srvDesc) {
    desc.Texture3D = srvDesc;
    desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE3D;
}

void KGV::Render::ShaderResourceViewConfigDX11::setTextureCube(D3D11_TEXCUBE_SRV srvDesc) {
    desc.TextureCube = srvDesc;
    desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
}

void KGV::Render::ShaderResourceViewConfigDX11::setTextureCubeArray(D3D11_TEXCUBE_ARRAY_SRV srvDesc) {
    desc.TextureCubeArray = srvDesc;
    desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBEARRAY;
}
