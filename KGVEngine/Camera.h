//
// Created by Brett on 2024-03-09.
//

#ifndef ENGINE_CAMERA_H
#define ENGINE_CAMERA_H

#include "pch.h"

namespace KGV::Engine {
    class Camera {
    public:
        void setOrthographicProject(F32 left, F32 right, F32 top, F32 bottom, F32 nearPlane, F32 farPlane);

        void setPerspectiveProject(F32 fovY, F32 aspect, F32 nearPlane, F32 farPlane);

        const DirectX::XMMATRIX& getProjectionMatrix();

    protected:
        DirectX::XMMATRIX projectionMatrix{DirectX::XMMatrixIdentity()};
//        float farPlane;
//        float nearPlane;
//        float fovY;
        bool isOrthographic;
    };
}

#endif //ENGINE_CAMERA_H
