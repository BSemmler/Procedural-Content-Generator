//
// Created by Brett on 2024-03-12.
//

#ifndef ENGINE_GEOMETRYFACTORY_H
#define ENGINE_GEOMETRYFACTORY_H

#include "pch.h"
#include "SimpleRenderer.h"

namespace KGV::Engine {
    class GeometryFactory {
    public:
        static void getCube(bool hasFlatNormalGeometry, std::vector<DirectX::XMFLOAT3>& vertices, std::vector<DirectX::XMFLOAT3> &normals, std::vector<U32> &indices);
        static void getVertexGridU16(U32 size, std::vector<DirectX::XMFLOAT3>& vertices, std::vector<DirectX::XMFLOAT3> &normals, std::vector<U16> &indices, float step);
        static void getVertexGridU32(U32 size, std::vector<Render::Vertex> &vertices, std::vector<U32> &indices, float step);
        static void getPlane(std::vector<Render::Vertex> &vertices, std::vector<U32> &indices);
    };
};



#endif //ENGINE_GEOMETRYFACTORY_H
