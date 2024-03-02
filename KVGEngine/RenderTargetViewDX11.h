#ifndef RENDER_TARGET_VIEW_DX11_H_
#define RENDER_TARGET_VIEW_DX11_H_

#include "pch.h"

namespace KGV::Render
{
	class RenderTargetViewDX11
	{
	public:
		explicit RenderTargetViewDX11(ComPtr<ID3D11RenderTargetView> view);
		~RenderTargetViewDX11() = default;

	protected:
		ComPtr<ID3D11RenderTargetView> view;
	};
}

#endif // RENDER_TARGET_VIEW_DX11_H_