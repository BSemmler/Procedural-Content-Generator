//
// Created by Brett on 3/3/2024.
//

#ifndef ENGINE_SHADERDX11_H
#define ENGINE_SHADERDX11_H

#include "pch.h"

namespace KGV::Render {
    enum eShaderType {
        kVertex = 0,
        kPixel = 1,
    };

    class ShaderDX11 {
    public:
        ShaderDX11(const std::string &fileName, const std::string &function, const std::string &shaderModel,
                   const ComPtr<ID3DBlob> &compiledShader);

        virtual eShaderType getShaderType() = 0;

        const std::string &getFileName() const;

        void setFileName(const std::string &fileName);

        const std::string &getFunction() const;

        void setFunction(const std::string &function);

        const std::string &getShaderModel() const;

        void setShaderModel(const std::string &shaderModel);

        const ComPtr<ID3DBlob> &getCompiledShader() const;

        void setCompiledShader(const ComPtr<ID3DBlob> &compiledShader);

    protected:
        std::string fileName;
        std::string function;
        std::string shaderModel;
        ComPtr<ID3DBlob> compiledShader;
    };

}


#endif //ENGINE_SHADERDX11_H
