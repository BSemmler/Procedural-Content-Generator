#include "BufferConfigDX11.h"

namespace KGV::Render {
    void BufferConfigDX11::setDefaultConstantBuffer(U32 size, bool dynamic) {
        KGV_ASSERT_FALSE(size > 0);
        desc.ByteWidth = size;
        desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        desc.MiscFlags = 0;
        desc.StructureByteStride = 0;

        if (dynamic) {
            desc.Usage = D3D11_USAGE_DYNAMIC;
            desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        } else {
            desc.Usage = D3D11_USAGE_IMMUTABLE;
            desc.CPUAccessFlags = 0;
        }
    }

    void BufferConfigDX11::setDefaultVertexBuffer(U32 size, bool dynamic) {
        KGV_ASSERT_FALSE(size > 0);
        desc.ByteWidth = size;
        desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
        desc.MiscFlags = 0;
        desc.StructureByteStride = 0;

        if (dynamic) {
            desc.Usage = D3D11_USAGE_DYNAMIC;
            desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        } else {
            desc.Usage = D3D11_USAGE_IMMUTABLE;
            desc.CPUAccessFlags = 0;
        }
    }

    void BufferConfigDX11::setDefaultIndexBuffer(U32 size, bool dynamic) {
        KGV_ASSERT_FALSE(size > 0);
        desc.ByteWidth = size;
        desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        desc.MiscFlags = 0;
        desc.StructureByteStride = 0;

        if (dynamic) {
            desc.Usage = D3D11_USAGE_DYNAMIC;
            desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        } else {
            desc.Usage = D3D11_USAGE_IMMUTABLE;
            desc.CPUAccessFlags = 0;
        }
    }

    D3D11_BUFFER_DESC BufferConfigDX11::getDesc() {
        return desc;
    }

}
