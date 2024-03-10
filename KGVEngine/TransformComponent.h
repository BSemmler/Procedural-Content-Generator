//
// Created by Brett on 2024-03-09.
//

#ifndef ENGINE_TRANSFORMCOMPONENT_H
#define ENGINE_TRANSFORMCOMPONENT_H

#include "pch.h"

namespace KGV::Engine {
    class TransformComponent {
    public:
        TransformComponent();

        DirectX::XMFLOAT3A position{};
        DirectX::XMFLOAT3A rotation{};
        DirectX::XMFLOAT3A scale{};
        DirectX::XMMATRIX matrix();
    };
}



#endif //ENGINE_TRANSFORMCOMPONENT_H
