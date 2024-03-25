//
// Created by Brett on 2024-03-06.
//

#include "InputAssemblerStageDX11.h"
#include "render/RenderDeviceDX11.h"
#include "render/resources/containers/VertexBufferDX11.h"
#include "render/resources/containers/IndexBufferDX11.h"


const KGV::Render::InputAssemblerStateDX11 &KGV::Render::InputAssemblerStageDX11::getCurrentState() const {
    return currentState;
}

void KGV::Render::InputAssemblerStageDX11::setCurrentState(const KGV::Render::InputAssemblerStateDX11 &currentState) {
    InputAssemblerStageDX11::currentState = currentState;
}

const KGV::Render::InputAssemblerStateDX11 &KGV::Render::InputAssemblerStageDX11::getDesiredState() const {
    return desiredState;
}

void KGV::Render::InputAssemblerStageDX11::setDesiredState(const KGV::Render::InputAssemblerStateDX11 &desiredState) {
    InputAssemblerStageDX11::desiredState = desiredState;
}

void KGV::Render::InputAssemblerStageDX11::applyDesiredState(ComPtr<ID3D11DeviceContext> context, RenderDeviceDX11* device) {
    context->IASetInputLayout(device->getInputLayoutById(desiredState.getInputLayout()).Get());

    std::vector<ID3D11Buffer*> vertexBuffers;
    for (S32 id : desiredState.getVertexBuffers()) {
        auto* buff = dynamic_cast<VertexBufferDX11*>(device->getResourceById(id));
        vertexBuffers.emplace_back(buff->getBuffer().Get());
    }
    context->IASetVertexBuffers(0, vertexBuffers.size(), vertexBuffers.data(),
                                reinterpret_cast<const UINT *>(desiredState.getStrides().data()),
                                reinterpret_cast<const UINT *>(desiredState.getOffsets().data()));

    auto indexBuffer = dynamic_cast<IndexBufferDX11*>(device->getResourceById(desiredState.getIndexBuffer()));
    context->IASetIndexBuffer(indexBuffer->getBuffer().Get(), desiredState.getIndexFormat(), 0);

    context->IASetPrimitiveTopology(desiredState.getTopology());

    currentState = desiredState;
}
