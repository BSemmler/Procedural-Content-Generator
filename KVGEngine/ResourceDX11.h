//
// Created by Brett on 2024-02-19.
//

#ifndef ENGINE_RESOURCE_DX11_H
#define ENGINE_RESOURCE_DX11_H


#include "pch.h"
namespace KGV::Render {
    enum class eResourceType : U32 {
        kVertexBuffer       = 0x01,
        kIndexBuffer        = 0x02,
        kConstantBuffer     = 0x03,
        kStructuredBuffer   = 0x04,
        kByteAddressBuffer  = 0x05,
        kReservedForFuture  = 0x06,
        kTexture1D          = 0x07,
        kTexture2D          = 0x08,
        kTexture3D          = 0x09
    };

    class ResourceDX11 {
    public:
        virtual eResourceType getResourceType() = 0;
        virtual ID3D11Resource* getResource() = 0;
    };
}



#endif //ENGINE_RESOURCEDX11_H
