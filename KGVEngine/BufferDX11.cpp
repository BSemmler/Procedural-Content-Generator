#include "BufferDX11.h"

KGV::Render::BufferDX11::BufferDX11() 
{
	// Initialize both buffer descriptions to a clean slate.
	ZeroMemory( &actualDesc, sizeof( D3D11_BUFFER_DESC ) );
	ZeroMemory( &desiredDesc, sizeof( D3D11_BUFFER_DESC ) );
}

KGV::Render::BufferDX11::~BufferDX11() 
{
	// Empty
}

// ******************************************************************************************
//								Getters and Setters
// ******************************************************************************************

D3D11_BUFFER_DESC KGV::Render::BufferDX11::getActualDesc() 
{
	return actualDesc;
}

D3D11_BUFFER_DESC KGV::Render::BufferDX11::getDesiredDesc() 
{
	return desiredDesc;
}

void KGV::Render::BufferDX11::setDesiredDesc( D3D11_BUFFER_DESC desc ) 
{
	desiredDesc = desc;
}

U32 KGV::Render::BufferDX11::getByteWidth() 
{
	return actualDesc.ByteWidth;
}

void KGV::Render::BufferDX11::setByteWidth( U32 width ) 
{
	desiredDesc.ByteWidth = width;
}

D3D11_USAGE KGV::Render::BufferDX11::getUsage() 
{
	return actualDesc.Usage;
}

void KGV::Render::BufferDX11::setUsage( D3D11_USAGE usage ) 
{
	desiredDesc.Usage = usage;
}

U32 KGV::Render::BufferDX11::getBindFlags() 
{
	return actualDesc.BindFlags;
}

void KGV::Render::BufferDX11::setBindFlags( U32 flags ) 
{
	desiredDesc.BindFlags = flags;
}

U32 KGV::Render::BufferDX11::getCPUAccessFlags() 
{
	return actualDesc.CPUAccessFlags;
}

void KGV::Render::BufferDX11::setCPUAccessFlags( U32 flags ) 
{
	desiredDesc.CPUAccessFlags = flags;
}

U32 KGV::Render::BufferDX11::getMiscFlags() 
{
	return actualDesc.MiscFlags;
}

void KGV::Render::BufferDX11::setMiscFlags( U32 flags ) 
{
	desiredDesc.MiscFlags = flags;
}

U32 KGV::Render::BufferDX11::getByteStride() 
{
	return actualDesc.StructureByteStride;
}

void KGV::Render::BufferDX11::setByteStride( U32 stride ) 
{
	desiredDesc.StructureByteStride = stride;
}

ID3D11Resource *KGV::Render::BufferDX11::getResource() {
    return buffer.Get();
}

ComPtr<ID3D11Buffer> KGV::Render::BufferDX11::getBuffer() {
    return buffer;
}
