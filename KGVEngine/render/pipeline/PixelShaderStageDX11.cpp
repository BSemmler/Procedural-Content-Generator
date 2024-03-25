//
// Created by Brett on 3/7/2024.
//

#include "PixelShaderStageDX11.h"
#include "render/RenderDeviceDX11.h"
#include "render/shader/PixelShaderDX11.h"
#include "render/resources/containers/ConstantBufferDX11.h"

const KGV::Render::ShaderStageStateDX11 &KGV::Render::PixelShaderStageDX11::getCurrentState() const {
    return currentState;
}

void KGV::Render::PixelShaderStageDX11::setCurrentState(const KGV::Render::ShaderStageStateDX11 &currentState) {
    PixelShaderStageDX11::currentState = currentState;
}

const KGV::Render::ShaderStageStateDX11 &KGV::Render::PixelShaderStageDX11::getDesiredState() const {
    return desiredState;
}

void KGV::Render::PixelShaderStageDX11::setDesiredState(const KGV::Render::ShaderStageStateDX11 &desiredState) {
    PixelShaderStageDX11::desiredState = desiredState;
}

void KGV::Render::PixelShaderStageDX11::applyDesiredState(ComPtr<ID3D11DeviceContext> context,
                                                          KGV::Render::RenderDeviceDX11 *device) {
    auto shader = dynamic_cast<PixelShaderDX11*>(device->getShaderById(desiredState.getShaderId()));
    context->PSSetShader(shader->getPixelShader().Get(), nullptr, 0);

    std::vector<ID3D11ShaderResourceView*> srvs;
    for (S32 id : desiredState.getSrvIds()) {
        srvs.emplace_back(device->getSrvById(id)->getView().Get());
    }

    context->PSSetShaderResources(0, srvs.size(), srvs.data());

    std::vector<ID3D11SamplerState*> samplers;
    for (auto id : desiredState.getSamplerIds()) {
        samplers.emplace_back(device->getSamplerStateById(id).Get());
    }

    context->PSSetSamplers(0, samplers.size(), samplers.data());

    std::vector<ID3D11Buffer*> constantBuffers;
    for (S32 id : desiredState.getConstantBuffersIds()) {
        auto* buff = dynamic_cast<ConstantBufferDX11 *>(device->getResourceById(id));
        constantBuffers.emplace_back(buff->getBuffer().Get());
    }

    context->PSSetConstantBuffers(desiredState.getCbufferStartSlot(), constantBuffers.size(), constantBuffers.data());
}
