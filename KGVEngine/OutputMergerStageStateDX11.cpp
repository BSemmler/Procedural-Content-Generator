//
// Created by Brett on 2024-03-06.
//

#include "OutputMergerStageStateDX11.h"

const std::vector<S32> &KGV::Render::OutputMergerStageStateDX11::getRtvIds() const {
    return rtvIds;
}

void KGV::Render::OutputMergerStageStateDX11::setRtvIds(const std::vector<S32> &rtvIds) {
    this->rtvIds = rtvIds;
}

void KGV::Render::OutputMergerStageStateDX11::setDsvId(S32 id) {
    depthStencilViewId = id;
}

S32 KGV::Render::OutputMergerStageStateDX11::getDsvId() const {
    return depthStencilViewId;
}

S32 KGV::Render::OutputMergerStageStateDX11::getDsvStateId() const {
    return dsvStateId;
}

void KGV::Render::OutputMergerStageStateDX11::setDsvStateId(S32 dsvStateId) {
    OutputMergerStageStateDX11::dsvStateId = dsvStateId;
}
