//
// Created by Brett on 2024-03-09.
//

#ifndef ENGINE_TRANSFORM_H
#define ENGINE_TRANSFORM_H

#include "pch.h"

namespace KGV::Engine {
    class Transform {
    public:
        DirectX::XMFLOAT4A position;
        DirectX::XMFLOAT4A rotation;
        DirectX::XMFLOAT4A scale;
        DirectX::XMMATRIX matrix();
    };
}



#endif //ENGINE_TRANSFORM_H
