//
// Created by Brett on 2024-02-19.
//

#ifndef ENGINE_SHADERRESOURCEVIEWCONFIGDX11_H
#define ENGINE_SHADERRESOURCEVIEWCONFIGDX11_H

#include "pch.h"

namespace KGV::Render {
    class ShaderResourceViewConfigDX11 {
    public:
        D3D11_SHADER_RESOURCE_VIEW_DESC getDesc();

        void setFormat(DXGI_FORMAT format);
        void setViewDimension(D3D11_SRV_DIMENSION dimension);

        void setBuffer(D3D11_BUFFER_SRV srvDesc);
        void setBufferEx(D3D11_BUFFEREX_SRV srvDesc);
        void setTexture1D(D3D11_TEX1D_SRV srvDesc);
        void setTexture1DArray(D3D11_TEX1D_ARRAY_SRV srvDesc);
        void setTexture2D(D3D11_TEX2D_SRV srvDesc);
        void setTexture2DArray(D3D11_TEX2D_ARRAY_SRV srvDesc);
        void setTexture2DMS(D3D11_TEX2DMS_SRV srvDesc);
        void setTexture2DMSArray(D3D11_TEX2DMS_ARRAY_SRV srvDesc);
        void setTexture3D(D3D11_TEX3D_SRV srvDesc);
        void setTextureCube(D3D11_TEXCUBE_SRV srvDesc);
        void setTextureCubeArray(D3D11_TEXCUBE_ARRAY_SRV srvDesc);
    protected:
        D3D11_SHADER_RESOURCE_VIEW_DESC desc;
    };
}



#endif //ENGINE_SHADERRESOURCEVIEWCONFIGDX11_H
