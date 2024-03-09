//
// Created by Brett on 2024-03-08.
//

#ifndef ENGINE_OUTPUTMERGERSTAGEDX11_H
#define ENGINE_OUTPUTMERGERSTAGEDX11_H

#include "pch.h"
#include "IPipelineStageDX11.h"
#include "OutputMergerStageState.h"

namespace KGV::Render {
    class OutputMergerStageDX11 : public IPipelineStageDX11 {
    public:
        const OutputMergerStageState &getCurrentState() const;

        void setCurrentState(const OutputMergerStageState &currentState);

        const OutputMergerStageState &getDesiredState() const;

        void setDesiredState(const OutputMergerStageState &desiredState);

    private:
        void applyDesiredState(ComPtr<ID3D11DeviceContext> context, RenderDeviceDX11 *device) override;

    protected:
        OutputMergerStageState currentState;
        OutputMergerStageState desiredState;
    };
}



#endif //ENGINE_OUTPUTMERGERSTAGEDX11_H
