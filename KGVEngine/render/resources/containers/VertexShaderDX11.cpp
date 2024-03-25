//
// Created by Brett on 3/3/2024.
//

#include "VertexShaderDX11.h"

KGV::Render::VertexShaderDX11::VertexShaderDX11(const std::string &fileName, const std::string &function,
                                                const std::string &shaderModel, const ComPtr<ID3DBlob> &compiledShader,
                                                const ComPtr<ID3D11VertexShader> &vertexShader)
                                                : ShaderDX11(fileName, function, shaderModel, compiledShader),
                                                             vertexShader(vertexShader) {}

KGV::Render::eShaderType KGV::Render::VertexShaderDX11::getShaderType() {
    return kVertex;
}

const ComPtr<ID3D11VertexShader> &KGV::Render::VertexShaderDX11::getVertexShader() const {
    return vertexShader;
}

void KGV::Render::VertexShaderDX11::setVertexShader(const ComPtr<ID3D11VertexShader> &vertexShader) {
    VertexShaderDX11::vertexShader = vertexShader;
}
