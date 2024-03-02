#include "Texture2dConfigDX11.h"

namespace KGV::Render
{
	void Texture2dConfigDX11::setColorTexture( U32 width, U32 height )
	{
		desc.Width				= width;
		desc.Height				= height;
		desc.MipLevels			= 1;
		desc.ArraySize			= 1;
		desc.Format				= DXGI_FORMAT_R8G8B8A8_UNORM;
		desc.SampleDesc.Count	= 1;
		desc.SampleDesc.Quality = 0;
		desc.Usage				= D3D11_USAGE_DEFAULT;
		desc.BindFlags			= D3D11_BIND_SHADER_RESOURCE;
		desc.CPUAccessFlags		= 0;
		desc.MiscFlags			= 0;
	}

	void Texture2dConfigDX11::setDepthTexture( U32 width, U32 height )
	{
		desc.Width				= width;
		desc.Height				= height;
		desc.MipLevels			= 1;
		desc.ArraySize			= 1;
		desc.Format				= DXGI_FORMAT_D32_FLOAT;
		desc.SampleDesc.Count	= 1;
		desc.SampleDesc.Quality = 0;
		desc.Usage				= D3D11_USAGE_DEFAULT;
		desc.BindFlags			= D3D11_BIND_SHADER_RESOURCE;
		desc.CPUAccessFlags		= 0;
		desc.MiscFlags			= 0;
	}

	void Texture2dConfigDX11::setWidth( U32 width )
	{
		KGV_SLOW_ASSERT_TRUE( width > 0 );
		desc.Width = width;
	}

	void Texture2dConfigDX11::setHeight( U32 height )
	{
		KGV_SLOW_ASSERT_TRUE( height > 0 );
		desc.Height = height;
	}

	void Texture2dConfigDX11::setMipLevels( U32 levels )
	{
		KGV_SLOW_ASSERT_TRUE( levels >= 1 );
		desc.MipLevels = levels;
	}

	void Texture2dConfigDX11::setArraySize( U32 arrSize )
	{
		KGV_SLOW_ASSERT_TRUE( arrSize >= 1 );
		desc.ArraySize = arrSize;
	}

    D3D11_TEXTURE2D_DESC Texture2dConfigDX11::getDesc() {
        return desc;
    }

    void Texture2dConfigDX11::setFormat(DXGI_FORMAT format) {
        desc.Format = format;
    }

    void Texture2dConfigDX11::setSampleDesc(DXGI_SAMPLE_DESC sampleDesc) {
        desc.SampleDesc = sampleDesc;
    }

    void Texture2dConfigDX11::setUsage(DXGI_USAGE usage) {
        desc.Usage = usage;
    }

    void Texture2dConfigDX11::setBindflags(U32 flags) {
        desc.BindFlags = flags;
    }

    void Texture2dConfigDX11::setCPUAccessFlags(U32 flags) {
        desc.CPUAccessFlags = flags;
    }

    void Texture2dConfigDX11::setMiscFlags(U32 flags) {
        desc.MiscFlags = flags;
    }

}