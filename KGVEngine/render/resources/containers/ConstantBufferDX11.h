//
// Created by Brett on 2024-03-06.
//

#ifndef ENGINE_CONSTANTBUFFERDX11_H
#define ENGINE_CONSTANTBUFFERDX11_H

#include "pch.h"
#include "BufferDX11.h"

namespace KGV::Render {
    // TODO: Fill in this stub mapping.
    class ConstantBufferDX11 : public BufferDX11 {
    public:
        explicit ConstantBufferDX11( ComPtr<ID3D11Buffer> constantBuffer );

        eResourceType getResourceType() override;

    protected:

    };
}



#endif //ENGINE_CONSTANTBUFFERDX11_H
