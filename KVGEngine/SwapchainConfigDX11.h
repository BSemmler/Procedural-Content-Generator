#pragma once
#include "pch.h"

namespace KGV::Render
{
	class SwapchainConfigDX11
	{
	public:
		SwapchainConfigDX11();
		~SwapchainConfigDX11();



	protected:
		DXGI_SWAP_CHAIN_DESC1 desc;

		friend class RenderDeviceDX11;
	};
}