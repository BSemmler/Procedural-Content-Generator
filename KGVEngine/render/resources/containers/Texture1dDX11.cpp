//
// Created by Brett on 2024-03-24.
//

#include "Texture1dDX11.h"

KGV::Render::Texture1dDX11::Texture1dDX11(ComPtr<ID3D11Texture1D> resource)
{
    texture = resource;
    // Initialize both texture2D descriptions to a clean slate.
    ZeroMemory( &actualDesc, sizeof( D3D11_TEXTURE1D_DESC ) );
    ZeroMemory( &desiredDesc, sizeof( D3D11_TEXTURE1D_DESC ) );
}

D3D11_TEXTURE1D_DESC KGV::Render::Texture1dDX11::getActualDesc()
{
    texture->GetDesc(&actualDesc);
    return actualDesc;
}

D3D11_TEXTURE1D_DESC KGV::Render::Texture1dDX11::getDesiredDesc()
{
    return desiredDesc;
}

void KGV::Render::Texture1dDX11::setDesiredDesc( D3D11_TEXTURE1D_DESC desc )
{
    desiredDesc = desc;
}

KGV::Render::eResourceType KGV::Render::Texture1dDX11::getResourceType() {
    return eResourceType::kTexture1D;
}

ID3D11Resource *KGV::Render::Texture1dDX11::getResource() {
    return texture.Get();
}
