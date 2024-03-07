//
// Created by Brett on 2024-03-06.
//

#include "InputAssemblerState.h"

const std::vector<S32> &KGV::Render::InputAssemblerState::getVertexBuffers() const {
    return vertexBuffers;
}

void KGV::Render::InputAssemblerState::setVertexBuffers(const std::vector<S32> &vertexBuffers) {
    InputAssemblerState::vertexBuffers = vertexBuffers;
}

const std::vector<S32> &KGV::Render::InputAssemblerState::getStrides() const {
    return strides;
}

void KGV::Render::InputAssemblerState::setStrides(const std::vector<S32> &strides) {
    InputAssemblerState::strides = strides;
}

const std::vector<S32> &KGV::Render::InputAssemblerState::getOffsets() const {
    return offsets;
}

void KGV::Render::InputAssemblerState::setOffsets(const std::vector<S32> &offsets) {
    InputAssemblerState::offsets = offsets;
}

S32 KGV::Render::InputAssemblerState::getInputLayout() const {
    return inputLayout;
}

void KGV::Render::InputAssemblerState::setInputLayout(S32 inputLayout) {
    InputAssemblerState::inputLayout = inputLayout;
}

S32 KGV::Render::InputAssemblerState::getIndexBuffer() const {
    return indexBuffer;
}

void KGV::Render::InputAssemblerState::setIndexBuffer(S32 indexBuffer) {
    InputAssemblerState::indexBuffer = indexBuffer;
}

DXGI_FORMAT KGV::Render::InputAssemblerState::getIndexFormat() const {
    return indexFormat;
}

void KGV::Render::InputAssemblerState::setIndexFormat(DXGI_FORMAT indexFormat) {
    InputAssemblerState::indexFormat = indexFormat;
}

D3D11_PRIMITIVE_TOPOLOGY KGV::Render::InputAssemblerState::getTopology() const {
    return topology;
}

void KGV::Render::InputAssemblerState::setTopology(D3D11_PRIMITIVE_TOPOLOGY topology) {
    InputAssemblerState::topology = topology;
}

S32 KGV::Render::InputAssemblerState::getAvailableSlots() const {
    return static_cast<S32>(maxSlots - vertexBuffers.size());
}
