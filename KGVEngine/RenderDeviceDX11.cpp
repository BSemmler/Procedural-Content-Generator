#include "RenderDeviceDX11.h"
#include "StringUtil.h"

namespace KGV::Render {
    RenderDeviceDX11::RenderDeviceDX11(spdlog::logger logger) : logger(std::move(logger)) {

    }

    RenderDeviceDX11::~RenderDeviceDX11() {

    }

    void RenderDeviceDX11::shutdown() {

    }

    ComPtr<IDXGIAdapter1> RenderDeviceDX11::getOptimalAdapter(const ComPtr<IDXGIFactory2> &pFactory) {
        HRESULT hr;
        auto adapters = getAdapters(pFactory);

        if (adapters.empty()) {
            logger.error("Couldn't retrieve any IDXGIAdapters");
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
                pAdapter->GetDesc1(&desc);

                // Ignore the software adapter.
                if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) {
                    continue;
                }

                std::wstring wStr(desc.Description);
                logger.info("Found the adapter: {}, video memory: {}MB", utf8_encode(wStr), desc.DedicatedVideoMemory / 1024 / 1024);
            }
        } else {
            logger.critical("Couldn't create IDXGIFactory6, ({:0X})", hr);
        }

        return adapters;
    }


    void RenderDeviceDX11::init(D3D_FEATURE_LEVEL featureLevel, const ComPtr<IDXGIAdapter1> &_pAdapter) {
        ComPtr<IDXGIFactory2> pFactory2;
        const ComPtr<IDXGIAdapter1> &pAdapter1 = _pAdapter;

        HRESULT hr;
        if (!pAdapter1) {
            hr = CreateDXGIFactory2(0, __uuidof(IDXGIFactory2), &pFactory2);
            getOptimalAdapter(pFactory2);
        }

        if (pAdapter1) {
            logger.critical("Failed to acquire an adapter, aborting! ({:0X})");
            throw std::exception("Failed to acquire an adapter, aborting!");
        }

        S32 deviceCreateFlags = 0;
#ifdef _DEBUG
        deviceCreateFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
        D3D_FEATURE_LEVEL createdLevel;
        ComPtr<IDXGIAdapter> pAdapter;
        pAdapter1.As(&pAdapter);
        if (FAILED(hr = D3D11CreateDevice(pAdapter.Get(), D3D_DRIVER_TYPE_HARDWARE, nullptr, deviceCreateFlags,
                          &featureLevel, 1, D3D11_SDK_VERSION,device.GetAddressOf(),
                          &createdLevel, context.GetAddressOf()))) {
            logger.critical("Failed to create ID3D11Device. ({:0X})", hr);
            throw std::exception("Failed to create ID3D11Device");
        }

        if(FAILED(hr = device.As(&device1))) {
            logger.critical("Failed to create ID3D11Device1. ({:0X})", hr);
            throw std::exception("Failed to create ID3D11Device1");
        }

        if(FAILED(hr = context.As(&context1))) {
            logger.critical("Failed to create ID3D11DeviceContext1. ({:0X})", hr);
            throw std::exception("Failed to create ID3D11DeviceContext1");
        }
    }

    Texture2dDX11 *RenderDeviceDX11::createTexture2D(Texture2dConfigDX11 &config, ResourceData &data, eResourceType type) {
        auto *texture = new Texture2dDX11();
        HRESULT hr = device->CreateTexture2D(&config.desc, reinterpret_cast<D3D11_SUBRESOURCE_DATA *>(&data), &texture->texture);

        if (FAILED(hr)) {
            logger.error("Failed to create D3D11 Texture2D");

            // Trigger a breakpoint if we're in a debug build.
            KGV_debugBreak();

            delete texture;
            texture = nullptr;
        }

        if (texture) {
            texture->actualDesc = config.desc;
            texture->desiredDesc = texture->actualDesc;
        }

        return texture;
    }

//    BufferDX11 * RenderDeviceDX11::createBuffer(BufferConfigDX11 &config, ResourceData &data, eResourceType type) {
//        auto *buff = new BufferDX11();
//        HRESULT hr = device->CreateBuffer(&config.desc, reinterpret_cast<D3D11_SUBRESOURCE_DATA *>(&data), &buff->buffer);
//
//        if (FAILED(hr)) {
//            logger.error("Failed to create D3D11 Buffer!");
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
        if (FAILED(device.CopyTo(dxgiDevice.GetAddressOf()))) {
            logger.error("Failed to acquire DXGIDevice, reason: {}", GetLastError());
            return -1;
        }

        ComPtr<IDXGIFactory2> dxgiFactory2;
        if (FAILED(CreateDXGIFactory2(
                0, __uuidof(IDXGIFactory2), reinterpret_cast<void **>(dxgiFactory2.GetAddressOf())))) {
            logger.error("Failed to acquire DXGIFactory2 interface, reason: {}", GetLastError());
        }

        HRESULT hr;
        ComPtr<IDXGISwapChain1> swapChain;
        if (FAILED(hr = dxgiFactory2->CreateSwapChainForHwnd(device.Get(), static_cast<HWND>(hwnd), &config.getDesc(), nullptr, nullptr,
                                                             swapChain.GetAddressOf()))) {
            logger.error("Failed to create DXGI swap chain, reason: {}", GetLastError());
            MessageBox(nullptr, "Failed to create swap chain!", "Error!", MB_OK);
            return false;
        }

        config.setWidth(config.getDesc().Width);
        config.setHeight(config.getDesc().Height);

//        D3D11_VIEWPORT viewport;
//        memset(&viewport, 0, sizeof(D3D11_VIEWPORT));
//        viewport.TopLeftX = 0.0f;                             // left hand bounds of the viewport.
//        viewport.TopLeftY = 0.0f;                             // top bounds of the view port
//        viewport.Width = static_cast< float >( gWidth );   // width of the viewport
//        viewport.Height = static_cast< float >( gHeight );  // height of the view port
//
//        // Bind the viewport.
//        gpImmediateContext->RSSetViewports(1, &viewport);

        // Get the surface of the backbuffer and then create a render target view for that surface.
        ComPtr<ID3D11Texture2D> backBuffer;
        swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void **>(backBuffer.GetAddressOf()));

//        // Get the resource view for our back buffer.
//        if (FAILED(hr = device->CreateRenderTargetView(gpBackBuffer.Get(), nullptr, &gpBackBufferTarget))) {
//            OutputDebugStringA("Error: Failed to create create render target! Line: " + __LINE__);
//            MessageBox(0, L"Failed to create render target!", L"Error!", MB_OK);
//            return false;
//        }

        return true;
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
                    logger.trace("Successfully created Shader Resource View {} for resource {}", srvId, resourceId);
                    return static_cast<S32>(srvId);
                }
            }

        }

        logger.debug("Failed to create Shader Resource View, resource {}, valid resource: {}, resource initialized: {}, HRESULT: {}",
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
                    logger.trace("Successfully created Render Target View {} for resource {}", rtvId, resourceId);
                    return static_cast<S32>(rtvId);
                }
            }

        }

        logger.debug("Failed to create Render Target View, resource {}, valid resource: {}, resource initialized: {}, HRESULT: {}",
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
        this->availableIds.push_back(index);

        return true;
    }
}