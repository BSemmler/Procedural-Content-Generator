//
// Created by Brett on 3/3/2024.
//

#include "ShaderDX11.h"

const std::string &KGV::Render::ShaderDX11::getFileName() const {
    return fileName;
}

void KGV::Render::ShaderDX11::setFileName(const std::string &fileName) {
    ShaderDX11::fileName = fileName;
}

const std::string &KGV::Render::ShaderDX11::getFunction() const {
    return function;
}

void KGV::Render::ShaderDX11::setFunction(const std::string &function) {
    ShaderDX11::function = function;
}

const std::string &KGV::Render::ShaderDX11::getShaderModel() const {
    return shaderModel;
}

void KGV::Render::ShaderDX11::setShaderModel(const std::string &shaderModel) {
    ShaderDX11::shaderModel = shaderModel;
}

const ComPtr<ID3DBlob> &KGV::Render::ShaderDX11::getCompiledShader() const {
    return compiledShader;
}

void KGV::Render::ShaderDX11::setCompiledShader(const ComPtr<ID3DBlob> &compiledShader) {
    ShaderDX11::compiledShader = compiledShader;
}

KGV::Render::ShaderDX11::ShaderDX11(const std::string &fileName, const std::string &function,
                                    const std::string &shaderModel, const ComPtr<ID3DBlob> &compiledShader) : fileName(
        fileName), function(function), shaderModel(shaderModel), compiledShader(compiledShader) {}
