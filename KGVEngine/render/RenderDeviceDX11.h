#ifndef RENDER_DEVICE_DX11_H_
#define RENDER_DEVICE_DX11_H_

#include "pch.h"
#include "render/resources/containers/Texture2dDX11.h"
#include "render/resources/containers/BufferDX11.h"
#include "render/resources/config/BufferConfigDX11.h"
#include "render/resources/config/Texture2dConfigDX11.h"
#include "render/resources/ResourceData.h"
#include "render/resources/config/SwapChainConfigDX11.h"
#include "render/resources/ResourceDX11.h"
#include "render/resources/containers/SwapChainDX11.h"
#include "render/resources/ResourceViewDX11.h"
#include "render/resources/views/ShaderResourceViewConfigDX11.h"
#include "render/resources/views/RenderTargetViewConfigDX11.h"
#include "render/resources/config/DepthStencilViewConfigDX11.h"
#include "render/resources/views/RenderTargetViewDX11.h"
#include "render/resources/views/ShaderResourceViewDX11.h"
#include "render/shader/ShaderDX11.h"
#include "render/pipeline/PipelineManagerDX11.h"
#include "render/resources/containers/DepthStencilViewDX11.h"
#include "render/resources/config/Texture1dConfigDX11.h"

namespace KGV::Render {
    class ResourceViewDX11;
    class SwapChainDX11;
    class PipelineManagerDX11;
//    class ShaderResourceViewConfigDX11;
//    class RenderTargetViewConfigDX11;

    class RenderDeviceDX11 {
    public:
        explicit RenderDeviceDX11(std::shared_ptr<spdlog::logger> logger);

        virtual ~RenderDeviceDX11();

        void init(D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_1, const ComPtr<IDXGIAdapter1>& _pAdapter = nullptr);

        void shutdown();

        ComPtr<IDXGIAdapter1> getOptimalAdapter(const ComPtr<IDXGIFactory2>& _pFactory = nullptr);
        std::vector<ComPtr<IDXGIAdapter1>> getAdapters(const ComPtr<IDXGIFactory2>& pFactory = nullptr);

        std::shared_ptr<ResourceViewDX11> CreateTexture1D(Texture1dConfigDX11 &texConfig, ResourceData *data,
                                                          ShaderResourceViewConfigDX11 *srvConfig = nullptr,
                                                          RenderTargetViewConfigDX11 *rtvConfig = nullptr);

        std::shared_ptr<ResourceViewDX11> CreateTexture2D(Texture2dConfigDX11 &texConfig, ResourceData *data,
                                                          ShaderResourceViewConfigDX11 *srvConfig = nullptr,
                                                          RenderTargetViewConfigDX11 *rtvConfig = nullptr,
                                                          DepthStencilViewConfigDX11 *dsvConfig = nullptr);

        std::shared_ptr<ResourceViewDX11> createVertexBuffer(BufferConfigDX11 &config, ResourceData *data);
        std::shared_ptr<ResourceViewDX11> createIndexBuffer(BufferConfigDX11 &config, ResourceData *data);
        std::shared_ptr<ResourceViewDX11> createConstantBuffer(BufferConfigDX11 &config, ResourceData *data);



        S32 createSwapChain(void* windowHandle, SwapChainConfigDX11& config);

        S32 createShaderResourceView(S32 resourceId, D3D11_SHADER_RESOURCE_VIEW_DESC* desc);

        S32 createRenderTargetView(S32 resourceId, D3D11_RENDER_TARGET_VIEW_DESC* desc);

        S32 createDepthStencilView(S32 resourceId, D3D11_DEPTH_STENCIL_VIEW_DESC* desc);

        S32 storeResource(std::unique_ptr<ResourceDX11> resource);
        ResourceDX11* getResourceById(S32 id);

        ShaderDX11* getShaderById(S32 id);

        SwapChainDX11* getSwapChainById(S32 id);

        RenderTargetViewDX11* getRtvById(S32 id);

        ShaderResourceViewDX11* getSrvById(S32 id);

        DepthStencilViewDX11* getDsvById(S32 id);

        ComPtr<ID3D11InputLayout> getInputLayoutById(S32 id);

        bool deleteResource(const std::shared_ptr<ResourceViewDX11>& resource);
        bool deleteResource(S32 index);

        S32 loadShader(const std::string& file, eShaderType type, bool isPreCompiled,
                       const std::string& function = "", const std::string& shaderModel = "");

        S32 createInputLayout( S32 shaderId, std::vector<D3D11_INPUT_ELEMENT_DESC> &inputElements);

        S32 createRasterizerState(D3D11_RASTERIZER_DESC &desc);

        S32 createDepthStencilState(D3D11_DEPTH_STENCIL_DESC &desc);

        S32 createSamplerState(D3D11_SAMPLER_DESC &desc);

        ComPtr<ID3D11SamplerState> getSamplerStateById(S32 id);

        ComPtr<ID3D11RasterizerState> getRasterizerStateById(S32 id);

        ComPtr<ID3D11DepthStencilState> getDsvStateById(S32 id);

        D3D11_VIEWPORT* getViewPortById(S32 id);

        D3D11_RECT* getScissorRectById(S32 id);

        S32 createViewPort(D3D11_VIEWPORT& viewPort);

        S32 createScissorRect(D3D11_RECT &rect);

        void presentSwapChain(S32 id, U32 syncInterval, U32 flags);

        std::shared_ptr<PipelineManagerDX11> getPipelineManager();

        ComPtr<ID3D11Device> getID3D11Device();
        ComPtr<ID3D11Device1> getID3D11Device1();
        ComPtr<ID3D11DeviceContext> getID3D11DeviceContext();
        ComPtr<ID3D11DeviceContext1> getID3D11DeviceContext1();

    protected:
        ComPtr<ID3D11Device> device;
        ComPtr<ID3D11Device1> device1;
        ComPtr<ID3D11DeviceContext> context;
        ComPtr<ID3D11DeviceContext1> context1;

        std::vector<std::unique_ptr<ShaderDX11>> shaders;
        std::vector<std::unique_ptr<SwapChainDX11>> swapChains;
        std::vector<ShaderResourceViewDX11> shaderResourceViews;
        std::vector<RenderTargetViewDX11> renderTargetViews;
        std::vector<DepthStencilViewDX11> depthStencilViews;
        std::vector<ComPtr<ID3D11DepthStencilState>> depthStencilStates;
        std::vector<ComPtr<ID3D11InputLayout>> inputLayouts;
        std::vector<ComPtr<ID3D11RasterizerState>> rasterStates;
        std::vector<ComPtr<ID3D11SamplerState>> samplerStates;
        std::vector<D3D11_VIEWPORT> viewPorts;
        std::vector<D3D11_RECT> scissorRects;
        std::vector<std::unique_ptr<ResourceDX11>> resources;
        std::vector<S32> availableResourceIds;
        std::shared_ptr<spdlog::logger> logger;

        ComPtr<ID3DBlob> compileShaderFromFile(const std::string& file, const std::string& function, const std::string& shaderModel);
        ComPtr<ID3DBlob> loadPrecompiledShader(const std::string& file);

    private:

    };
}

#endif // RENDER_DEVICE_DX11_H_