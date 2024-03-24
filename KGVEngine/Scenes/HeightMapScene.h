//
// Created by Brett on 2024-03-24.
//

#ifndef ENGINE_HEIGHTMAPSCENE_H
#define ENGINE_HEIGHTMAPSCENE_H

#include "IScene.h"
#include "IShaderManager.h"
#include "Entity.h"

namespace KGV::Engine {
    class HeightMapScene : public IScene {
    public:
        bool Init(std::weak_ptr<Render::RenderDeviceDX11> device, std::weak_ptr<Render::SimpleRenderer> renderer, S32 renderTargetId,
                  S32 windowWidth, S32 windowHeight) override;

        bool Init(std::weak_ptr<Render::RenderDeviceDX11> device, std::weak_ptr<Render::SimpleRenderer> renderer,
                  S32 renderTargetId, S32 windowWidth, S32 windowHeight, std::shared_ptr<IShaderManager> shaderManager,
                  const std::shared_ptr<Render::ResourceViewDX11>& displacementTexture);

        void Shutdown() override;

        void Tick(float deltaTime) override;

    protected:
        void SetupPrimaryCamera(S32 width, S32 height, S32 renderTargetId);
        void SetupEntities();

        std::weak_ptr<Render::RenderDeviceDX11> graphicsDevice;
        std::weak_ptr<Render::SimpleRenderer> graphicsRenderer;
        std::vector<std::shared_ptr<Entity>> cameras;
        std::vector<std::shared_ptr<Entity>> entities;
        std::shared_ptr<Render::ResourceViewDX11> heightMapTexture;
        std::shared_ptr<Render::ResourceViewDX11> depthBuffer;
        std::shared_ptr<IShaderManager> shaderManager;
    };
}



#endif //ENGINE_HEIGHTMAPSCENE_H
