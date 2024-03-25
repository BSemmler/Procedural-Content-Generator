//
// Created by Brett on 3/3/2024.
//

#ifndef ENGINE_PIXELSHADERDX11_H
#define ENGINE_PIXELSHADERDX11_H

#include "pch.h"
#include "ShaderDX11.h"

namespace KGV::Render {
    class PixelShaderDX11 : public ShaderDX11{
    public:
        PixelShaderDX11(const std::string &fileName, const std::string &function, const std::string &shaderModel,
                        const ComPtr<ID3DBlob> &compiledShader, const ComPtr<ID3D11PixelShader> &pixelShader);

        eShaderType getShaderType() override;

        const ComPtr<ID3D11PixelShader> &getPixelShader() const;

        void setPixelShader(const ComPtr<ID3D11PixelShader> &pixelShader);

    protected:
        ComPtr<ID3D11PixelShader> pixelShader;
    };

}


#endif //ENGINE_PIXELSHADERDX11_H
