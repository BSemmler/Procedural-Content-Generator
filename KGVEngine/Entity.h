//
// Created by Brett on 2024-03-09.
//

#ifndef ENGINE_ENTITY_H
#define ENGINE_ENTITY_H

#include "pch.h"
#include "Transform.h"
#include "Mesh.h"

namespace KGV::Engine {
    class Entity {
    public:
        Transform transform;
        Mesh* mesh;
    };
}



#endif //ENGINE_ENTITY_H
