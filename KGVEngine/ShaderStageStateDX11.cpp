//
// Created by Brett on 2024-03-06.
//

#include "ShaderStageStateDX11.h"

S32 KGV::Render::ShaderStageStateDX11::getShaderId() const {
    return shaderId;
}

void KGV::Render::ShaderStageStateDX11::setShaderId(S32 shaderId) {
    ShaderStageStateDX11::shaderId = shaderId;
}

const std::vector<S32> &KGV::Render::ShaderStageStateDX11::getConstantBuffersIds() const {
    return constantBuffersIds;
}

void KGV::Render::ShaderStageStateDX11::setConstantBuffersIds(const std::vector<S32> &constantBuffersIds,  S32 startSlot) {
    ShaderStageStateDX11::constantBuffersIds = constantBuffersIds;
    this->cbufferStartSlot = startSlot;
}

const std::vector<S32> &KGV::Render::ShaderStageStateDX11::getSrvIds() const {
    return srvIds;
}

void KGV::Render::ShaderStageStateDX11::setSrvIds(const std::vector<S32> &srvIds) {
    ShaderStageStateDX11::srvIds = srvIds;
}

S32 KGV::Render::ShaderStageStateDX11::getCbufferStartSlot() const {
    return cbufferStartSlot;
}

void KGV::Render::ShaderStageStateDX11::setCbufferStartSlot(S32 cbufferStartSlot) {
    ShaderStageStateDX11::cbufferStartSlot = cbufferStartSlot;
}

const std::vector<S32> &KGV::Render::ShaderStageStateDX11::getSamplerIds() const {
    return samplerIds;
}

void KGV::Render::ShaderStageStateDX11::setSamplerIds(const std::vector<S32> &samplerIds) {
    ShaderStageStateDX11::samplerIds = samplerIds;
}
