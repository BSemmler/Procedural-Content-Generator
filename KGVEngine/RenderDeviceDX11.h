#ifndef RENDER_DEVICE_DX11_H_
#define RENDER_DEVICE_DX11_H_

#include "pch.h"
#include "Texture2dDX11.h"
#include "BufferDX11.h"
#include "BufferConfigDX11.h"
#include "Texture2dConfigDX11.h"
#include "ResourceData.h"
#include "SwapChainConfigDX11.h"
#include "ResourceDX11.h"
#include "SwapChainDX11.h"
#include "ResourceViewDX11.h"
#include "ShaderResourceViewConfigDX11.h"
#include "RenderTargetViewConfigDX11.h"
#include "RenderTargetViewDX11.h"
#include "ShaderResourceViewDX11.h"

namespace KGV::Render {
    class ResourceViewDX11;
    class SwapChainDX11;
//    class ShaderResourceViewConfigDX11;
//    class RenderTargetViewConfigDX11;

    class RenderDeviceDX11 {
    public:
        RenderDeviceDX11(spdlog::logger logger);

        virtual ~RenderDeviceDX11();

        void init(D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_1, const ComPtr<IDXGIAdapter1>& _pAdapter = nullptr);

        void shutdown();

        ComPtr<IDXGIAdapter1> getOptimalAdapter(const ComPtr<IDXGIFactory2>& _pFactory = nullptr);
        std::vector<ComPtr<IDXGIAdapter1>> getAdapters(const ComPtr<IDXGIFactory2>& pFactory = nullptr);

        //bool createTexture1D();
        std::shared_ptr<ResourceViewDX11> createTexture2D(Texture2dConfigDX11 &texConfig, ResourceData &data,
                                                           ShaderResourceViewConfigDX11 &srvConfig, RenderTargetViewConfigDX11 &rtvConfig);
        //bool createTexture3D();

//        std::shared_ptr<ResourceViewDX11> createBuffer(BufferConfigDX11 &config, ResourceData &data, ShaderResourceViewConfigDX11 srvConfig, RenderTargetViewConfigDX11 rtvConfig);

        S32 createSwapChain(void* windowHandle, SwapChainConfigDX11& config);

        S32 createShaderResourceView(S32 resourceId, D3D11_SHADER_RESOURCE_VIEW_DESC* desc);

        S32 createRenderTargetView(S32 resourceId, D3D11_RENDER_TARGET_VIEW_DESC* desc);

        S32 storeResource(std::unique_ptr<ResourceDX11> resource);
        ResourceDX11* getResourceById(S32 index);
        bool deleteResource(const std::shared_ptr<ResourceViewDX11>& resource);
        bool deleteResource(S32 index);

    protected:
        ComPtr<ID3D11Device> device;
        ComPtr<ID3D11Device1> device1;
        ComPtr<ID3D11DeviceContext> context;
        ComPtr<ID3D11DeviceContext1> context1;

        std::vector<std::unique_ptr<SwapChainDX11>> swapChains;
        std::vector<ShaderResourceViewDX11> shaderResourceViews;
        std::vector<RenderTargetViewDX11> renderTargetViews;
        std::vector<std::unique_ptr<ResourceDX11>> resources;
        std::vector<S32> availableIds;
        spdlog::logger logger;

    private:

    };
}

#endif // RENDER_DEVICE_DX11_H_