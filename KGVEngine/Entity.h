//
// Created by Brett on 2024-03-09.
//

#ifndef ENGINE_ENTITY_H
#define ENGINE_ENTITY_H

#include "pch.h"
#include "TransformComponent.h"
#include "MeshComponent.h"
#include "Camera.h"
#include "MaterialComponent.h"
#include "LightComponent.h"

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
