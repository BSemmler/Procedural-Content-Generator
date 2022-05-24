#pragma once
#include "pch.h"

namespace KGV::Render
{
	class RenderTargetViewDX11
	{
	public:
		RenderTargetViewDX11();
		~RenderTargetViewDX11();

	protected:
		ComPtr<ID3D11RenderTargetView> view;
	};
}