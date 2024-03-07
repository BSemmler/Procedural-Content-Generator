//
// Created by Brett on 2024-03-06.
//

#include "InputAssemblerStateDX11.h"

const std::vector<S32> &KGV::Render::InputAssemblerStateDX11::getVertexBuffers() const {
    return vertexBuffers;
}

void KGV::Render::InputAssemblerStateDX11::setVertexBuffers(const std::vector<S32> &vertexBuffers) {
    InputAssemblerStateDX11::vertexBuffers = vertexBuffers;
}

const std::vector<S32> &KGV::Render::InputAssemblerStateDX11::getStrides() const {
    return strides;
}

void KGV::Render::InputAssemblerStateDX11::setStrides(const std::vector<S32> &strides) {
    InputAssemblerStateDX11::strides = strides;
}

const std::vector<S32> &KGV::Render::InputAssemblerStateDX11::getOffsets() const {
    return offsets;
}

void KGV::Render::InputAssemblerStateDX11::setOffsets(const std::vector<S32> &offsets) {
    InputAssemblerStateDX11::offsets = offsets;
}

S32 KGV::Render::InputAssemblerStateDX11::getInputLayout() const {
    return inputLayout;
}

void KGV::Render::InputAssemblerStateDX11::setInputLayout(S32 inputLayout) {
    InputAssemblerStateDX11::inputLayout = inputLayout;
}

S32 KGV::Render::InputAssemblerStateDX11::getIndexBuffer() const {
    return indexBuffer;
}

void KGV::Render::InputAssemblerStateDX11::setIndexBuffer(S32 indexBuffer) {
    InputAssemblerStateDX11::indexBuffer = indexBuffer;
}

DXGI_FORMAT KGV::Render::InputAssemblerStateDX11::getIndexFormat() const {
    return indexFormat;
}

void KGV::Render::InputAssemblerStateDX11::setIndexFormat(DXGI_FORMAT indexFormat) {
    InputAssemblerStateDX11::indexFormat = indexFormat;
}

D3D11_PRIMITIVE_TOPOLOGY KGV::Render::InputAssemblerStateDX11::getTopology() const {
    return topology;
}

void KGV::Render::InputAssemblerStateDX11::setTopology(D3D11_PRIMITIVE_TOPOLOGY topology) {
    InputAssemblerStateDX11::topology = topology;
}

S32 KGV::Render::InputAssemblerStateDX11::getAvailableSlots() const {
    return static_cast<S32>(maxSlots - vertexBuffers.size());
}
