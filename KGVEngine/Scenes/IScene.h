//
// Created by Brett on 2024-03-23.
//

#ifndef ENGINE_ISCENE_H
#define ENGINE_ISCENE_H

#include "pch.h"

// Forward declare these classes since we don't actually need the headers.
namespace KGV::Render {
    class RenderDeviceDX11;
    class SimpleRenderer;
    class PipelineManagerDX11;
}

namespace KGV::Engine {
    class IShaderManager;


    class IScene {
    public:
        virtual bool Init(std::weak_ptr<Render::RenderDeviceDX11> device, std::weak_ptr<Render::PipelineManagerDX11> deviceContext,
                          std::weak_ptr<Render::SimpleRenderer> renderer, std::shared_ptr<Engine::IShaderManager> shaderManager,
                          S32 renderTargetId, S32 windowWidth, S32 windowHeight) = 0;

        virtual void Shutdown() = 0;
        virtual void Tick(float deltaTime) = 0;
    };
}



#endif //ENGINE_ISCENE_H
