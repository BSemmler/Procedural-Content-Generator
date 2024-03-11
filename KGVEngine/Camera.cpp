//
// Created by Brett on 2024-03-09.
//

#include "Camera.h"

using namespace DirectX;

void KGV::Engine::Camera::setOrthographicProject(F32 left, F32 right, F32 top, F32 bottom, F32 nearPlane, F32 farPlane) {
    projectionMatrix = XMMatrixOrthographicLH( right - left, bottom - top, nearPlane, farPlane);
//    DirectX::XMFLOAT4X4A matrix;
//    matrix._11 = 2.0f / (right - left);
//    matrix._22 = 2.0f / (bottom - top);
//    matrix._33 = 1.0f / (farPlane - nearPlane);
//    matrix._41 = -(right + left) / (right - left);
//    matrix._42 = -(bottom + top) / (bottom - top);
//    matrix._43 = -nearPlane / (farPlane - nearPlane);
//    projectionMatrix = DirectX::XMLoadFloat4x4A(&matrix);
    m_isOrthographic = true;
}

void KGV::Engine::Camera::setPerspectiveProject(F32 fovY, F32 aspect, F32 nearPlane, F32 farPlane) {
    projectionMatrix = XMMatrixPerspectiveFovLH(fovY, aspect, nearPlane, farPlane);
//    auto tanHalfFovY = tan(fovY / 2.0f);
//    DirectX::XMFLOAT4X4A matrix;
//    matrix._11 = 1.0f / (aspect * tanHalfFovY);
//    matrix._22 = 1.0f / (tanHalfFovY);
//    matrix._33 = farPlane / (farPlane - nearPlane);
//    matrix._34 = 1.0f;
//    matrix._43 = -(farPlane * nearPlane) / (farPlane - nearPlane);
//    projectionMatrix = DirectX::XMLoadFloat4x4A(&matrix);
    m_isOrthographic = false;
}

const DirectX::XMMATRIX &KGV::Engine::Camera::getProjectionMatrix() {
    return projectionMatrix;
}

bool KGV::Engine::Camera::isActive() const {
    return m_isActive;
}

void KGV::Engine::Camera::setIsActive(bool isActive) {
    m_isActive = isActive;
}

bool KGV::Engine::Camera::isWireframe() const {
    return m_isWireframe;
}

void KGV::Engine::Camera::setIsWireframe(bool isWireframe) {
    m_isWireframe = isWireframe;
}

S32 KGV::Engine::Camera::getRtvId() const {
    return rtvId;
}

bool KGV::Engine::Camera::isOrthographic() const {
    return m_isOrthographic;
}

const DirectX::XMMATRIX &KGV::Engine::Camera::getViewMatrix() const {
    return viewMatrix;
}

void KGV::Engine::Camera::setViewMatrix(const DirectX::XMMATRIX &viewMatrix) {
    Camera::viewMatrix = viewMatrix;
}

const DirectX::XMVECTOR &KGV::Engine::Camera::getLookAt() const {
    return lookAt;
}

void KGV::Engine::Camera::setLookAt(const DirectX::XMVECTOR &lookAt) {
    Camera::lookAt = lookAt;
}

const DirectX::XMVECTOR &KGV::Engine::Camera::getUp() const {
    return up;
}

void KGV::Engine::Camera::setUp(const DirectX::XMVECTOR &up) {
    Camera::up = up;
}

const DirectX::XMVECTOR &KGV::Engine::Camera::getEye() const {
    return eye;
}

void KGV::Engine::Camera::setEye(const DirectX::XMVECTOR &eye) {
    Camera::eye = eye;
}

S32 KGV::Engine::Camera::getViewPortId() const {
    return viewPortId;
}

void KGV::Engine::Camera::setViewPortId(S32 viewPortId) {
    Camera::viewPortId = viewPortId;
}

void KGV::Engine::Camera::setRtvId(const S32 id) {
    rtvId = id;
}

