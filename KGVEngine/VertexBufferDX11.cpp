#include "VertexBufferDX11.h"

KGV::Render::VertexBufferDX11::VertexBufferDX11( Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer, S32 vertexSize, S32 vertexCount ) 
{
	this->buffer = vertexBuffer;
	this->vertexSize = vertexSize;
	this->vertexCount = vertexCount;
}

S32 KGV::Render::VertexBufferDX11::getVertexSize() const
{
	return vertexSize;
}

void KGV::Render::VertexBufferDX11::setVertexSize( S32 _vertexSize )
{
	this->vertexSize = _vertexSize;
}

S32 KGV::Render::VertexBufferDX11::getVertexCount() const
{
	return vertexCount;
}

void KGV::Render::VertexBufferDX11::setVertexCount( S32 _vertexCount )
{
	this->vertexCount = _vertexCount;
}

KGV::Render::eResourceType KGV::Render::VertexBufferDX11::getResourceType() {
    return eResourceType::kVertexBuffer;
}
