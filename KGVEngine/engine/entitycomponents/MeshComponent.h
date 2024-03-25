//
// Created by Brett on 2024-03-09.
//

#ifndef ENGINE_MESHCOMPONENT_H
#define ENGINE_MESHCOMPONENT_H

#include "pch.h"
#include "render/resources/ResourceViewDX11.h"

namespace KGV::Engine {
    class MeshComponent {
    public:
        S32 meshId;
        bool render;
    };
}


#endif //ENGINE_MESHCOMPONENT_H
