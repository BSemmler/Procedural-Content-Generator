//
// Created by Brett on 2024-03-24.
//

#include "ShaderManager.h"
#include "RenderDeviceDX11.h"

S32
KGV::Engine::ShaderManager::loadShader(std::string path, std::string function, std::string name,
                                       KGV::Render::eShaderType shaderType, std::string shaderModel) {
    if (auto device = graphicsDevice.lock()) {
        auto shaderId = device->loadShader(path, shaderType, false, function, shaderModel);
        if (shaderId < 0) {
            return -1;
        }

        shaders.insert({{name, shaderType}, shaderId});
        return shaderId;
    }

    return -1;
}

S32 KGV::Engine::ShaderManager::getShaderByName(std::string name, KGV::Render::eShaderType shaderType) {
    if (auto shaderIt = shaders.find({name, shaderType}); shaderIt != shaders.end()) {
        return shaderIt->second;
    }

    return -1;
}

S32 KGV::Engine::ShaderManager::getInputLayoutForShader(S32 id) {
    if (auto inputLayoutIt = inputLayoutsForShaders.find(id); inputLayoutIt != inputLayoutsForShaders.end()) {
        return inputLayoutIt->second;
    }

    return -1;
}

S32 KGV::Engine::ShaderManager::setInputLayoutForShader(S32 shaderId, S32 inputLayoutId) {
    inputLayoutsForShaders.insert({shaderId, inputLayoutId});
    return 0;
}

S32 KGV::Engine::ShaderManager::createInputLayout(std::vector<D3D11_INPUT_ELEMENT_DESC> &inputElements, S32 shaderId) {
    if (auto device = graphicsDevice.lock()) {
        auto layoutId = device->createInputLayout(shaderId, inputElements);
        if (layoutId < 0)
            return layoutId;

        inputLayoutsForShaders.insert({shaderId, layoutId});
        return layoutId;
    }

    return -1;
}
