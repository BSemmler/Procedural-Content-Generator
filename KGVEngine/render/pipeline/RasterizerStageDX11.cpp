//
// Created by Brett on 3/7/2024.
//

#include "RasterizerStageDX11.h"
#include "render/RenderDeviceDX11.h"

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
    const auto rasterState = device->getRasterizerStateById(desiredState.getRasterizerState());
    context->RSSetState(rasterState.Get());

    std::vector<D3D11_VIEWPORT> viewPorts;
    for (auto id : desiredState.getViewportIds()) {
        viewPorts.emplace_back(*device->getViewPortById(id));
    }
    context->RSSetViewports( viewPorts.size(), viewPorts.data());


    context->RSSetScissorRects(desiredState.getScissorRect().size(), desiredState.getScissorRect().data());
}
