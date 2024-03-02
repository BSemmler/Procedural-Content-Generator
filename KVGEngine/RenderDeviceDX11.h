#ifndef RENDER_DEVICE_DX11_H_
#define RENDER_DEVICE_DX11_H_

#include "pch.h"
#include "Texture2dDX11.h"
#include "BufferDX11.h"
#include "BufferConfigDX11.h"
#include "Texture2dConfigDX11.h"
#include "ResourceData.h"
#include "SwapChainConfigDX11.h"
#include "ShaderResourceViewDX11.h"
#include "RenderTargetViewDX11.h"
#include "ResourceDX11.h"
#include "ResourceViewDX11.h"

namespace KGV::Render {
    class ResourceViewDX11;

    class RenderDeviceDX11 {
    public:
        RenderDeviceDX11();

        virtual ~RenderDeviceDX11();

        bool init();

        void shutdown();

        //bool createTexture1D();
        Texture2dDX11 * createTexture2D(Texture2dConfigDX11 &config, ResourceData &data, eResourceType type);
        //bool createTexture3D();

//        BufferDX11 * createBuffer(BufferConfigDX11 &config, ResourceData &data, eResourceType type);

        S32 createSwapChain(void* windowHandle, SwapChainConfigDX11& config);

        S32 createShaderResourceView(S32 resourceId, D3D11_SHADER_RESOURCE_VIEW_DESC* desc);

        S32 createRenderTargetView(S32 resourceId, D3D11_RENDER_TARGET_VIEW_DESC* desc);

        ResourceDX11* getResourceById(S32 index);

        bool deleteResource(const std::shared_ptr<ResourceViewDX11>& resource);
        bool deleteResource(S32 index);

    protected:
        ComPtr<ID3D11Device1> device;
        std::vector<ComPtr<IDXGISwapChain1>> swapChains;
        std::vector<BufferDX11> buffers;
        std::vector<Texture2dDX11> textures;
        std::vector<ShaderResourceViewDX11> shaderResourceViews;
        std::vector<RenderTargetViewDX11> renderTargetViews;
        std::vector<std::unique_ptr<ResourceDX11>> resources;
        std::vector<S32> availableIds;
        spdlog::logger logger;

    private:

    };
}

#endif // RENDER_DEVICE_DX11_H_