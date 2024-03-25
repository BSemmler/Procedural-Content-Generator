//
// Created by Brett on 2024-03-23.
//

#ifndef ENGINE_TERRAINSCENE_H
#define ENGINE_TERRAINSCENE_H

#include "IScene.h"
#include "Entity.h"
#include "IShaderManager.h"

namespace KGV::Engine {
    class TerrainScene : public IScene {
    public:
        bool Init(std::weak_ptr<Render::RenderDeviceDX11> device, std::weak_ptr<Render::PipelineManagerDX11> deviceContext,
                  std::weak_ptr<Render::SimpleRenderer> renderer, std::shared_ptr<IShaderManager> shaderManager,
                  S32 renderTargetId, S32 windowWidth, S32 windowHeight) override;


        bool Init(std::weak_ptr<Render::RenderDeviceDX11> device, std::weak_ptr<Render::PipelineManagerDX11> deviceContext,
                  std::weak_ptr<Render::SimpleRenderer> renderer, std::shared_ptr<IShaderManager> shaderManager, S32 renderTargetId, S32 windowWidth, S32 windowHeight,
                  const std::shared_ptr<Render::ResourceViewDX11>& displacementTexture, S32 mapSize);

        void Shutdown() override;

        void Tick(float deltaTime) override;

        S32 mapSize;

    protected:
        bool SetupPrimaryCamera(S32 width, S32 height, S32 renderTargetId);
        bool LoadTextures();
        void SetupEntities();


        std::weak_ptr<Render::RenderDeviceDX11> graphicsDevice;
        std::weak_ptr<Render::SimpleRenderer> graphicsRenderer;
        std::weak_ptr<Render::PipelineManagerDX11> graphicsDeviceContext;
        std::vector<std::shared_ptr<Entity>> cameras;
        std::vector<std::shared_ptr<Entity>> lights;
        std::vector<std::shared_ptr<Entity>> entities;
        std::shared_ptr<Render::ResourceViewDX11> terrainMapDisplacementTextureF32;
        std::shared_ptr<Render::ResourceViewDX11> rockTexture;
        std::shared_ptr<Render::ResourceViewDX11> sandTexture;
        std::shared_ptr<Render::ResourceViewDX11> grassTexture;
        std::shared_ptr<Render::ResourceViewDX11> snowTexture;
        std::shared_ptr<Render::ResourceViewDX11> depthBuffer;
        std::shared_ptr<IShaderManager> shaderManager;
        S32 terrainWrapSampler;

    };
}



#endif //ENGINE_TERRAINSCENE_H
