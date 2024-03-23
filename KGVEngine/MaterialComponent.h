//
// Created by Brett on 2024-03-10.
//

#ifndef ENGINE_MATERIALCOMPONENT_H
#define ENGINE_MATERIALCOMPONENT_H

#include "pch.h"

namespace KGV::Render {
    class ResourceViewDX11;
}

namespace KGV::Engine {
    class MaterialComponent {
    public:
        S32 materialId;
        DirectX::XMFLOAT4A ambient;
        DirectX::XMFLOAT4A diffuse;
        DirectX::XMFLOAT4A specular {0.1f, 0.1f, 0.1f, 16};
        float displacement;
//        float
        std::shared_ptr<Render::ResourceViewDX11> mapTexture;
        std::vector<std::shared_ptr<Render::ResourceViewDX11>> colorTextures;
        std::vector<S32> samplerIds;
    };
}



#endif //ENGINE_MATERIALCOMPONENT_H
