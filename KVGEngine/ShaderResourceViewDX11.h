#pragma once
#include "pch.h"

namespace KGV::Render
{
	class ShaderResourceViewDX11
	{
	public:
		ShaderResourceViewDX11();
		~ShaderResourceViewDX11();

	protected:
		ComPtr<ID3D11ShaderResourceView> view;
	};
}