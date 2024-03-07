#ifndef SHADER_RESOURCE_VIEW_DX11_H_
#define SHADER_RESOURCE_VIEW_DX11_H_
#include "pch.h"

namespace KGV::Render
{
	class ShaderResourceViewDX11
	{
	public:
		explicit ShaderResourceViewDX11(ComPtr<ID3D11ShaderResourceView> view);
		~ShaderResourceViewDX11() = default;

        const ComPtr<ID3D11ShaderResourceView> &getView() const;

        void setView(const ComPtr<ID3D11ShaderResourceView> &view);

    protected:
		ComPtr<ID3D11ShaderResourceView> view;
	};
}

#endif // SHADER_RESOURCE_VIEW_DX11_H_