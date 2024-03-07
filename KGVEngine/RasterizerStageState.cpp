//
// Created by Brett on 2024-03-06.
//

#include "RasterizerStageState.h"

S32 KGV::Render::RasterizerStageState::getRasterizerState() const {
    return rasterizerState;
}

void KGV::Render::RasterizerStageState::setRasterizerState(S32 rasterizerState) {
    RasterizerStageState::rasterizerState = rasterizerState;
}

const std::vector<S32> &KGV::Render::RasterizerStageState::getViewportIds() const {
    return viewportIds;
}

void KGV::Render::RasterizerStageState::setViewportIds(const std::vector<S32> &viewportIds) {
    RasterizerStageState::viewportIds = viewportIds;
}

const std::vector<D3D11_RECT> &KGV::Render::RasterizerStageState::getScissorRectIds() const {
    return scissorRectIds;
}

void KGV::Render::RasterizerStageState::setScissorRectIds(const std::vector<D3D11_RECT> &scissorRectIds) {
    RasterizerStageState::scissorRectIds = scissorRectIds;
}
