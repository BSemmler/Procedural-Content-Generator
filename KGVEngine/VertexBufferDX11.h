#ifndef VERTEX_BUFFER_DX11_H_
#define VERTEX_BUFFER_DX11_H_
#include "render/BufferDX11.h"


namespace KGV::Render {
	class VertexBufferDX11 : public BufferDX11 {
	public:
		/**
		 * @brief Instantiates a new vertex buffer object.
		 * @param vertexBuffer An ID3D11Buffer object configured as a vertex buffer.
		*/
		explicit VertexBufferDX11( Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer ) : VertexBufferDX11( std::move(vertexBuffer), 0, 0 ) {}

		/**
		 * @brief Instantiates a new vertex buffer object with vertex data.
		 * @param vertexBuffer An ID3D11Buffer object configured as a vertex buffer.
		 * @param vertexSize Size in bytes of a vertex.
		 * @param vertexCount Number of vertices in the buffer.
		*/
		VertexBufferDX11( Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer, S32 vertexSize, S32 vertexCount );
        ~VertexBufferDX11() override = default;

        eResourceType getResourceType() override;

        /**
         * @brief Returns the number of bytes which a vertex occupies in the buffer.
         * @return Size of the vertex.
        */
		S32	 getVertexSize() const;

		/**
		 * @brief Sets the number of bytes which a vertex occupies in the buffer.
		 * @param vertexSize Size of the vertex.
		*/
		void setVertexSize( S32 vertexSize );
		
		/**
		 * @brief Returns the number of vertices contained with the buffer.
		 * @return Number of vertices
		*/
		S32	 getVertexCount() const;

		/**
		 * @brief Sets the number of vertices contained with the buffer.
		 * @param vertexCount Number of vertices
		*/
		void setVertexCount( S32 vertexCount );

	protected:
		S32 vertexSize;
		S32	vertexCount;

        friend RenderDeviceDX11;
	};
}

#endif // VERTEX_BUFFER_DX11_H_