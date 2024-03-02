#ifndef TEXTURE_2D_CONFIG_DX11_H_
#define TEXTURE_2D_CONFIG_DX11_H_
#include "pch.h"

namespace KGV::Render
{
	class Texture2dConfigDX11
	{
	public:
		void setColorTexture( U32 width, U32 height );
		void setDepthTexture( U32 width, U32 height );

		void setWidth(U32 width);
		void setHeight( U32 height);
		void setMipLevels(U32 levels);
		void setArraySize(U32 arrSize);
        void setFormat(DXGI_FORMAT format);
        void setSampleDesc(DXGI_SAMPLE_DESC sampleDesc);
        void setUsage(D3D11_USAGE usage);
        void setBindflags(U32 flags);
        void setCPUAccessFlags(U32 flags);
        void setMiscFlags(U32 flags);


        D3D11_TEXTURE2D_DESC getDesc();

    protected:
		D3D11_TEXTURE2D_DESC desc{};

		friend class RenderDeviceDX11;
    };
}
#endif // TEXTURE_2D_CONFIG_DX11_H_