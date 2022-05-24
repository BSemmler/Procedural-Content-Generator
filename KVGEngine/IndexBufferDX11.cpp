#include "IndexBufferDX11.h"

KGV::Render::IndexBufferDX11::IndexBufferDX11( Microsoft::WRL::ComPtr<ID3D11Buffer> indexBuffer, S32 indexSize, S32 indexCount ) 
{
	this->buffer = indexBuffer;
	this->indexSize = indexSize;
	this->indexCount = indexCount;
}

KGV::Render::IndexBufferDX11::~IndexBufferDX11() 
{
	// Empty
}

S32 KGV::Render::IndexBufferDX11::getIndexSize() 
{
	return indexSize;
}

void KGV::Render::IndexBufferDX11::setIndexSize( S32 indexSize ) 
{
	this->indexSize = indexSize;
}

S32 KGV::Render::IndexBufferDX11::getIndexCount() 
{
	return indexCount;
}

void KGV::Render::IndexBufferDX11::setIndexCount( S32 indexCount ) 
{
	this->indexCount;
}
