//
// Created by Brett on 2024-02-19.
//
#include "ResourceViewDX11.h"

namespace KGV::Render {
    ResourceViewDX11::ResourceViewDX11(S32 _resourceId, BufferConfigDX11& _config, RenderDeviceDX11& _device,
                                       ShaderResourceViewConfigDX11* _srvConfig, RenderTargetViewConfigDX11* _rtvConfig) {
        D3D11_BUFFER_DESC desc = _config.getDesc();
        initResource(desc.BindFlags, _resourceId, _device, _srvConfig, _rtvConfig);

        bufferConfig = std::make_unique<BufferConfigDX11>();
        *bufferConfig = _config;
    }

    ResourceViewDX11::ResourceViewDX11(S32 _resourceId, Texture2dConfigDX11 &_config, RenderDeviceDX11 &_device, ShaderResourceViewConfigDX11 *_srvConfig,
                                       RenderTargetViewConfigDX11 *_rtvConfig) {
        D3D11_TEXTURE2D_DESC desc = _config.getDesc();
        initResource(desc.BindFlags, _resourceId, _device, _srvConfig, _rtvConfig);

        texture2dConfig = std::make_unique<Texture2dConfigDX11>();
        *texture2dConfig = _config;
    }

    void ResourceViewDX11::initResource(U32 _bindFlags, S32 _resourceId, RenderDeviceDX11 &_device, ShaderResourceViewConfigDX11 *_srvConfig,
                                        RenderTargetViewConfigDX11 *_rtvConfig) {
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

        if ((_bindFlags & D3D11_BIND_RENDER_TARGET) == D3D11_BIND_RENDER_TARGET ) {
            D3D11_RENDER_TARGET_VIEW_DESC* desc = rtvConfig ? rtvConfig->getDesc() : nullptr;
            rtvId = _device.createRenderTargetView(resourceId, desc)
        }

        if ((_bindFlags & D3D11_BIND_SHADER_RESOURCE) == D3D11_BIND_SHADER_RESOURCE ) {
            D3D11_SHADER_RESOURCE_VIEW_DESC* desc = srvConfig ? srvConfig->getDesc() : nullptr;
            rtvId = _device.createShaderResourceView(resourceId, desc)
        }
    }

    S32 ResourceViewDX11::getResourceId() const {
        return resourceId;
    }


}