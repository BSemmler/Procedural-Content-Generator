//
// Created by Brett on 2024-03-09.
//

#ifndef ENGINE_RENDERMESH_H
#define ENGINE_RENDERMESH_H

#include "pch.h"
#include "render/resources/ResourceViewDX11.h"

namespace KGV::Render {
    class RenderMesh {
    public:
        S32 id;
        std::vector<std::shared_ptr<Render::ResourceViewDX11>> vertexBuffers;
        std::shared_ptr<Render::ResourceViewDX11> indexBuffer;
        S32 vertexCount;
        S32 indexCount;
        S32 offset;
        eBufferUpdateType updateType;
        D3D11_PRIMITIVE_TOPOLOGY topology;
    };
}



#endif //ENGINE_RENDERMESH_H
