//
// Created by Brett on 2024-03-06.
//

#ifndef ENGINE_INPUTASSEMBLERSTAGEDX11_H
#define ENGINE_INPUTASSEMBLERSTAGEDX11_H

#include "pch.h"
#include "IPipelineStageDX11.h"
#include "InputAssemblerStateDX11.h"


namespace KGV::Render {
    class IPipelineStageDX11;
//    class InputAssemblerStateDX11;

    class InputAssemblerStageDX11 : public IPipelineStageDX11 {
    public:
        const InputAssemblerStateDX11 &getCurrentState() const;

        void setCurrentState(const InputAssemblerStateDX11 &currentState);

        const InputAssemblerStateDX11 &getDesiredState() const;

        void setDesiredState(const InputAssemblerStateDX11 &desiredState);

        void applyDesiredState(ComPtr<ID3D11DeviceContext> context, RenderDeviceDX11* device) override;

    protected:
        InputAssemblerStateDX11 currentState;
        InputAssemblerStateDX11 desiredState;
    };
}



#endif //ENGINE_INPUTASSEMBLERSTAGEDX11_H
