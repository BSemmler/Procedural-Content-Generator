//
// Created by Brett on 2024-03-06.
//

#ifndef ENGINE_RASTERIZERSTAGESTATEDX11_H
#define ENGINE_RASTERIZERSTAGESTATEDX11_H

#include "pch.h"

namespace KGV::Render {
    class RasterizerStageStateDX11 {
    public:
        S32 getRasterizerState() const;

        void setRasterizerState(S32 rasterizerState);

        const std::vector<S32> &getViewportIds() const;

        void setViewportIds(const std::vector<S32> &viewportIds);

        const std::vector<D3D11_RECT> &getScissorRect() const;

        void setScissorRect(const std::vector<D3D11_RECT> &scissorRects);

    protected:
        S32 rasterizerState;
        std::vector<S32> viewportIds;
        std::vector<D3D11_RECT> scissorRects;
    };
}



#endif //ENGINE_RASTERIZERSTAGESTATEDX11_H
