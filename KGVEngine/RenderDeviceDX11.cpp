#include "RenderDeviceDX11.h"

#include <utility>
#include "StringUtil.h"
#include "VertexBufferDX11.h"
#include "IndexBufferDX11.h"
#include "VertexShaderDX11.h"
#include "PixelShaderDX11.h"
#include "ConstantBufferDX11.h"

namespace KGV::Render {
    RenderDeviceDX11::RenderDeviceDX11(std::shared_ptr<spdlog::logger> logger) : logger(std::move(logger)) {

    }

    RenderDeviceDX11::~RenderDeviceDX11() {

    }

    void RenderDeviceDX11::shutdown() {

    }

    ComPtr<IDXGIAdapter1> RenderDeviceDX11::getOptimalAdapter(const ComPtr<IDXGIFactory2> &pFactory) {
        HRESULT hr;
        auto adapters = getAdapters(pFactory);

        if (adapters.empty()) {
            logger->error("Couldn't retrieve any IDXGIAdapters");
            return nullptr;
        }

        return adapters[0];
    }

    std::vector<ComPtr<IDXGIAdapter1>> RenderDeviceDX11::getAdapters(const ComPtr<IDXGIFactory2> &pFactory) {
        ComPtr<IDXGIAdapter1> pAdapter;
        ComPtr<IDXGIFactory6> pFactory6;

        std::vector<ComPtr<IDXGIAdapter1>> adapters;

        HRESULT hr;
        if (SUCCEEDED(hr = pFactory.As(&pFactory6))) {
            adapters.emplace_back();
            for (auto i = 0; DXGI_ERROR_NOT_FOUND != pFactory6->EnumAdapterByGpuPreference(
                    i, DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE, __uuidof(IDXGIAdapter1), &adapters[i]); i++) {
                DXGI_ADAPTER_DESC1 desc;
                if (adapters[i])
                    adapters[i]->GetDesc1(&desc);

                std::wstring wStr(desc.Description);

                logger->info("Found the adapter: {}, video memory: {}MB", utf8_encode(wStr), desc.DedicatedVideoMemory / 1024 / 1024);
                adapters.emplace_back();
            }
        } else {
            logger->critical("Couldn't create IDXGIFactory6, ({:0X})", hr);
        }

        return adapters;
    }


    void RenderDeviceDX11::init(D3D_FEATURE_LEVEL featureLevel, const ComPtr<IDXGIAdapter1> &_pAdapter) {
        ComPtr<IDXGIFactory2> pFactory2;
        ComPtr<IDXGIAdapter1> pAdapter1 = _pAdapter;

        HRESULT hr;
        if (!pAdapter1) {
            hr = CreateDXGIFactory2(0, __uuidof(IDXGIFactory2), &pFactory2);
            pAdapter1 = getOptimalAdapter(pFactory2);
        }

        if (!pAdapter1) {
            logger->critical("Failed to acquire an adapter, aborting! ({:0X})");
            throw std::exception("Failed to acquire an adapter, aborting!");
        }

        S32 deviceCreateFlags = 0;
#ifdef _DEBUG
        deviceCreateFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
        D3D_FEATURE_LEVEL createdLevel;
        ComPtr<IDXGIAdapter> pAdapter;
        pAdapter1.As(&pAdapter);
        if (FAILED(hr = D3D11CreateDevice(pAdapter.Get(), D3D_DRIVER_TYPE_UNKNOWN, nullptr, deviceCreateFlags,
                          &featureLevel, 1, D3D11_SDK_VERSION,device.GetAddressOf(),
                          &createdLevel, context.GetAddressOf()))) {
            logger->critical("Failed to create ID3D11Device. ({:0X})", hr);
            throw std::exception("Failed to create ID3D11Device");
        }

        if(FAILED(hr = device.As(&device1))) {
            logger->critical("Failed to create ID3D11Device1. ({:0X})", hr);
            throw std::exception("Failed to create ID3D11Device1");
        }

        if(FAILED(hr = context.As(&context1))) {
            logger->critical("Failed to create ID3D11DeviceContext1. ({:0X})", hr);
            throw std::exception("Failed to create ID3D11DeviceContext1");
        }
    }

    std::shared_ptr<ResourceViewDX11> RenderDeviceDX11::createTexture2D(
            Texture2dConfigDX11 &texConfig, ResourceData &data,
            ShaderResourceViewConfigDX11 &srvConfig, RenderTargetViewConfigDX11 &rtvConfig) {
        ComPtr<ID3D11Texture2D> tex;
        HRESULT hr = device->CreateTexture2D(&texConfig.desc, reinterpret_cast<D3D11_SUBRESOURCE_DATA *>(&data), tex.GetAddressOf());

        if (FAILED(hr)) {
            logger->error("Failed to create D3D11 Texture2D");

            // Trigger a breakpoint if we're in a debug build.
            // KGV_debugBreak();
        }

        auto texture = std::make_unique<Texture2dDX11>(tex);
        texture->desiredDesc = texConfig.desc;
        S32 resourceId = storeResource(std::move(texture));
        return std::make_shared<ResourceViewDX11>(resourceId, &texConfig, this, &srvConfig, &rtvConfig);
    }

//    BufferDX11 * RenderDeviceDX11::createBuffer(BufferConfigDX11 &config, ResourceData &data, eResourceType type) {
//        auto *buff = new BufferDX11();
//        HRESULT hr = device->CreateBuffer(&config.desc, reinterpret_cast<D3D11_SUBRESOURCE_DATA *>(&data), &buff->buffer);
//
//        if (FAILED(hr)) {
//            logger->error("Failed to create D3D11 Buffer!");
//            // Trigger a breakpoint if we're in a debug build.
//            KGV_debugBreak();
//            delete buff;
//            buff = nullptr;
//        }
//
//        if (buff) {
//            buff->actualDesc = config.desc;
//            buff->desiredDesc = buff->actualDesc;
//        }
//
//        return buff;
//    }

    S32 RenderDeviceDX11::createSwapChain(void *hwnd, SwapChainConfigDX11 &config) {
        ComPtr<IDXGIDevice> dxgiDevice;

        HRESULT hr;
        if (FAILED(hr = device.CopyTo(dxgiDevice.GetAddressOf()))) {
            logger->error("Failed to acquire DXGIDevice, reason: {:0X}", hr);
            return -1;
        }

        ComPtr<IDXGIFactory2> dxgiFactory2;
        if (FAILED(hr = CreateDXGIFactory2(
                0, __uuidof(IDXGIFactory2), reinterpret_cast<void **>(dxgiFactory2.GetAddressOf())))) {
            logger->error("Failed to acquire DXGIFactory2 interface, reason: {:0X}", hr);
            return -1;
        }

        
        ComPtr<IDXGISwapChain1> swapChain;
        auto desc = config.getDesc();
        if (FAILED(hr = dxgiFactory2->CreateSwapChainForHwnd(device.Get(), static_cast<HWND>(hwnd), &desc, nullptr, nullptr,
                                                             swapChain.GetAddressOf()))) {
            logger->error("Failed to create DXGI swap chain, reason: {:0X}", hr);
            return -1;
        }

        config.setWidth(static_cast<S32>(config.getDesc().Width));
        config.setHeight(static_cast<S32>(config.getDesc().Height));


        ComPtr<ID3D11Texture2D> backBuffer;
        hr = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void **>(backBuffer.GetAddressOf()));

        if (FAILED(hr)) {
            logger->error("Failed to get back buffer of swap chain. ({:0X})", hr);
            return -1;
        }

        S32 resourceId = storeResource(std::make_unique<Texture2dDX11>(backBuffer));
        Texture2dConfigDX11 texConfig;
        backBuffer->GetDesc(&texConfig.desc);
        auto backBufferRes = std::make_shared<ResourceViewDX11>(resourceId, &texConfig, this);

        swapChains.emplace_back(new SwapChainDX11(swapChain, backBufferRes));

        return static_cast<S32>(swapChains.size() - 1);
    }

    S32 RenderDeviceDX11::createShaderResourceView(S32 resourceId, D3D11_SHADER_RESOURCE_VIEW_DESC *desc) {
        auto resource = getResourceById(resourceId);

        bool isResourceIdValid = false;
        bool isResourceInitialized = false;
        HRESULT hr = 0;

        if (resource) {
            isResourceIdValid = true;
            if (resource->getResource()) {
                isResourceInitialized = true;
                ComPtr<ID3D11ShaderResourceView> view;

                hr = device->CreateShaderResourceView(resource->getResource(), desc, &view);
                if (view) {
                    ShaderResourceViewDX11 srv(view);
                    shaderResourceViews.push_back(srv);
                    auto srvId = shaderResourceViews.size() - 1;
                    logger->trace("Successfully created Shader Resource View {} for resource {}", srvId, resourceId);
                    return static_cast<S32>(srvId);
                }
            }

        }

        logger->debug("Failed to create Shader Resource View, resource {}, valid resource: {}, resource initialized: {}, HRESULT: {}",
                     resourceId, isResourceIdValid, isResourceInitialized, hr);

        return -1;
    }

    S32 RenderDeviceDX11::createRenderTargetView(S32 resourceId, D3D11_RENDER_TARGET_VIEW_DESC *desc) {
        auto resource = getResourceById(resourceId);

        bool isResourceIdValid = false;
        bool isResourceInitialized = false;
        HRESULT hr = 0;

        if (resource) {
            isResourceIdValid = true;
            if (resource->getResource()) {
                isResourceInitialized = true;
                ComPtr<ID3D11RenderTargetView> view;

                hr = device->CreateRenderTargetView(resource->getResource(), desc, &view);
                if (view) {
                    RenderTargetViewDX11 rtv(view);
                    renderTargetViews.push_back(rtv);
                    auto rtvId = renderTargetViews.size() - 1;
                    logger->trace("Successfully created Render Target View {} for resource {}", rtvId, resourceId);
                    return static_cast<S32>(rtvId);
                }
            }

        }

        logger->debug("Failed to create Render Target View, resource {}, valid resource: {}, resource initialized: {}, HRESULT: {}",
                     resourceId, isResourceIdValid, isResourceInitialized, hr);

        return -1;
    }

    ResourceDX11 *RenderDeviceDX11::getResourceById(S32 index) {
        if (index < 0 || index >= resources.size())
            return nullptr;

        return resources[index].get();
    }

    bool RenderDeviceDX11::deleteResource(const std::shared_ptr<ResourceViewDX11> &resource) {
        return this->deleteResource(resource->getResourceId());
    }

    bool RenderDeviceDX11::deleteResource(int index) {
        if (index < 0 || index >= resources.size())
            return false;

        this->resources[index].reset(nullptr);
        this->availableResourceIds.push_back(index);

        return true;
    }

    S32 RenderDeviceDX11::storeResource(std::unique_ptr<ResourceDX11> resource) {
        S32 nextId;
        if (!availableResourceIds.empty())
            nextId = availableResourceIds.back();
        else
            nextId = resources.size();

        resources.emplace_back(std::move(resource));
        return nextId;
    }

    std::shared_ptr<ResourceViewDX11> RenderDeviceDX11::createIndexBuffer(BufferConfigDX11 &config, ResourceData *data) {
        logger->trace("Creating new index buffer.");

        ComPtr<ID3D11Buffer> buffer;
        HRESULT hr = device->CreateBuffer(&config.desc,
                                          reinterpret_cast<D3D11_SUBRESOURCE_DATA*>(data), buffer.GetAddressOf());

        if (buffer) {
            auto b = std::make_unique<IndexBufferDX11>(buffer);
            b->setDesiredDesc(config.getDesc());
            int resourceId = storeResource(std::move(b));
            auto resourceView = std::make_shared<ResourceViewDX11>(resourceId, &config, this);

            return resourceView;
        }

        logger->error("Failed to create index buffer. ({})", hr);
        return {};
    }

    std::shared_ptr<ResourceViewDX11> RenderDeviceDX11::createVertexBuffer(BufferConfigDX11 &config, ResourceData *data) {
        logger->trace("Creating new vertex buffer.");

        ComPtr<ID3D11Buffer> buffer;
        HRESULT hr = device->CreateBuffer(&config.desc,
                                          reinterpret_cast<D3D11_SUBRESOURCE_DATA*>(data), buffer.GetAddressOf());

        if (buffer) {
            auto b = std::make_unique<VertexBufferDX11>(buffer);
            b->setDesiredDesc(config.getDesc());
            int resourceId = storeResource(std::move(b));
            auto resourceView = std::make_shared<ResourceViewDX11>(resourceId, &config, this);

            return resourceView;
        }

        logger->error("Failed to create vertex buffer. ({})", hr);
        return {};
    }

    std::shared_ptr<ResourceViewDX11> RenderDeviceDX11::createConstantBuffer(BufferConfigDX11 &config, ResourceData *data) {
        logger->trace("Creating new vertex buffer.");

        ComPtr<ID3D11Buffer> buffer;
        HRESULT hr = device->CreateBuffer(&config.desc,
                                          reinterpret_cast<D3D11_SUBRESOURCE_DATA*>(data), buffer.GetAddressOf());

        if (buffer) {
            auto b = std::make_unique<ConstantBufferDX11>(buffer);
            b->setDesiredDesc(config.getDesc());
            int resourceId = storeResource(std::move(b));
            auto resourceView = std::make_shared<ResourceViewDX11>(resourceId, &config, this);

            return resourceView;
        }

        logger->error("Failed to create vertex buffer. ({})", hr);
        return {};
    }

    S32 RenderDeviceDX11::loadShader(const std::string& file, eShaderType type, bool isPreCompiled,
                                     const std::string& function, const std::string& shaderModel) {

        ComPtr<ID3DBlob> compiledShader;
        if (isPreCompiled)
            compiledShader = loadPrecompiledShader(file);
        else
            compiledShader = compileShaderFromFile(file, function, shaderModel);

        if (!compiledShader)
            return -1;

        HRESULT hr = S_OK;
        std::unique_ptr<ShaderDX11> shaderWrapper;
        switch (type) {
            case kVertex: {
                ComPtr<ID3D11VertexShader> shader;
                hr = device->CreateVertexShader(
                        compiledShader->GetBufferPointer(),
                        compiledShader->GetBufferSize(),
                        nullptr,
                        shader.GetAddressOf());

                shaderWrapper = std::make_unique<VertexShaderDX11>(file, function, shaderModel, compiledShader, shader);
                break;
            }
            case kPixel: {
                ComPtr<ID3D11PixelShader> shader;
                hr = device->CreatePixelShader(
                        compiledShader->GetBufferPointer(),
                        compiledShader->GetBufferSize(),
                        nullptr,
                        shader.GetAddressOf());

                shaderWrapper = std::make_unique<PixelShaderDX11>(file, function, shaderModel, compiledShader, shader);
                break;
            }
        }

        if (FAILED(hr)) {
            logger->error("Failed to create shader from {} {} {}", file, function, shaderModel);
            return -1;
        }

        shaders.emplace_back(std::move(shaderWrapper));

        return static_cast<S32>(shaders.size() - 1);
    }

    std::pair<std::unique_ptr<char[]>, S32> loadFile(const std::string& file, const std::shared_ptr<spdlog::logger>& logger) {
        std::ifstream fileReader(file, std::ifstream::binary);

        if (!fileReader.is_open()) {
            logger->error("Failed to open the file {}", file);
            return { nullptr, -1 };
        }

        fileReader.seekg(0, std::ios::end);
        auto length = fileReader.tellg();

        fileReader.seekg(0, std::ios::beg);
        auto buffer = std::make_unique<char[]>(length);
        fileReader.read(buffer.get(), length);

        fileReader.close();
        return { std::move(buffer), length };
    }

    ComPtr<ID3DBlob> RenderDeviceDX11::loadPrecompiledShader(const std::string& file) {
        auto shaderFile = loadFile(file, logger);

        if (shaderFile.second == -1)
            return nullptr;

        ComPtr<ID3DBlob> blob;
        HRESULT hr = D3DCreateBlob(shaderFile.second, blob.GetAddressOf());

        if (FAILED(hr)) {
            logger->error("Failed to load compiled shader into blob, file: {}", file);
            return nullptr;
        }


        memcpy(blob->GetBufferPointer(), shaderFile.first.get(), shaderFile.second);
        return blob;
    }

    ComPtr<ID3DBlob> RenderDeviceDX11::compileShaderFromFile(const std::string& file, const std::string& function,
                                                             const std::string& shaderModel) {
        auto shaderFile = loadFile(file, logger);

        if (shaderFile.second == -1)
            return nullptr;

        ComPtr<ID3DBlob> compiledShader;
        ComPtr<ID3DBlob> errorMessages;

        U32 flags = D3DCOMPILE_PACK_MATRIX_ROW_MAJOR;
#ifdef _DEBUG
        flags |= D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

        HRESULT hr = D3DCompile(shaderFile.first.get(),
                                shaderFile.second,
                                nullptr,
                                nullptr,
                                nullptr,
                                function.c_str(),
                                shaderModel.c_str(),
                                flags,
                                0,
                                &compiledShader,
                                &errorMessages);

        if (FAILED(hr)) {
            logger->error("Failed to compile shader {} {} with the following message: {}", file, function, (const char*)errorMessages->GetBufferPointer());
            return nullptr;
        }

        return compiledShader;
    }

    S32 RenderDeviceDX11::createInputLayout(S32 shaderId, std::vector<D3D11_INPUT_ELEMENT_DESC> &inputElements) {
        auto compiledShader = shaders[shaderId]->getCompiledShader();
        ComPtr<ID3D11InputLayout> inputLayout;

        HRESULT hr = device->CreateInputLayout(inputElements.data(),
                                               inputElements.size(),
                                               compiledShader->GetBufferPointer(),
                                               compiledShader->GetBufferSize(),
                                               inputLayout.GetAddressOf());

        if (FAILED(hr)) {
            logger->error("Failed to create input layout for shader ID {}", shaderId);
            return -1;
        }

        inputLayouts.emplace_back(inputLayout);
        return static_cast<S32>(inputLayouts.size() - 1);
    }

    ShaderDX11 *RenderDeviceDX11::getShaderById(S32 id) {
        if (id < shaders.size() && id >= 0)
            return shaders[id].get();

        return nullptr;
    }

    SwapChainDX11 *RenderDeviceDX11::getSwapChainById(S32 id) {
        if (id < swapChains.size() && id >= 0)
            return swapChains[id].get();

        return nullptr;
    }

    RenderTargetViewDX11 *RenderDeviceDX11::getRtvById(S32 id) {
        if (id < renderTargetViews.size() && id >= 0)
            return &renderTargetViews[id];

        return nullptr;
    }

    ShaderResourceViewDX11* RenderDeviceDX11::getSrvById(S32 id) {
        if (id < shaderResourceViews.size() && id >= 0)
            return &shaderResourceViews[id];

        return nullptr;
    }

    ComPtr<ID3D11InputLayout> RenderDeviceDX11::getInputLayoutById(S32 id) {
        if (id < inputLayouts.size() && id >= 0)
            return inputLayouts[id];

        return {};
    }

    void RenderDeviceDX11::presentSwapChain(S32 id, U32 syncInterval, U32 flags) {
        if (id >= 0 && id < swapChains.size())
            swapChains[id]->getSwapChain()->Present(syncInterval, flags);
    }

    S32 RenderDeviceDX11::createRasterizerState(D3D11_RASTERIZER_DESC &desc) {
        ComPtr<ID3D11RasterizerState> rasterState;
        HRESULT hr = device->CreateRasterizerState(&desc, rasterState.GetAddressOf());

        if (FAILED(hr)) {
            logger->error("Failed to create rasterizer state!");
            return -1;
        }

        rasterStates.emplace_back(rasterState);

        return static_cast<S32>(rasterStates.size() - 1);
    }

    S32 RenderDeviceDX11::createViewPort(D3D11_VIEWPORT &viewPort) {
        viewPorts.emplace_back(viewPort);

        return static_cast<S32>(viewPorts.size() - 1);
    }

    S32 RenderDeviceDX11::createScissorRect(D3D11_RECT &rect) {
        scissorRects.emplace_back(rect);

        return static_cast<S32>(scissorRects.size() - 1);
    }

    ComPtr<ID3D11RasterizerState> RenderDeviceDX11::getRasterizerStateById(S32 id) {
        if (id < rasterStates.size() && id >= 0)
            return rasterStates[id];
        return nullptr;
    }

    D3D11_VIEWPORT *RenderDeviceDX11::getViewPortById(S32 id) {
        if (id < viewPorts.size() && id >= 0)
            return &viewPorts[id];
        return nullptr;
    }

    D3D11_RECT *RenderDeviceDX11::getScissorRectById(S32 id) {
        if (id < scissorRects.size() && id >= 0)
            return &scissorRects[id];
        return nullptr;
    }

    std::shared_ptr<PipelineManagerDX11> RenderDeviceDX11::getPipelineManager() {
        RenderDeviceDX11* dev = this;
        return std::make_shared<PipelineManagerDX11>(dev, context, context1);
    }


}