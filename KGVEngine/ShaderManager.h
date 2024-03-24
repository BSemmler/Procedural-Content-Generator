//
// Created by Brett on 2024-03-24.
//

#ifndef ENGINE_SHADERMANAGER_H
#define ENGINE_SHADERMANAGER_H

#include "IShaderManager.h"

namespace KGV::Engine {
    class ShaderManager : public IShaderManager {
    public:
        explicit ShaderManager(const std::weak_ptr<Render::RenderDeviceDX11> &graphicsDevice) : IShaderManager(graphicsDevice) {}

        S32 loadShader(std::string path, std::string function, std::string name, Render::eShaderType shaderType, std::string shaderModel) override;

        S32 getShaderByName(std::string name, Render::eShaderType shaderType) override;

        S32 createInputLayout(std::vector<D3D11_INPUT_ELEMENT_DESC> &inputElements, S32 shaderId) override;

        S32 setInputLayoutForShader(S32 shaderId, S32 inputLayoutId) override;

        S32 getInputLayoutForShader(S32 id) override;


    protected:
        std::map<std::pair<std::string, Render::eShaderType>, S32> shaders;
        std::map<S32, S32> inputLayoutsForShaders;
    };
}


#endif //ENGINE_SHADERMANAGER_H
