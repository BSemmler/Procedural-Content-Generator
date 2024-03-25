//
// Created by Brett on 2024-03-10.
//

#ifndef ENGINE_RENDERMATERIAL_H
#define ENGINE_RENDERMATERIAL_H

#include "pch.h"

namespace KGV::Render {
    class RenderMaterial {
    public:
        S32 inputLayoutId;
        S32 vertexShaderId;
        S32 pixelShaderId;
    };
}



#endif //ENGINE_RENDERMATERIAL_H
