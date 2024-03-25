//
// Created by Brett on 2024-03-09.
//

#ifndef ENGINE_ENTITY_H
#define ENGINE_ENTITY_H

#include "pch.h"
#include "engine/entitycomponents/TransformComponent.h"
#include "engine/entitycomponents/MeshComponent.h"
#include "Camera.h"
#include "engine/entitycomponents/MaterialComponent.h"
#include "engine/entitycomponents/LightComponent.h"

namespace KGV::Engine {
    class Entity {
    public:
        TransformComponent transform;
        std::unique_ptr<MeshComponent> mesh;
        std::unique_ptr<MaterialComponent> material;
        std::unique_ptr<Camera> camera;
        std::unique_ptr<LightComponent> light;
    };
}



#endif //ENGINE_ENTITY_H
