#pragma once

#include "pch.h"
#include "render/RenderDeviceDX11.h"
#include "render/resources/views/ShaderResourceViewConfigDX11.h"
#include "render/resources/views/RenderTargetViewConfigDX11.h"
#include "render/resources/config/DepthStencilViewConfigDX11.h"

namespace KGV::Render {
    class RenderDeviceDX11;
    class BufferConfigDX11;
    class RenderTargetViewConfigDX11;
    class ShaderResourceViewConfigDX11;
    class DepthStencilViewConfigDX11;
    class Texture1dConfigDX11;
    class Texture2dConfigDX11;

    class ResourceViewDX11 {
    public:
        ResourceViewDX11(S32 _resourceId, BufferConfigDX11 *_config, RenderDeviceDX11 *_device,
                         ShaderResourceViewConfigDX11 *_srvConfig = nullptr, RenderTargetViewConfigDX11 *_rtvConfig = nullptr);

        ResourceViewDX11(S32 _resourceId, Texture1dConfigDX11 *_config, RenderDeviceDX11* _device,
                         ShaderResourceViewConfigDX11 *_srvConfig = nullptr,
                         RenderTargetViewConfigDX11 *_rtvConfig = nullptr);

        ResourceViewDX11(S32 _resourceId, Texture2dConfigDX11 *_config, RenderDeviceDX11* _device,
                         ShaderResourceViewConfigDX11 *_srvConfig = nullptr,
                         RenderTargetViewConfigDX11 *_rtvConfig = nullptr,
                         DepthStencilViewConfigDX11 *_dsvConfig = nullptr);

        S32 getResourceId() const;
        S32 getSrvId() const;
        S32 getRtvId() const;
        S32 getDsvId() const;

        const DepthStencilViewConfigDX11* getDsvConfig() const;
        const ShaderResourceViewConfigDX11* getSrvConfig() const;
        const RenderTargetViewConfigDX11* getRtvConfig() const;
        const BufferConfigDX11* getBufferConfig() const;
        const Texture2dConfigDX11* getTexture2DConfig() const;


    protected:
        void initResource(U32 _bindFlags, S32 _resourceId, RenderDeviceDX11 *_device,
                          ShaderResourceViewConfigDX11 *_srvConfig = nullptr,
                          RenderTargetViewConfigDX11 *_rtvConfig = nullptr,
                          DepthStencilViewConfigDX11 *_dsvConfig = nullptr);

        S32 resourceId{};
        S32 srvId{};
        S32 rtvId{};
        S32 dsvId;
//        S32 uavId;


        std::unique_ptr<ShaderResourceViewConfigDX11> srvConfig;
        std::unique_ptr<RenderTargetViewConfigDX11> rtvConfig;
        std::unique_ptr<DepthStencilViewConfigDX11> dsvConfig;
        std::unique_ptr<BufferConfigDX11> bufferConfig;
        std::unique_ptr<Texture1dConfigDX11> texture1dConfig;
        std::unique_ptr<Texture2dConfigDX11> texture2dConfig;
    };
}