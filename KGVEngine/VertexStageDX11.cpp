//
// Created by Brett on 2024-03-06.
//

#include "VertexStageDX11.h"
#include "VertexShaderDX11.h"
#include "ConstantBufferDX11.h"

const KGV::Render::ShaderStageStateDX11 &KGV::Render::VertexStageDX11::getCurrentState() const {
    return currentState;
}

void KGV::Render::VertexStageDX11::setCurrentState(const KGV::Render::ShaderStageStateDX11 &currentState) {
    VertexStageDX11::currentState = currentState;
}

const KGV::Render::ShaderStageStateDX11 &KGV::Render::VertexStageDX11::getDesiredState() const {
    return desiredState;
}

void KGV::Render::VertexStageDX11::setDesiredState(const KGV::Render::ShaderStageStateDX11 &desiredState) {
    VertexStageDX11::desiredState = desiredState;
}

void KGV::Render::VertexStageDX11::applyDesiredState(ComPtr<ID3D11DeviceContext> context, KGV::Render::RenderDeviceDX11 *device) {
    auto shader = dynamic_cast<VertexShaderDX11*>(device->getShaderById(desiredState.getShaderId()));
    context->VSSetShader(shader->getVertexShader().Get(), nullptr, 0);

    std::vector<ID3D11ShaderResourceView*> srvs;
    for (S32 id : desiredState.getSrvIds()) {
        srvs.emplace_back(device->getSrvById(id)->getView().Get());
    }

    context->VSSetShaderResources(0, srvs.size(), srvs.data());

    std::vector<ID3D11Buffer*> constantBuffers;
    for (S32 id : desiredState.getConstantBuffersIds()) {
        auto* buff = dynamic_cast<ConstantBufferDX11 *>(device->getResourceById(id));
        constantBuffers.emplace_back(buff->getBuffer().Get());
    }

    context->VSSetConstantBuffers(0, constantBuffers.size(), constantBuffers.data());
}
