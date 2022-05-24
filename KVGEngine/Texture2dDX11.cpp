#include "Texture2dDX11.h"

KGV::Render::Texture2dDX11::Texture2dDX11() 
{
	// Initialize both texture2D descriptions to a clean slate.
	ZeroMemory( &actualDesc, sizeof( D3D11_TEXTURE2D_DESC ) );
	ZeroMemory( &desiredDesc, sizeof( D3D11_TEXTURE2D_DESC ) );
}

KGV::Render::Texture2dDX11::~Texture2dDX11() 
{
	// Empty
}

D3D11_TEXTURE2D_DESC KGV::Render::Texture2dDX11::getActualDesc() 
{
	return actualDesc;
}

D3D11_TEXTURE2D_DESC KGV::Render::Texture2dDX11::getDesiredDesc() 
{
	return desiredDesc;
}

void KGV::Render::Texture2dDX11::setDesiredDesc( D3D11_TEXTURE2D_DESC desc ) 
{
	desiredDesc = desc;
}

U32 KGV::Render::Texture2dDX11::getWidth() 
{
	return actualDesc.Width;
}

void KGV::Render::Texture2dDX11::setWidth( U32 width ) 
{
	desiredDesc.Width = width;
}

U32 KGV::Render::Texture2dDX11::getHeight() 
{
	return actualDesc.Height;
}

void KGV::Render::Texture2dDX11::setHeight( U32 height ) 
{
	desiredDesc.Height = height;
}

U32 KGV::Render::Texture2dDX11::getMipLevels() 
{
	return actualDesc.MipLevels;
}

void KGV::Render::Texture2dDX11::setMipLevels( U32 levels ) 
{
	desiredDesc.MipLevels = levels;
}

U32 KGV::Render::Texture2dDX11::getArraySize() 
{
	return actualDesc.ArraySize;
}

void KGV::Render::Texture2dDX11::setArraySize( U32 arraySize ) 
{
	desiredDesc.ArraySize = arraySize;
}

DXGI_FORMAT KGV::Render::Texture2dDX11::getFormat() 
{
	return actualDesc.Format;
}

void KGV::Render::Texture2dDX11::setFormat( DXGI_FORMAT format ) 
{
	desiredDesc.Format = format;
}

DXGI_SAMPLE_DESC KGV::Render::Texture2dDX11::getSampleDesc() 
{
	return actualDesc.SampleDesc;
}

void KGV::Render::Texture2dDX11::setSampleDesc( DXGI_SAMPLE_DESC desc ) 
{
	desiredDesc.SampleDesc = desc;
}

D3D11_USAGE KGV::Render::Texture2dDX11::getUsage() 
{
	return actualDesc.Usage;
}

void KGV::Render::Texture2dDX11::setUsage( D3D11_USAGE usage ) 
{
	desiredDesc.Usage = usage;
}

U32 KGV::Render::Texture2dDX11::getBindFlags() 
{
	return actualDesc.BindFlags;
}

void KGV::Render::Texture2dDX11::setBindFlags( U32 flags ) 
{
	desiredDesc.BindFlags = flags;
}

U32 KGV::Render::Texture2dDX11::getCPUAccessFlags() 
{
	return actualDesc.CPUAccessFlags;
}

void KGV::Render::Texture2dDX11::setCPUAccessFlags( U32 flags ) 
{
	desiredDesc.CPUAccessFlags = flags;
}

U32 KGV::Render::Texture2dDX11::getMiscFlags() 
{
	return actualDesc.MiscFlags;
}

void KGV::Render::Texture2dDX11::setMiscFlags( U32 flags ) 
{
	desiredDesc.MiscFlags = flags;
}
