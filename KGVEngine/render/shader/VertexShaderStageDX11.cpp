//
// Created by Brett on 2024-03-06.
//

#include "VertexShaderStageDX11.h"
#include "render/RenderDeviceDX11.h"
#include "render/resources/containers/VertexShaderDX11.h"
#include "render/resources/containers/ConstantBufferDX11.h"

const KGV::Render::ShaderStageStateDX11 &KGV::Render::VertexShaderStageDX11::getCurrentState() const {
    return currentState;
}

void KGV::Render::VertexShaderStageDX11::setCurrentState(const KGV::Render::ShaderStageStateDX11 &currentState) {
    VertexShaderStageDX11::currentState = currentState;
}

const KGV::Render::ShaderStageStateDX11 &KGV::Render::VertexShaderStageDX11::getDesiredState() const {
    return desiredState;
}

void KGV::Render::VertexShaderStageDX11::setDesiredState(const KGV::Render::ShaderStageStateDX11 &desiredState) {
    VertexShaderStageDX11::desiredState = desiredState;
}

void KGV::Render::VertexShaderStageDX11::applyDesiredState(ComPtr<ID3D11DeviceContext> context, KGV::Render::RenderDeviceDX11 *device) {
    auto shader = dynamic_cast<VertexShaderDX11*>(device->getShaderById(desiredState.getShaderId()));
    context->VSSetShader(shader->getVertexShader().Get(), nullptr, 0);

    std::vector<ID3D11ShaderResourceView*> srvs;
    for (S32 id : desiredState.getSrvIds()) {
        srvs.emplace_back(device->getSrvById(id)->getView().Get());
    }

    // TODO: Update this so we can set start slots as well.
    context->VSSetShaderResources(0, srvs.size(), srvs.data());

    std::vector<ID3D11SamplerState*> samplers;
    for (S32 id : desiredState.getSamplerIds()) {
        samplers.emplace_back(device->getSamplerStateById(id).Get());
    }

    context->VSSetSamplers(0, samplers.size(), samplers.data());

    std::vector<ID3D11Buffer*> constantBuffers;
    for (S32 id : desiredState.getConstantBuffersIds()) {
        auto* buff = dynamic_cast<ConstantBufferDX11 *>(device->getResourceById(id));
        constantBuffers.emplace_back(buff->getBuffer().Get());
    }

    context->VSSetConstantBuffers(0, constantBuffers.size(), constantBuffers.data());



    currentState = desiredState;
}
