#pragma once
#include "pch.h"
#include "RenderDeviceDX11.h"

namespace KGV::Render
{
	class Resource
	{
		Resource(U32 resourceId, BufferConfigDX11 config )
	protected:
		U32 resourceId;
		U16 srvId;
		U16 rtvId;
		U16 dsvId;
		U16 uavId;

		std::shared_ptr<BufferConfigDX11> bufferConfig;
		std::shared_ptr<Texture2dConfigDX11> texture2dConfig;
	};
}