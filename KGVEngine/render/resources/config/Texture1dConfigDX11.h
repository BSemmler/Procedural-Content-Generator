//
// Created by Brett on 2024-03-24.
//

#ifndef ENGINE_TEXTURE1DCONFIGDX11_H
#define ENGINE_TEXTURE1DCONFIGDX11_H

#include "pch.h"

namespace KGV::Render {
    class Texture1dConfigDX11 {
    public:
        void setColorTexture( U32 width );

        void setWidth(U32 width);
        void setMipLevels(U32 levels);
        void setArraySize(U32 arrSize);
        void setFormat(DXGI_FORMAT format);
        void setUsage(D3D11_USAGE usage);
        void setBindflags(U32 flags);
        void setCPUAccessFlags(U32 flags);
        void setMiscFlags(U32 flags);


        D3D11_TEXTURE1D_DESC& getDesc();

    protected:
        D3D11_TEXTURE1D_DESC desc{};

        friend class RenderDeviceDX11;
    };
}



#endif //ENGINE_TEXTURE1DCONFIGDX11_H
