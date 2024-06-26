//
// Created by Brett on 2024-03-06.
//

#ifndef ENGINE_OUTPUTMERGERSTAGESTATEDX11_H
#define ENGINE_OUTPUTMERGERSTAGESTATEDX11_H

#include "pch.h"

namespace KGV::Render {
    class OutputMergerStageStateDX11 {
    public:
        const std::vector<S32> &getRtvIds() const;

        void setRtvIds(const std::vector<S32> &rtvIds);

        S32 getDsvId() const;
        void setDsvId(S32 id);

        S32 getDsvStateId() const;

        void setDsvStateId(S32 dsvStateId);

    protected:
        std::vector<S32> rtvIds;
        S32 depthStencilViewId;
        S32 dsvStateId;
    };
}



#endif //ENGINE_OUTPUTMERGERSTAGESTATEDX11_H
