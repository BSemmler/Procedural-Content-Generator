//
// Created by Brett on 2024-03-09.
//

#ifndef ENGINE_MESH_H
#define ENGINE_MESH_H

#include "pch.h"
#include "ResourceViewDX11.h"

namespace KGV::Engine {
    class Mesh {
    public:
        Render::ResourceViewDX11 vertexBuffer;
        Render::ResourceViewDX11 indexBuffer;
        S32 vertexCount;
        S32 indexCount;
        S32 offset;
    };
}


#endif //ENGINE_MESH_H
