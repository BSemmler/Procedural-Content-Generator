//
// Created by Brett on 2024-02-19.
//
#include "ResourceViewDX11.h"

namespace KGV::Render {
    ResourceViewDX11::ResourceViewDX11(S32 _resourceId, BufferConfigDX11* _config, RenderDeviceDX11* _device,
                                       ShaderResourceViewConfigDX11* _srvConfig, RenderTargetViewConfigDX11* _rtvConfig) {
        D3D11_BUFFER_DESC desc = _config->getDesc();
        initResource(desc.BindFlags, _resourceId, _device, _srvConfig, _rtvConfig);

        bufferConfig = std::make_unique<BufferConfigDX11>();
        *bufferConfig = *_config;
    }

    ResourceViewDX11::ResourceViewDX11(S32 _resourceId, Texture1dConfigDX11 *_config, RenderDeviceDX11 *_device,
                                       ShaderResourceViewConfigDX11 *_srvConfig, RenderTargetViewConfigDX11 *_rtvConfig) {

        D3D11_TEXTURE1D_DESC desc = _config->getDesc();
        initResource(desc.BindFlags, _resourceId, _device, _srvConfig, _rtvConfig);

        texture1dConfig = std::make_unique<Texture1dConfigDX11>();
        *texture1dConfig = *_config;
    }

    ResourceViewDX11::ResourceViewDX11(S32 _resourceId, Texture2dConfigDX11 *_config, RenderDeviceDX11 *_device, ShaderResourceViewConfigDX11 *_srvConfig,
                                       RenderTargetViewConfigDX11 *_rtvConfig, DepthStencilViewConfigDX11 *_dsvConfig) {
        D3D11_TEXTURE2D_DESC desc = _config->getDesc();
        initResource(desc.BindFlags, _resourceId, _device, _srvConfig, _rtvConfig);

        texture2dConfig = std::make_unique<Texture2dConfigDX11>();
        *texture2dConfig = *_config;
    }

    void ResourceViewDX11::initResource(U32 _bindFlags, S32 _resourceId, RenderDeviceDX11 *_device, ShaderResourceViewConfigDX11 *_srvConfig,
                                        RenderTargetViewConfigDX11 *_rtvConfig, DepthStencilViewConfigDX11 *_dsvConfig) {
        texture2dConfig = nullptr;
        bufferConfig    = nullptr;
        rtvConfig       = nullptr;
        srvConfig       = nullptr;

        resourceId  = _resourceId;
        rtvId       = 0;
        srvId       = 0;

        if (_rtvConfig) {
            rtvConfig = std::make_unique<RenderTargetViewConfigDX11>();
            *rtvConfig = *_rtvConfig;
        }

        if (_srvConfig) {
            srvConfig = std::make_unique<ShaderResourceViewConfigDX11>();
            *srvConfig = *_srvConfig;
        }

        if (_dsvConfig) {
            dsvConfig = std::make_unique<DepthStencilViewConfigDX11>();
            *dsvConfig = *_dsvConfig;
        }

        if ((_bindFlags & D3D11_BIND_RENDER_TARGET) == D3D11_BIND_RENDER_TARGET ) {
            D3D11_RENDER_TARGET_VIEW_DESC* desc = rtvConfig ? &rtvConfig->getDesc() : nullptr;
            rtvId = _device->createRenderTargetView(resourceId, desc);
        }

        if ((_bindFlags & D3D11_BIND_SHADER_RESOURCE) == D3D11_BIND_SHADER_RESOURCE ) {
            D3D11_SHADER_RESOURCE_VIEW_DESC* desc = srvConfig ? &srvConfig->getDesc() : nullptr;
            srvId = _device->createShaderResourceView(resourceId, desc);
        }

        if ((_bindFlags & D3D11_BIND_DEPTH_STENCIL) == D3D11_BIND_DEPTH_STENCIL) {
            D3D11_DEPTH_STENCIL_VIEW_DESC* desc = dsvConfig ? &dsvConfig->getDesc() : nullptr;
            dsvId = _device->createDepthStencilView(resourceId, desc);
        }
    }

    S32 ResourceViewDX11::getResourceId() const {
        return resourceId;
    }

    S32 ResourceViewDX11::getSrvId() const {
        return srvId;
    }

    S32 ResourceViewDX11::getRtvId() const {
        return rtvId;
    }

    S32 ResourceViewDX11::getDsvId() const {
        return dsvId;
    }

    const ShaderResourceViewConfigDX11* ResourceViewDX11::getSrvConfig() const {
        return srvConfig.get();
    }

    const RenderTargetViewConfigDX11* ResourceViewDX11::getRtvConfig() const {
        return rtvConfig.get();
    }

    const DepthStencilViewConfigDX11 *ResourceViewDX11::getDsvConfig() const {
        return dsvConfig.get();
    }

    const BufferConfigDX11* ResourceViewDX11::getBufferConfig() const {
        return bufferConfig.get();
    }


    const Texture2dConfigDX11* ResourceViewDX11::getTexture2DConfig() const {
        return texture2dConfig.get();
    }
}