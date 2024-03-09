//
// Created by Brett on 3/3/2024.
//

#include "PixelShaderDX11.h"

KGV::Render::PixelShaderDX11::PixelShaderDX11(const std::string &fileName, const std::string &function,
                                              const std::string &shaderModel, const ComPtr<ID3DBlob> &compiledShader,
                                              const ComPtr<ID3D11PixelShader> &pixelShader)
                                              : ShaderDX11(fileName, function, shaderModel, compiledShader),
                                              pixelShader(pixelShader) {}

KGV::Render::eShaderType KGV::Render::PixelShaderDX11::getShaderType() {
    return kPixel;
}

const ComPtr<ID3D11PixelShader> &KGV::Render::PixelShaderDX11::getPixelShader() const {
    return pixelShader;
}

void KGV::Render::PixelShaderDX11::setPixelShader(const ComPtr<ID3D11PixelShader> &pixelShader) {
    PixelShaderDX11::pixelShader = pixelShader;
}
