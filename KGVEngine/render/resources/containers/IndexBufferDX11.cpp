#include "IndexBufferDX11.h"

KGV::Render::IndexBufferDX11::IndexBufferDX11( Microsoft::WRL::ComPtr<ID3D11Buffer> indexBuffer, S32 _indexSize, S32 _indexCount )
{
	this->buffer = std::move(indexBuffer);
	this->indexSize = _indexSize;
	this->indexCount = _indexCount;
}

S32 KGV::Render::IndexBufferDX11::getIndexSize() const
{
	return indexSize;
}

void KGV::Render::IndexBufferDX11::setIndexSize( S32 _indexSize )
{
	this->indexSize = _indexSize;
}

S32 KGV::Render::IndexBufferDX11::getIndexCount() const
{
	return indexCount;
}

void KGV::Render::IndexBufferDX11::setIndexCount( S32 _indexCount )
{
	this->indexCount = _indexCount;
}

KGV::Render::eResourceType KGV::Render::IndexBufferDX11::getResourceType() {
    return eResourceType::kIndexBuffer;
}
