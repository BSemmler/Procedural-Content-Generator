#include "VertexBufferDX11.h"

KGV::Render::VertexBufferDX11::VertexBufferDX11( Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer, S32 vertexSize, S32 vertexCount ) 
{
	this->buffer = vertexBuffer;
	this->vertexSize = vertexSize;
	this->vertexCount = vertexCount;
}

KGV::Render::VertexBufferDX11::~VertexBufferDX11() 
{
	// Empty
}

S32 KGV::Render::VertexBufferDX11::getVertexSize() 
{
	return vertexSize;
}

void KGV::Render::VertexBufferDX11::setVertexSize( S32 vertexSize ) 
{
	this->vertexSize = vertexSize;
}

S32 KGV::Render::VertexBufferDX11::getVertexCount() 
{
	return vertexCount;
}

void KGV::Render::VertexBufferDX11::setVertexCount( S32 vertexCount ) 
{
	this->vertexCount = vertexCount;
}
