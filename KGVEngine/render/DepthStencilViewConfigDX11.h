//
// Created by Brett on 3/14/2024.
//

#ifndef ENGINE_DEPTHSTENCILVIEWCONFIGDX11_H
#define ENGINE_DEPTHSTENCILVIEWCONFIGDX11_H

#include "pch.h"

namespace KGV::Render {
    class DepthStencilViewConfigDX11 {
    public:
        DepthStencilViewConfigDX11();
        void setDefaults();
        void setFormat(DXGI_FORMAT dxgiFormat);
        void setViewDimension(D3D11_DSV_DIMENSION dimension);

        void SetTexture1D(D3D11_TEX1D_DSV state);
        void SetTexture1DArray(D3D11_TEX1D_ARRAY_DSV state);
        void SetTexture2D(D3D11_TEX2D_DSV state);
        void SetTexture2DArray(D3D11_TEX2D_ARRAY_DSV state);
        void SetTexture2DMS(D3D11_TEX2DMS_DSV state);
        void SetTexture2DMSArray(D3D11_TEX2DMS_ARRAY_DSV state);

        D3D11_DEPTH_STENCIL_VIEW_DESC &getDesc();

    protected:
        D3D11_DEPTH_STENCIL_VIEW_DESC desc{};
    };
};



#endif //ENGINE_DEPTHSTENCILVIEWCONFIGDX11_H
