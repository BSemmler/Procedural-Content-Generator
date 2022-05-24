#pragma once
#include "pch.h"

namespace KGV::Render {
	/**
	 * @brief This class encapsulates a ID3D11Texture2D object. It additionally holds the desired and actual descriptions
	 * of the texture. For information on the fields of this class please refer to the link in the description.
	 * @see <a href="https://docs.microsoft.com/en-us/windows/win32/api/d3d11/ns-d3d11-d3d11_buffer_desc">D3D11_BUFFER_DESC</a>
	 * 
	 * <p>All of the getters with class retrieve the current description while all of the setters set the desired 
	 * buffer description.</p>
	*/
	class Texture2dDX11 {
	public:
		Texture2dDX11();
		virtual ~Texture2dDX11();

		D3D11_TEXTURE2D_DESC	getActualDesc();
		D3D11_TEXTURE2D_DESC	getDesiredDesc();
		void					setDesiredDesc( D3D11_TEXTURE2D_DESC desc );

		U32						getWidth();
		void					setWidth( U32 width );

		U32						getHeight();
		void					setHeight( U32 height );

		U32						getMipLevels();
		void					setMipLevels( U32 levels );

		U32						getArraySize();
		void					setArraySize( U32 arraySize );

		DXGI_FORMAT				getFormat();
		void					setFormat( DXGI_FORMAT format );

		DXGI_SAMPLE_DESC		getSampleDesc();
		void					setSampleDesc( DXGI_SAMPLE_DESC desc );

		D3D11_USAGE				getUsage();
		void					setUsage( D3D11_USAGE usage );

		U32						getBindFlags();
		void					setBindFlags( U32 flags );

		U32						getCPUAccessFlags();
		void					setCPUAccessFlags( U32 flags );

		U32						getMiscFlags();
		void					setMiscFlags( U32 flags );

	protected:
		D3D11_TEXTURE2D_DESC actualDesc;
		D3D11_TEXTURE2D_DESC desiredDesc;

		Microsoft::WRL::ComPtr<ID3D11Texture2D> texture;

		friend class RenderDeviceDX11;
	};
}