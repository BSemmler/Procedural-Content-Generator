//
// Created by Brett on 2024-03-09.
//

#ifndef ENGINE_CAMERA_H
#define ENGINE_CAMERA_H

#include "pch.h"
#include "TransformComponent.h"

namespace KGV::Engine {
    class Camera {
    public:
        void setOrthographicProject(F32 left, F32 right, F32 top, F32 bottom, F32 nearPlane, F32 farPlane);

        void setPerspectiveProject(F32 fovY, F32 aspect, F32 nearPlane, F32 farPlane);

        const DirectX::XMMATRIX& getProjectionMatrix();

        const DirectX::XMMATRIX &getViewMatrix() const;

        void setViewMatrix(const DirectX::XMMATRIX &viewMatrix);

        const DirectX::XMVECTOR &getLookAt() const;

        void setLookAt(const DirectX::XMVECTOR &lookAt);

        const DirectX::XMVECTOR &getUp() const;

        void setUp(const DirectX::XMVECTOR &up);

        const DirectX::XMVECTOR &getEye() const;

        void setEye(const DirectX::XMVECTOR &eye);

        bool isActive() const;

        void setIsActive(bool isActive);

        bool isWireframe() const;

        void setIsWireframe(bool isWireframe);

        S32 getRtvId() const;

        void setRtvId(const S32 id);

        bool isOrthographic() const;

        S32 getViewPortId() const;

        void setViewPortId(S32 viewPortId);

        S32 getDsvId() const;

        void setDsvId(S32 dsvId);

        S32 getDsvStateId() const;

        void setDsvStateId(S32 dsvStateId);

    protected:
        DirectX::XMMATRIX projectionMatrix{DirectX::XMMatrixIdentity()};
        DirectX::XMMATRIX viewMatrix{DirectX::XMMatrixIdentity()};
        DirectX::XMVECTOR lookAt = { 0.0f, 0.0f, 1.0f };
        DirectX::XMVECTOR up { 0.0f, 1.0f, 0.0f };
        DirectX::XMVECTOR eye { 0.0f, 0.0f, 0.0f };
//        float farPlane;
//        float nearPlane;
//        float fovY;
        S32 rtvId;
        S32 viewPortId;
        S32 dsvId;
        S32 dsvStateId;
        bool m_isActive = true;
        bool m_isOrthographic;
        bool m_isWireframe;
    };
}

#endif //ENGINE_CAMERA_H
