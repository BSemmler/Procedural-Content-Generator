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

void KGV::Render::ShaderStageStateDX11::setConstantBuffersIds(const std::vector<S32> &constantBuffersIds) {
    ShaderStageStateDX11::constantBuffersIds = constantBuffersIds;
}

const std::vector<S32> &KGV::Render::ShaderStageStateDX11::getSrvIds() const {
    return srvIds;
}

void KGV::Render::ShaderStageStateDX11::setSrvIds(const std::vector<S32> &srvIds) {
    ShaderStageStateDX11::srvIds = srvIds;
}
