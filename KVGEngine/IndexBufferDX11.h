#pragma once
#include "BufferDX11.h"

namespace KGV::Render {
	class IndexBufferDX11 : public BufferDX11 {
	public:
		/**
		 * @brief Contructs a new vertex buffer object. 
		 * @param indexBuffer An ID3D11Buffer configured as an index buffer.
		*/
		IndexBufferDX11( ComPtr<ID3D11Buffer> indexBuffer ) : IndexBufferDX11( indexBuffer, 0, 0 ) {}

		/**
		 * @brief 
		 * @param indexBuffer An ID3D11Buffer configured as an index buffer.
		 * @param indexSize Size in bytes of an index.
		 * @param indexCount Number of indices in the buffer.
		*/
		IndexBufferDX11( ComPtr<ID3D11Buffer> indexBuffer, S32 indexSize, S32 indexCount );

		/**
		 * @brief Deconstructor...
		*/
		virtual ~IndexBufferDX11();

		/**
		 * @brief Returns the number of bytes which a index occupies in the buffer.
		 * @return Size of an index.
		*/
		S32	 getIndexSize();

		/**
		 * @brief Sets the number of bytes which a index occupies in the buffer.
		 * @param indexSize Size of an index.
		*/
		void setIndexSize( S32 indexSize );

		/**
		 * @brief Returns the number of indices contained with the buffer.
		 * @return Number of indices
		*/
		S32	 getIndexCount();

		/**
		 * @brief Sets the number of indices contained with the buffer.
		 * @param indexCount Number of indices
		*/
		void setIndexCount( S32 indexCount );

	protected:
		S32 indexSize;
		S32	indexCount;
	};
}