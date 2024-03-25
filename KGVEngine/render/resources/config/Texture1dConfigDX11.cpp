//
// Created by Brett on 2024-03-24.
//

#include "Texture1dConfigDX11.h"

namespace KGV::Render
{
    void Texture1dConfigDX11::setColorTexture( U32 width )
    {
        desc.Width				= width;
        desc.MipLevels			= 1;
        desc.ArraySize			= 1;
        desc.Format				= DXGI_FORMAT_R32_FLOAT;
        desc.Usage				= D3D11_USAGE_DEFAULT;
        desc.BindFlags			= D3D11_BIND_SHADER_RESOURCE;
        desc.CPUAccessFlags		= 0;
        desc.MiscFlags			= 0;
    }


    void Texture1dConfigDX11::setWidth( U32 width )
    {
        // KGV_SLOW_ASSERT_TRUE( width > 0 );
        desc.Width = width;
    }


    void Texture1dConfigDX11::setMipLevels( U32 levels )
    {
        // KGV_SLOW_ASSERT_TRUE( levels >= 1 );
        desc.MipLevels = levels;
    }

    void Texture1dConfigDX11::setArraySize( U32 arrSize )
    {
        // KGV_SLOW_ASSERT_TRUE( arrSize >= 1 );
        desc.ArraySize = arrSize;
    }

    D3D11_TEXTURE1D_DESC& Texture1dConfigDX11::getDesc() {
        return desc;
    }

    void Texture1dConfigDX11::setFormat(DXGI_FORMAT format) {
        desc.Format = format;
    }


    void Texture1dConfigDX11::setUsage(D3D11_USAGE usage) {
        desc.Usage = usage;
    }

    void Texture1dConfigDX11::setBindflags(U32 flags) {
        desc.BindFlags = flags;
    }

    void Texture1dConfigDX11::setCPUAccessFlags(U32 flags) {
        desc.CPUAccessFlags = flags;
    }

    void Texture1dConfigDX11::setMiscFlags(U32 flags) {
        desc.MiscFlags = flags;
    }

}