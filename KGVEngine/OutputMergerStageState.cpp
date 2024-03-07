//
// Created by Brett on 2024-03-06.
//

#include "OutputMergerStageState.h"

const std::vector<S32> &KGV::Render::OutputMergerStageState::getRtvIds() const {
    return rtvIds;
}

void KGV::Render::OutputMergerStageState::setRtvIds(const std::vector<S32> &rtvIds) {
    OutputMergerStageState::rtvIds = rtvIds;
}
