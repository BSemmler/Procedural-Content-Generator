//
// Created by Brett on 3/7/2024.
//

#include "RasterizerStageDX11.h"

const KGV::Render::RasterizerStageStateDX11 &KGV::Render::RasterizerStageDX11::getCurrentState() const {
    return currentState;
}

void KGV::Render::RasterizerStageDX11::setCurrentState(const KGV::Render::RasterizerStageStateDX11 &currentState) {
    RasterizerStageDX11::currentState = currentState;
}

const KGV::Render::RasterizerStageStateDX11 &KGV::Render::RasterizerStageDX11::getDesiredState() const {
    return desiredState;
}

void KGV::Render::RasterizerStageDX11::setDesiredState(const KGV::Render::RasterizerStageStateDX11 &desiredState) {
    RasterizerStageDX11::desiredState = desiredState;
}

void KGV::Render::RasterizerStageDX11::applyDesiredState(ComPtr<ID3D11DeviceContext> context,
                                                         KGV::Render::RenderDeviceDX11 *device) {
    device->getRasterizerStateById(desiredState.getRasterizerState());
//    context->RSSetState()
}
