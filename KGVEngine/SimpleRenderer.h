//
// Created by Brett on 2024-03-09.
//

#ifndef ENGINE_SIMPLERENDERER_H
#define ENGINE_SIMPLERENDERER_H

#include "pch.h"

#include "MeshComponent.h"
#include "Camera.h"
#include "Entity.h"
#include "RenderMesh.h"
#include "RenderMaterial.h"
#include "RenderDeviceDX11.h"

namespace KGV::Render {
    struct Vertex {
        DirectX::XMFLOAT4A position;
        DirectX::XMFLOAT3A normal;
        DirectX::XMFLOAT2A texCord;
    };

    class SimpleRenderer {
    public:
        SimpleRenderer(Render::RenderDeviceDX11* device, Render::PipelineManagerDX11* deviceContext);
        void renderScene(std::vector<std::shared_ptr<Engine::Entity>>& entities, std::vector<std::shared_ptr<Engine::Entity>>& cameras, F32 deltaTime);

        void updateMesh(S32 id, const std::vector<std::vector<Vertex>>& vertices, const std::vector<U32>& indices);
        S32 createMesh(const std::vector<std::vector<Vertex>>& vertices, std::vector<U32>& indices, eBufferUpdateType update);

        S32 createMaterial(S32 inputLayoutId, S32 vertexShaderId, S32 pixelShaderId);

    protected:
        std::vector<RenderMesh> meshes;
        Render::RenderDeviceDX11* device;
        Render::PipelineManagerDX11* deviceContext;
        std::shared_ptr<ResourceViewDX11> vsObjectConstantsBuffer;
        std::shared_ptr<ResourceViewDX11> vsCameraConstantsBuffer;
        std::shared_ptr<ResourceViewDX11> psMaterialConstantsBuffer;
        std::shared_ptr<ResourceViewDX11> vsFrameConstantsBuffer;
        std::vector<RenderMaterial> materials;
        std::shared_ptr<S32> inputLayouts;
        std::shared_ptr<S32> shaders;
        S32 currentMesh;
        S32 currentMaterial;
        S32 solidFillRasterState;
        S32 wireFrameFillRasterState;
    };
}



#endif //ENGINE_SIMPLERENDERER_H
