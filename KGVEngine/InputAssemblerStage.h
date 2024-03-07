//
// Created by Brett on 2024-03-06.
//

#ifndef ENGINE_INPUTASSEMBLERSTAGE_H
#define ENGINE_INPUTASSEMBLERSTAGE_H

#include "pch.h"
#include "RenderDeviceDX11.h"
#include "InputAssemblerState.h"

namespace KGV::Render {
    class InputAssemblerStage {
    public:
        const InputAssemblerState &getCurrentState() const;

        void setCurrentState(const InputAssemblerState &currentState);

        const InputAssemblerState &getDesiredState() const;

        void setDesiredState(const InputAssemblerState &desiredState);

        void applyDesiredState(const ComPtr<ID3D11DeviceContext>& context, RenderDeviceDX11* device);

    protected:
        InputAssemblerState currentState;
        InputAssemblerState desiredState;
    };
}



#endif //ENGINE_INPUTASSEMBLERSTAGE_H
