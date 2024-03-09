//
// Created by Brett on 2024-03-06.
//

#include "RasterizerStageStateDX11.h"

S32 KGV::Render::RasterizerStageStateDX11::getRasterizerState() const {
    return rasterizerState;
}

void KGV::Render::RasterizerStageStateDX11::setRasterizerState(S32 rasterizerState) {
    RasterizerStageStateDX11::rasterizerState = rasterizerState;
}

const std::vector<S32> &KGV::Render::RasterizerStageStateDX11::getViewportIds() const {
    return viewportIds;
}

void KGV::Render::RasterizerStageStateDX11::setViewportIds(const std::vector<S32> &viewportIds) {
    RasterizerStageStateDX11::viewportIds = viewportIds;
}

const std::vector<D3D11_RECT> &KGV::Render::RasterizerStageStateDX11::getScissorRectIds() const {
    return scissorRectIds;
}

void KGV::Render::RasterizerStageStateDX11::setScissorRectIds(const std::vector<D3D11_RECT> &scissorRectIds) {
    RasterizerStageStateDX11::scissorRectIds = scissorRectIds;
}
