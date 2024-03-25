#include "BufferConfigDX11.h"



namespace KGV::Render {
    namespace {
        void setBufferAccess(D3D11_BUFFER_DESC &desc, eBufferUpdateType updateType) {
            switch(updateType) {
                case kDynamic: {
                    desc.Usage = D3D11_USAGE_DYNAMIC;
                    desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
                    break;
                }
                case kImmutable: {
                    desc.Usage = D3D11_USAGE_IMMUTABLE;
                    desc.CPUAccessFlags = 0;
                    break;
                }
                case kDefault: {
                    desc.Usage = D3D11_USAGE_DEFAULT;
                    desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
                    break;
                }
                default: {
                    desc.Usage = D3D11_USAGE_DEFAULT;
                    desc.CPUAccessFlags = 0;
                }
            }
        }
    }

    void BufferConfigDX11::setDefaultConstantBuffer(U32 size, eBufferUpdateType updateType) {
        // KGV_ASSERT_FALSE(size > 0);
        desc.ByteWidth = size;
        desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        desc.MiscFlags = 0;
        desc.StructureByteStride = 0;

        setBufferAccess(desc, updateType);
    }

    void BufferConfigDX11::setDefaultVertexBuffer(U32 size, eBufferUpdateType updateType) {
        // KGV_ASSERT_FALSE(size > 0);
        desc.ByteWidth = size;
        desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        desc.MiscFlags = 0;
        desc.StructureByteStride = 0;

        setBufferAccess(desc, updateType);
    }

    void BufferConfigDX11::setDefaultIndexBuffer(U32 size, eBufferUpdateType updateType) {
        // KGV_ASSERT_FALSE(size > 0);
        desc.ByteWidth = size;
        desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
        desc.MiscFlags = 0;
        desc.StructureByteStride = 0;

        setBufferAccess(desc, updateType);
    }

    D3D11_BUFFER_DESC BufferConfigDX11::getDesc() {
        return desc;
    }

}
