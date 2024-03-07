//
// Created by Brett on 3/3/2024.
//

#ifndef ENGINE_VERTEXSHADERDX11_H
#define ENGINE_VERTEXSHADERDX11_H

#include "pch.h"
#include "ShaderDX11.h"

namespace KGV::Render {
    class VertexShaderDX11 : public ShaderDX11 {
    public:
        VertexShaderDX11(const std::string &fileName, const std::string &function, const std::string &shaderModel,
                         const ComPtr<ID3DBlob> &compiledShader, const ComPtr<ID3D11VertexShader> &vertexShader);

        eShaderType getShaderType() override;

        const ComPtr<ID3D11VertexShader> &getVertexShader() const;

        void setVertexShader(const ComPtr<ID3D11VertexShader> &vertexShader);

    protected:
        ComPtr<ID3D11VertexShader> vertexShader;
    };
}



#endif //ENGINE_VERTEXSHADERDX11_H
