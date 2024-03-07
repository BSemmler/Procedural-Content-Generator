//
// Created by Brett on 2024-03-06.
//

#include "ShaderStageState.h"

S32 KGV::Render::ShaderStageState::getShaderId() const {
    return shaderId;
}

void KGV::Render::ShaderStageState::setShaderId(S32 shaderId) {
    ShaderStageState::shaderId = shaderId;
}

const std::vector<S32> &KGV::Render::ShaderStageState::getConstantBuffersIds() const {
    return constantBuffersIds;
}

void KGV::Render::ShaderStageState::setConstantBuffersIds(const std::vector<S32> &constantBuffersIds) {
    ShaderStageState::constantBuffersIds = constantBuffersIds;
}

const std::vector<S32> &KGV::Render::ShaderStageState::getSrvIds() const {
    return srvIds;
}

void KGV::Render::ShaderStageState::setSrvIds(const std::vector<S32> &srvIds) {
    ShaderStageState::srvIds = srvIds;
}
