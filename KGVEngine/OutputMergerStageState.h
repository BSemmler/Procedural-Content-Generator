//
// Created by Brett on 2024-03-06.
//

#ifndef ENGINE_OUTPUTMERGERSTAGESTATE_H
#define ENGINE_OUTPUTMERGERSTAGESTATE_H

#include "pch.h"

namespace KGV::Render {
    class OutputMergerStageState {
    public:
        const std::vector<S32> &getRtvIds() const;

        void setRtvIds(const std::vector<S32> &rtvIds);

    protected:
        std::vector<S32> rtvIds;
//        S32 depthStencilViewId;
    };
}



#endif //ENGINE_OUTPUTMERGERSTAGESTATE_H
