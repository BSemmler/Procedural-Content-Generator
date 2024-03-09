//
// Created by Brett on 2024-03-06.
//

#include "OutputMergerStageStateDX11.h"

const std::vector<S32> &KGV::Render::OutputMergerStageStateDX11::getRtvIds() const {
    return rtvIds;
}

void KGV::Render::OutputMergerStageStateDX11::setRtvIds(const std::vector<S32> &rtvIds) {
    OutputMergerStageStateDX11::rtvIds = rtvIds;
}
