#pragma once

#include "pch.h"
#include "RenderDeviceDX11.h"
#include "ShaderResourceViewConfigDX11.h"
#include "RenderTargetViewConfigDX11.h"

namespace KGV::Render {
    class ResourceViewDX11 {
        ResourceViewDX11(S32 _resourceId, BufferConfigDX11 &_config, RenderDeviceDX11 &_device,
                         ShaderResourceViewConfigDX11 *_srvConfig = nullptr, RenderTargetViewConfigDX11 *_rtvConfig = nullptr);

        ResourceViewDX11(S32 _resourceId, Texture2dConfigDX11 &_config, RenderDeviceDX11 &_device,
                         ShaderResourceViewConfigDX11 *_srvConfig = nullptr, RenderTargetViewConfigDX11 *_rtvConfig = nullptr);

    protected:
        void initResource(U32 _bindFlags, S32 _resourceId, RenderDeviceDX11 &_device,
                          ShaderResourceViewConfigDX11 *_srvConfig = nullptr, RenderTargetViewConfigDX11 *_rtvConfig = nullptr);

        S32 resourceId{};
        S32 srvId{};
        S32 rtvId{};
//        S32 dsvId;
//        S32 uavId;


        std::unique_ptr<ShaderResourceViewConfigDX11> srvConfig;
        std::unique_ptr<RenderTargetViewConfigDX11> rtvConfig;
        std::unique_ptr<BufferConfigDX11> bufferConfig;
        std::unique_ptr<Texture2dConfigDX11> texture2dConfig;
    };
}