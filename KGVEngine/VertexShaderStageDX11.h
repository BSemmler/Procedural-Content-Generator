//
// Created by Brett on 2024-03-06.
//

#ifndef ENGINE_VERTEXSHADERSTAGEDX11_H
#define ENGINE_VERTEXSHADERSTAGEDX11_H

#include "pch.h"
#include "RenderDeviceDX11.h"
#include "ShaderStageStateDX11.h"
#include "IPipelineStageDX11.h"

namespace KGV::Render {
    class VertexShaderStageDX11 : public IPipelineStageDX11 {
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



#endif //ENGINE_VERTEXSHADERSTAGEDX11_H
