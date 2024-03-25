//
// Created by Brett on 3/7/2024.
//

#ifndef ENGINE_PIXELSHADERSTAGEDX11_H
#define ENGINE_PIXELSHADERSTAGEDX11_H

#include "pch.h"
#include "IPipelineStageDX11.h"
#include "ShaderStageStateDX11.h"

namespace KGV::Render {
    class IPipelineStageDX11;
    class PixelShaderStageDX11 : public IPipelineStageDX11 {
    public:
        const ShaderStageStateDX11 &getCurrentState() const;

        void setCurrentState(const ShaderStageStateDX11 &currentState);

        const ShaderStageStateDX11 &getDesiredState() const;

        void setDesiredState(const ShaderStageStateDX11 &desiredState);

        void applyDesiredState(ComPtr<ID3D11DeviceContext> context, RenderDeviceDX11 *device) override;

    protected:
        ShaderStageStateDX11 currentState;
        ShaderStageStateDX11 desiredState;
    };
}



#endif //ENGINE_PIXELSHADERSTAGEDX11_H
