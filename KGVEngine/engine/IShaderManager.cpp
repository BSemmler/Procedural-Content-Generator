//
// Created by Brett on 2024-03-24.
//

#include "IShaderManager.h"

#include <utility>

KGV::Engine::IShaderManager::IShaderManager(std::weak_ptr<Render::RenderDeviceDX11> _graphicsDevice) {
    graphicsDevice = std::move(_graphicsDevice);
}
