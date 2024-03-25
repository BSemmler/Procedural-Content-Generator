//
// Created by Brett on 3/7/2024.
//

#ifndef ENGINE_RASTERIZERSTAGEDX11_H
#define ENGINE_RASTERIZERSTAGEDX11_H

#include "pch.h"
#include "IPipelineStageDX11.h"
#include "RasterizerStageStateDX11.h"

namespace KGV::Render {
    class IPipelineStageDX11;
    class RasterizerStageDX11 : public IPipelineStageDX11 {
    public:
        const RasterizerStageStateDX11 &getCurrentState() const;

        void setCurrentState(const RasterizerStageStateDX11 &currentState);

        const RasterizerStageStateDX11 &getDesiredState() const;

        void setDesiredState(const RasterizerStageStateDX11 &desiredState);

        void applyDesiredState(ComPtr<ID3D11DeviceContext> context, RenderDeviceDX11 *device) override;

    protected:
        RasterizerStageStateDX11 currentState;
        RasterizerStageStateDX11 desiredState;
    };
}



#endif //ENGINE_RASTERIZERSTAGEDX11_H
