#ifndef BUFFER_CONFIG_DX11_H_
#define BUFFER_CONFIG_DX11_H_

#include "pch.h"

namespace KGV::Render
{
	class BufferConfigDX11
	{
    public:
		void setDefaultConstantBuffer( U32 size, bool dynamic );
		void setDefaultVertexBuffer( U32 size, bool dynamic );
		void setDefaultIndexBuffer( U32 size, bool dynamic );
        D3D11_BUFFER_DESC getDesc();

    protected:
		D3D11_BUFFER_DESC desc;

		friend class RenderDeviceDX11;
	};

}

#endif // BUFFER_CONFIG_DX11_H_