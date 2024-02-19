#pragma once

#include "pch.h"
#include "Texture2dDX11.h"
#include "BufferDX11.h"
#include "BufferConfigDX11.h"
#include "Texture2dConfigDX11.h"
#include "ResourceData.h"

enum class ResourceType : U32 {
    kVertexBuffer = 0x010000,
    kIndexBuffer = 0x020000,
    kConstantBuffer = 0x030000,
    kStructuredBuffer = 0x040000,
    kByteAddressBuffer = 0x050000,
    kReservedForFuture = 0x060000,
    kTexture1D = 0x070000,
    kTexture2D = 0x080000,
    kTexture3D = 0x090000
};

namespace KGV::Render {
    class RenderDeviceDX11 {
    public:
        RenderDeviceDX11();

        virtual ~RenderDeviceDX11();

        bool init();

        void shutdown();

        //bool createTexture1D();
        Texture2dDX11 *createTexture2D(Texture2dConfigDX11 *config, ResourceData *data, ResourceType type);
        //bool createTexture3D();

        BufferDX11 *createBuffer(BufferConfigDX11 *config, ResourceData *data, ResourceType type);

        S32 createSwapchain();

        void createShaderResourceView();

        void createRenderTargetView();

    protected:
        ComPtr<ID3D11Device1> device;

        //std::vector<ComPtr<IDXGISwapChain1>> swapchains;
        std::vector<BufferDX11> buffers;
        std::vector<Texture2dDX11> textures;

    private:

    };
}