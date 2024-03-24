//
// Created by Brett on 2024-03-24.
//

#ifndef ENGINE_ISHADERMANAGER_H
#define ENGINE_ISHADERMANAGER_H

#include "pch.h"
#include "ShaderDX11.h"

namespace KGV::Render {
    class RenderDeviceDX11;
}

namespace KGV::Engine {
    class IShaderManager {
    public:
        IShaderManager() = delete; // Explicitly mark the default constuctor as deleted so inherited classes have to implement constructor.
        explicit IShaderManager(std::weak_ptr<Render::RenderDeviceDX11> _graphicsDevice);
        virtual S32 loadShader(std::string path, std::string function, std::string name,
                               Render::eShaderType shaderType, std::string shaderModel) = 0;

        virtual S32 getShaderByName(std::string name, Render::eShaderType shaderType) = 0;

        virtual S32 createInputLayout(std::vector<D3D11_INPUT_ELEMENT_DESC>& inputElements, S32 shaderId) = 0;
        virtual S32 setInputLayoutForShader(S32 shaderId, S32 inputLayoutId) = 0;
        virtual S32 getInputLayoutForShader(S32 id) = 0;
    protected:
        std::weak_ptr<Render::RenderDeviceDX11> graphicsDevice;
    };
}




#endif //ENGINE_ISHADERMANAGER_H
