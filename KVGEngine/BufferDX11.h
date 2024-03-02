#ifndef BUFFER_DX11_H_
#define BUFFER_DX11_H_
#include "pch.h"
#include "ResourceDX11.h"

namespace KGV::Render 
{
	/**
	 * @brief Encapsulates a D3D buffer. Holds the desired description as well as the current description of the buffer.
	 * For more information on buffer descriptions refer to the link in this description.
	 * @see <a href="https://docs.microsoft.com/en-us/windows/win32/api/d3d11/ns-d3d11-d3d11_buffer_desc">D3D11_BUFFER_DESC</a>
	 * 
	 * <p>All of the getters with class retrieve the current description while all of the setters set the desired 
	 * buffer description.</p>
	*/
	class BufferDX11 : public ResourceDX11
	{
	public:
		BufferDX11();
		virtual ~BufferDX11();

        ID3D11Resource *getResource() override;

        D3D11_BUFFER_DESC	getActualDesc();
		D3D11_BUFFER_DESC	getDesiredDesc();
		void				setDesiredDesc(D3D11_BUFFER_DESC desc );

		U32					getByteWidth();
		void				setByteWidth( U32 width );

		D3D11_USAGE			getUsage();
		void				setUsage( D3D11_USAGE usage );

		U32					getBindFlags();
		void				setBindFlags( U32 flags);

		U32					getCPUAccessFlags();
		void				setCPUAccessFlags( U32 flags);

		U32					getMiscFlags();
		void				setMiscFlags( U32 flags );

		U32					getByteStride();
		void				setByteStride( U32 stride );

	protected:
		D3D11_BUFFER_DESC actualDesc;
		D3D11_BUFFER_DESC desiredDesc;

		ComPtr<ID3D11Buffer> buffer;
		friend class RenderDeviceDX11;
	};
}

#endif // BUFFER_DX11_H_