#ifndef TEXTURE_2D_DX11_H_
#define TEXTURE_2D_DX11_H_

#include "pch.h"
#include "ResourceDX11.h"

namespace KGV::Render {
	/**
	 * @brief This class encapsulates a ID3D11Texture2D object. It additionally holds the desired and actual descriptions
	 * of the texture. For information on the fields of this class please refer to the link in the description.
	 * @see <a href="https://docs.microsoft.com/en-us/windows/win32/api/d3d11/ns-d3d11-d3d11_buffer_desc">D3D11_BUFFER_DESC</a>
	 * 
	 * <p>All of the getters with class retrieve the current description while all of the setters set the desired 
	 * buffer description.</p>
	*/
	class Texture2dDX11 : public ResourceDX11 {
	public:
		Texture2dDX11();
		virtual ~Texture2dDX11();

		D3D11_TEXTURE2D_DESC	getActualDesc();
		D3D11_TEXTURE2D_DESC	getDesiredDesc();
		void					setDesiredDesc( D3D11_TEXTURE2D_DESC desc );

        eResourceType getResourceType() override;

        ID3D11Resource *getResource() override;

    protected:
		D3D11_TEXTURE2D_DESC actualDesc;
		D3D11_TEXTURE2D_DESC desiredDesc;

		Microsoft::WRL::ComPtr<ID3D11Texture2D> texture;

		friend class RenderDeviceDX11;
	};
}

#endif // TEXTURE_2D_DX11_H_