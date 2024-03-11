//
// Created by Brett on 2024-03-10.
//

#ifndef ENGINE_MATERIALCOMPONENT_H
#define ENGINE_MATERIALCOMPONENT_H

#include "pch.h"

namespace KGV::Engine {
    class MaterialComponent {
    public:
        S32 materialId;
        DirectX::XMFLOAT4A color;
    };
}



#endif //ENGINE_MATERIALCOMPONENT_H
