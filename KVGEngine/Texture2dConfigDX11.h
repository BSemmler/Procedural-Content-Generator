#pragma once
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

	protected:
		D3D11_TEXTURE2D_DESC desc;

		friend class RenderDeviceDX11;
	};
}