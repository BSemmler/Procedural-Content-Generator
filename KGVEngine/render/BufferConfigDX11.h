#ifndef BUFFER_CONFIG_DX11_H_
#define BUFFER_CONFIG_DX11_H_

#include "pch.h"

namespace KGV::Render
{
    enum eBufferUpdateType {
        kImmutable = 0,
        kDefault = 1,
        kDynamic = 2,
    };

	class BufferConfigDX11
	{
    public:
		void setDefaultConstantBuffer( U32 size, eBufferUpdateType updateType );
		void setDefaultVertexBuffer( U32 size, eBufferUpdateType updateType );
		void setDefaultIndexBuffer( U32 size, eBufferUpdateType updateType );
        D3D11_BUFFER_DESC getDesc();

    protected:
		D3D11_BUFFER_DESC desc;

		friend class RenderDeviceDX11;
	};

}

#endif // BUFFER_CONFIG_DX11_H_