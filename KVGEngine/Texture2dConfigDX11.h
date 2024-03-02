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

		void setWidth(U32 width );
		void setHeight( U32 height );
		void setMipLevels( U32 levels );
		void setArraySize( U32 arrSize );

        D3D11_TEXTURE2D_DESC getDesc();

    protected:
		D3D11_TEXTURE2D_DESC desc{};

		friend class RenderDeviceDX11;
    };
}
#endif // TEXTURE_2D_CONFIG_DX11_H_