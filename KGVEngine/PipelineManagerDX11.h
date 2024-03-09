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
#include "PipelineStateDX11.h"

namespace KGV::Render {
    class PipelineManagerDX11 {
    public:
        explicit PipelineManagerDX11(RenderDeviceDX11* device);
        void drawIndexed(S32 indexCount, S32 startIndex, S32 vertexOffset);

        void applyState(PipelineStateDX11 state);
        void applyState(InputAssemblerStateDX11 *iaState, ShaderStageStateDX11 *vsState, RasterizerStageStateDX11 *rsState,
                        ShaderStageStateDX11 *psState, OutputMergerStageStateDX11 *omState);

        void clearColorBuffers(DirectX::XMFLOAT4 color);

        void mapResource(S32 id, U32 subResource, D3D11_MAP actions, U32 flags);
        void mapResource(ResourceViewDX11* resource, U32 subResource, D3D11_MAP actions, U32 flags);
        void mapResource(ResourceDX11* resource, U32 subResource, D3D11_MAP actions, U32 flags);

        void unmapResource(S32 id, U32 subResource);
        void unmapResource(ResourceViewDX11* resource, U32 subResource);
        void unmapResource(ResourceDX11* resource, U32 subResource);

    protected:
        RenderDeviceDX11* device;
        ComPtr<ID3D11DeviceContext> context;
        ComPtr<ID3D11DeviceContext1> context1;
        InputAssemblerStageDX11 inputAsmStage;
        VertexShaderStageDX11 vertexShaderStage;
        RasterizerStageDX11 rasterStage;
        PixelShaderStageDX11 pixelShaderStage;
        OutputMergerStageDX11 outputMergerStage;
    };
}


#endif //ENGINE_PIPELINEMANAGERDX11_H
