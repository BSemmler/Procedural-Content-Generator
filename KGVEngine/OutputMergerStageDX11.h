//
// Created by Brett on 2024-03-08.
//

#ifndef ENGINE_OUTPUTMERGERSTAGEDX11_H
#define ENGINE_OUTPUTMERGERSTAGEDX11_H

#include "pch.h"
#include "IPipelineStageDX11.h"
#include "OutputMergerStageStateDX11.h"

namespace KGV::Render {
    class OutputMergerStageDX11 : public IPipelineStageDX11 {
    public:
        const OutputMergerStageStateDX11 &getCurrentState() const;

        void setCurrentState(const OutputMergerStageStateDX11 &currentState);

        const OutputMergerStageStateDX11 &getDesiredState() const;

        void setDesiredState(const OutputMergerStageStateDX11 &desiredState);

    private:
        void applyDesiredState(ComPtr<ID3D11DeviceContext> context, RenderDeviceDX11 *device) override;

    protected:
        OutputMergerStageStateDX11 currentState;
        OutputMergerStageStateDX11 desiredState;
    };
}



#endif //ENGINE_OUTPUTMERGERSTAGEDX11_H
