//
// Created by Brett on 2024-02-19.
//

#ifndef ENGINE_RENDER_TARGET_VIEW_CONFIG_DX11_H_
#define ENGINE_RENDER_TARGET_VIEW_CONFIG_DX11_H_
#include "pch.h"

namespace KGV::Render {
    class RenderTargetViewConfigDX11 {
    public:
        D3D11_RENDER_TARGET_VIEW_DESC& getDesc();

        void setFormat(DXGI_FORMAT format);
        void setViewDimension(D3D11_RTV_DIMENSION dimension);

        void setBuffer(D3D11_BUFFER_RTV rtvDesc);
        void setTexture1D(D3D11_TEX1D_RTV rtvDesc);
        void setTexture1DArray(D3D11_TEX1D_ARRAY_RTV rtvDesc);
        void setTexture2D(D3D11_TEX2D_RTV rtvDesc);
        void setTexture2DArray(D3D11_TEX2D_ARRAY_RTV rtvDesc);
        void setTexture2DMS(D3D11_TEX2DMS_RTV rtvDesc);
        void setTexture2DMSArray(D3D11_TEX2DMS_ARRAY_RTV rtvDesc);
        void setTexture3D(D3D11_TEX3D_RTV rtvDesc);
        
    protected:
        D3D11_RENDER_TARGET_VIEW_DESC desc;
    };

}


#endif // ENGINE_RENDER_TARGET_VIEW_CONFIG_DX11_H_
