//
// Created by Brett on 2024-03-24.
//

#ifndef ENGINE_TEXTURE1DDX11_H
#define ENGINE_TEXTURE1DDX11_H

#include "pch.h"
#include "render/resources/ResourceDX11.h"

namespace KGV::Render {
    class Texture1dDX11 : public ResourceDX11 {
    public:
        explicit Texture1dDX11(ComPtr<ID3D11Texture1D> resource);
        virtual ~Texture1dDX11() = default;

        D3D11_TEXTURE1D_DESC	getActualDesc();
        D3D11_TEXTURE1D_DESC	getDesiredDesc();
        void					setDesiredDesc( D3D11_TEXTURE1D_DESC desc );

        eResourceType getResourceType() override;

        ID3D11Resource *getResource() override;

    protected:
        D3D11_TEXTURE1D_DESC actualDesc;
        D3D11_TEXTURE1D_DESC desiredDesc;

        ComPtr<ID3D11Texture1D> texture;

        friend class RenderDeviceDX11;
    };
}



#endif //ENGINE_TEXTURE1DDX11_H
