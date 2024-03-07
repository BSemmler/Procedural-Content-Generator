//
// Created by Brett on 2024-03-06.
//

#ifndef ENGINE_RASTERIZERSTAGESTATE_H
#define ENGINE_RASTERIZERSTAGESTATE_H

#include "pch.h"

namespace KGV::Render {
    class RasterizerStageState {
    public:
        S32 getRasterizerState() const;

        void setRasterizerState(S32 rasterizerState);

        const std::vector<S32> &getViewportIds() const;

        void setViewportIds(const std::vector<S32> &viewportIds);

        const std::vector<D3D11_RECT> &getScissorRectIds() const;

        void setScissorRectIds(const std::vector<D3D11_RECT> &scissorRectIds);

    protected:
        S32 rasterizerState;
        std::vector<S32> viewportIds;
        std::vector<D3D11_RECT> scissorRectIds;
    };
}



#endif //ENGINE_RASTERIZERSTAGESTATE_H
