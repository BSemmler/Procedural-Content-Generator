//
// Created by Brett on 2024-03-09.
//

#include "Transform.h"

DirectX::XMMATRIX KGV::Engine::Transform::matrix() {
    const F32 c3 = cos(rotation.z);
    const F32 s3 = sin(rotation.z);
    const F32 c2 = cos(rotation.x);
    const F32 s2 = sin(rotation.x);
    const F32 c1 = cos(rotation.y);
    const F32 s1 = sin(rotation.y);
    return DirectX::XMMATRIX {
        scale.x * (c1 * c3 + s1 * s2 * s3),
        scale.x * (c2 * s3),
        scale.x * (c1 * s2 * s3 - c3 * s1),
        0.0f,

        scale.y * (c3 * s1 * s2 - c1 * s3),
        scale.y * (c2 * c3),
        scale.y * (c1 * c3 * s2 - s1 * s3),
        0.0f,

        scale.z * (c2 * s1),
        scale.z * (-s2),
        scale.z * (c1 * c2),
        0.0f,

        position.x,
        position.y,
        position.z,
        1.0f
    };
}
