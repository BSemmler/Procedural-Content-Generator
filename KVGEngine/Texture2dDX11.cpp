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

KGV::Render::eResourceType KGV::Render::Texture2dDX11::getResourceType() {
    return eResourceType::kTexture2D;
}

ID3D11Resource *KGV::Render::Texture2dDX11::getResource() {
    return texture.Get();
}
