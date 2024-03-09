//
// Created by Brett on 2024-03-09.
//

#include "Camera.h"

void KGV::Engine::Camera::setOrthographicProject(F32 left, F32 right, F32 top, F32 bottom, F32 nearPlane, F32 farPlane) {
    DirectX::XMFLOAT4X4A matrix;
    matrix._11 = 2.0f / (right - left);
    matrix._22 = 2.0f / (bottom - top);
    matrix._33 = 1.0f / (farPlane - nearPlane);
    matrix._41 = -(right + left) / (right - left);
    matrix._42 = -(bottom + top) / (bottom - top);
    matrix._43 = -nearPlane / (farPlane - nearPlane);
    projectionMatrix = DirectX::XMLoadFloat4x4A(&matrix);
    isOrthographic = true;
}

void KGV::Engine::Camera::setPerspectiveProject(F32 fovY, F32 aspect, F32 nearPlane, F32 farPlane) {
    auto tanHalfFovY = tan(fovY / 2.0f);
    DirectX::XMFLOAT4X4A matrix;
    matrix._11 = 1.0f / (aspect * tanHalfFovY);
    matrix._22 = 1.0f / (tanHalfFovY);
    matrix._33 = farPlane / (farPlane - nearPlane);
    matrix._34 = 1.0f;
    matrix._43 = -(farPlane * nearPlane) / (farPlane - nearPlane);
    projectionMatrix = DirectX::XMLoadFloat4x4A(&matrix);
    isOrthographic = false;
}

const DirectX::XMMATRIX &KGV::Engine::Camera::getProjectionMatrix() {
    return projectionMatrix;
}
