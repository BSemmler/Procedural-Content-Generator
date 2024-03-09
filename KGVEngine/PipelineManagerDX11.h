//
// Created by Brett on 3/7/2024.
//

#ifndef ENGINE_PIPELINEMANAGERDX11_H
#define ENGINE_PIPELINEMANAGERDX11_H

#include "pch.h"
#include "InputAssemblerStageDX11.h"
#include "VertexShaderStageDX11.h"
#include "PixelShaderStageDX11.h"
#include "RasterizerStageDX11.h"
#include "OutputMergerStageDX11.h"

namespace KGV::Render {
    class PipelineManagerDX11 {
    protected:
        InputAssemblerStageDX11 inputAsmStage;
        VertexShaderStageDX11 vertexShaderStage;
        RasterizerStageDX11 rasterStage;
        PixelShaderStageDX11 pixelShaderStage;
        OutputMergerStageDX11 outputMergerStage;
    };
}



#endif //ENGINE_PIPELINEMANAGERDX11_H
