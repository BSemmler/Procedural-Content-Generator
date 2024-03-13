//
// Created by Brett on 2024-03-10.
//

#ifndef ENGINE_LIGHTCOMPONENT_H
#define ENGINE_LIGHTCOMPONENT_H

#include "pch.h"

namespace KGV::Engine {
    enum eLightType : U32 {
        kDirectional = 0,
        kPoint = 1,
        kSpot = 2,
    };

    class LightComponent {
    public:
        DirectX::XMFLOAT4A ambient;
        DirectX::XMFLOAT4A diffuse;
        DirectX::XMFLOAT4A specular;
        DirectX::XMFLOAT3 attenuation;
        float range;
        float spot;
        eLightType lightType;
    };
}


#endif //ENGINE_LIGHTCOMPONENT_H
