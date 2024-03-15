//
// Created by Brett on 2024-03-08.
//

#include "OutputMergerStageDX11.h"
#include "RenderDeviceDX11.h"

const KGV::Render::OutputMergerStageStateDX11 &KGV::Render::OutputMergerStageDX11::getCurrentState() const {
    return currentState;
}

void KGV::Render::OutputMergerStageDX11::setCurrentState(const KGV::Render::OutputMergerStageStateDX11 &currentState) {
    OutputMergerStageDX11::currentState = currentState;
}

const KGV::Render::OutputMergerStageStateDX11 &KGV::Render::OutputMergerStageDX11::getDesiredState() const {
    return desiredState;
}

void KGV::Render::OutputMergerStageDX11::setDesiredState(const KGV::Render::OutputMergerStageStateDX11 &desiredState) {
    OutputMergerStageDX11::desiredState = desiredState;
}

void KGV::Render::OutputMergerStageDX11::applyDesiredState(ComPtr<ID3D11DeviceContext> context, KGV::Render::RenderDeviceDX11 *device) {
    std::vector<ID3D11RenderTargetView*> renderTargets;
    renderTargets.reserve(desiredState.getRtvIds().size());
    for (auto id : desiredState.getRtvIds()) {
        renderTargets.emplace_back(device->getRtvById(id)->getView().Get());
    }

    auto dsv = device->getDsvById(desiredState.getDsvId());

    // TODO: Configure this to also retrieve depth stencil and blend states.
    context->OMSetRenderTargets(renderTargets.size(), renderTargets.data(), dsv ? dsv->getView().Get() : nullptr);

//    contex->OMSet

    currentState = desiredState;
}
