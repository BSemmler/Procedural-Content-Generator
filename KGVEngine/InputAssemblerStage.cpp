//
// Created by Brett on 2024-03-06.
//

#include "InputAssemblerStage.h"

const KGV::Render::InputAssemblerState &KGV::Render::InputAssemblerStage::getCurrentState() const {
    return currentState;
}

void KGV::Render::InputAssemblerStage::setCurrentState(const KGV::Render::InputAssemblerState &currentState) {
    InputAssemblerStage::currentState = currentState;
}

const KGV::Render::InputAssemblerState &KGV::Render::InputAssemblerStage::getDesiredState() const {
    return desiredState;
}

void KGV::Render::InputAssemblerStage::setDesiredState(const KGV::Render::InputAssemblerState &desiredState) {
    InputAssemblerStage::desiredState = desiredState;
}

void KGV::Render::InputAssemblerStage::applyDesiredState(const ComPtr<ID3D11DeviceContext>& context, RenderDeviceDX11* device) {
    auto inputLayout = device->getInputLayoutById(desiredState.getInputLayout());
    context->IASetInputLayout(inputLayout.Get());

    std::vector<ID3D11Buffer*> vertexBuffers;
    for (S32 id : desiredState.getVertexBuffers()) {
        auto* buff = dynamic_cast<VertexBufferDX11 *>(device->getResourceById(id));
        vertexBuffers.emplace_back(dynamic_cast<ID3D11Buffer *>(buff->getResource()));
    }
    context->IASetVertexBuffers(0, vertexBuffers.size(), vertexBuffers.data(),
                                reinterpret_cast<const UINT *>(desiredState.getStrides().data()),
                                reinterpret_cast<const UINT *>(desiredState.getOffsets().data()));

    auto indexBuffer = dynamic_cast<IndexBufferDX11*>(device->getResourceById(desiredState.getIndexBuffer()));
    context->IASetIndexBuffer(dynamic_cast<ID3D11Buffer *>(indexBuffer->getResource()), desiredState.getIndexFormat(), 0);

    context->IASetPrimitiveTopology(desiredState.getTopology());

    currentState = desiredState;
}
