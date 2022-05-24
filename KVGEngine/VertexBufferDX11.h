#pragma once
#include "BufferDX11.h"

namespace KGV::Render {
	class VertexBufferDX11 : public BufferDX11 {
	public:
		/**
		 * @brief Instantiates a new vertex buffer object.
		 * @param vertexBuffer An ID3D11Buffer object configured as a vertex buffer.
		*/
		VertexBufferDX11( Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer ) : VertexBufferDX11( vertexBuffer, 0, 0 ) {}

		/**
		 * @brief Instantiates a new vertex buffer object with vertex data.
		 * @param vertexBuffer An ID3D11Buffer object configured as a vertex buffer.
		 * @param vertexSize Size in bytes of a vertex.
		 * @param vertexCount Number of vertices in the buffer.
		*/
		VertexBufferDX11( Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer, S32 vertexSize, S32 vertexCount );


		virtual ~VertexBufferDX11();

		/**
		 * @brief Returns the number of bytes which a vertex occupies in the buffer.
		 * @return Size of the vertex.
		*/
		S32	 getVertexSize();

		/**
		 * @brief Sets the number of bytes which a vertex occupies in the buffer.
		 * @param vertexSize Size of the vertex.
		*/
		void setVertexSize( S32 vertexSize );
		
		/**
		 * @brief Returns the number of vertices contained with the buffer.
		 * @return Number of vertices
		*/
		S32	 getVertexCount();

		/**
		 * @brief Sets the number of vertices contained with the buffer.
		 * @param vertexCount Number of vertices
		*/
		void setVertexCount( S32 vertexCount );

	protected:
		S32 vertexSize;
		S32	vertexCount;
	};
}