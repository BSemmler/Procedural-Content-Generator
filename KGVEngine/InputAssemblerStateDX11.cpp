//
// Created by Brett on 2024-03-06.
//

#include "InputAssemblerStateDX11.h"
#include "ResourceViewDX11.h"

const std::vector<S32> &KGV::Render::InputAssemblerStateDX11::getVertexBuffers() const {
    return vertexBuffers;
}

void KGV::Render::InputAssemblerStateDX11::setVertexBuffers(const std::vector<S32> &vertexBuffers) {
    this->vertexBuffers = vertexBuffers;
}

const std::vector<S32> &KGV::Render::InputAssemblerStateDX11::getStrides() const {
    return strides;
}

void KGV::Render::InputAssemblerStateDX11::setStrides(const std::vector<S32> &strides) {
    this->strides = strides;
}

const std::vector<S32> &KGV::Render::InputAssemblerStateDX11::getOffsets() const {
    return offsets;
}

void KGV::Render::InputAssemblerStateDX11::setOffsets(const std::vector<S32> &offsets) {
    this->offsets = offsets;
}

S32 KGV::Render::InputAssemblerStateDX11::getInputLayout() const {
    return inputLayout;
}

void KGV::Render::InputAssemblerStateDX11::setInputLayout(S32 inputLayout) {
    this->inputLayout = inputLayout;
}

S32 KGV::Render::InputAssemblerStateDX11::getIndexBuffer() const {
    return indexBuffer;
}

void KGV::Render::InputAssemblerStateDX11::setIndexBuffer(S32 indexBuffer) {
    this->indexBuffer = indexBuffer;
}

DXGI_FORMAT KGV::Render::InputAssemblerStateDX11::getIndexFormat() const {
    return indexFormat;
}

void KGV::Render::InputAssemblerStateDX11::setIndexFormat(DXGI_FORMAT indexFormat) {
    this->indexFormat = indexFormat;
}

D3D11_PRIMITIVE_TOPOLOGY KGV::Render::InputAssemblerStateDX11::getTopology() const {
    return topology;
}

void KGV::Render::InputAssemblerStateDX11::setTopology(D3D11_PRIMITIVE_TOPOLOGY topology) {
    this->topology = topology;
}

S32 KGV::Render::InputAssemblerStateDX11::getAvailableSlots() const {
    return static_cast<S32>(maxSlots - vertexBuffers.size());
}

void KGV::Render::InputAssemblerStateDX11::setVertexBuffers(const std::vector<S32> &vertexBuffers, const std::vector<S32> &strides,
                                                            const std::vector<S32> &offsets) {
    this->vertexBuffers = vertexBuffers;
    this->strides = strides;
    this->offsets = offsets;
}

void KGV::Render::InputAssemblerStateDX11::setVertexBuffers(const std::vector<std::shared_ptr<ResourceViewDX11>> &vertexBufferViews) {
    vertexBuffers.clear();
    strides.clear();
    offsets.clear();
    for (auto r : vertexBufferViews) {
        vertexBuffers.emplace_back(r->getResourceId());
        strides.emplace_back(0);
        offsets.emplace_back(0);
    }
}

void KGV::Render::InputAssemblerStateDX11::setVertexBuffers(const std::vector<std::shared_ptr<ResourceViewDX11>> &vertexBufferViews,
                                                            const std::vector<S32> &strides, const std::vector<S32> &offsets) {
    vertexBuffers.clear();
    for (const auto& r : vertexBufferViews) {
        vertexBuffers.emplace_back(r->getResourceId());
        this->strides = strides;
        this->offsets = offsets;
    }
}
