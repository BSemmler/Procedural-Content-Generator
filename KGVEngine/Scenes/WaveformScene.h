//
// Created by Brett on 2024-03-24.
//

#ifndef ENGINE_WAVEFORMSCENE_H
#define ENGINE_WAVEFORMSCENE_H

#include "IScene.h"
#include "Entity.h"
#include "IShaderManager.h"
#include "PerlinNoise.h"

namespace KGV::Engine {
    class WaveformScene : public IScene {
    public:
        bool Init(std::weak_ptr<Render::RenderDeviceDX11> device, std::weak_ptr<Render::PipelineManagerDX11> deviceContext,
                  std::weak_ptr<Render::SimpleRenderer> renderer, std::shared_ptr<IShaderManager> shaderManager,
                  S32 renderTargetId,S32 windowWidth, S32 windowHeight) override;

        void Shutdown() override;

        void Tick(float deltaTime) override;

    protected:
        void SetupPrimaryCamera(S32 width, S32 height, S32 renderTargetId);
        void SetupEntities();
        void SetupHeightMapTexture();
        void GenerateWaveForm();

        Procedural::PerlinNoise perlinNoiseGenerator;
        std::vector<F32> heightMapBuffer;
        std::weak_ptr<Render::RenderDeviceDX11> graphicsDevice;
        std::weak_ptr<Render::SimpleRenderer> graphicsRenderer;
        std::weak_ptr<Render::PipelineManagerDX11> graphicsDeviceContext;
        std::vector<std::shared_ptr<Entity>> cameras;
        std::vector<std::shared_ptr<Entity>> entities;
        std::shared_ptr<Render::ResourceViewDX11> heightMapTexture;
        std::shared_ptr<Render::ResourceViewDX11> depthBuffer;
        std::shared_ptr<IShaderManager> shaderManager;
        S32 terrainOctaves = 8;
        F32 terrainAmplitude = 1.0f;
        F32 terrainFrequency = 1.0f;
        F32 terrainPersistence = 0.45f;
        F32 terrainLacunarity = 2.0f;
        S32 numVertices = 160;
    };
}



#endif //ENGINE_WAVEFORMSCENE_H
