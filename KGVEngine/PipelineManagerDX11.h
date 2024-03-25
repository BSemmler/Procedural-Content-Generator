//
// Created by Brett on 3/7/2024.
//

#ifndef ENGINE_PIPELINEMANAGERDX11_H
#define ENGINE_PIPELINEMANAGERDX11_H

#include "pch.h"
//#include "RenderDeviceDX11.h"
#include "render/InputAssemblerStageDX11.h"
#include "VertexShaderStageDX11.h"
#include "PixelShaderStageDX11.h"
#include "RasterizerStageDX11.h"
#include "OutputMergerStageDX11.h"
#include "PipelineStateDX11.h"

namespace KGV::Render {
    class ResourceDX11;
//    class ResourceViewDX11;


    class PipelineManagerDX11 {
    public:
        explicit PipelineManagerDX11(RenderDeviceDX11* device, ComPtr<ID3D11DeviceContext> context, ComPtr<ID3D11DeviceContext1> context1);
        void drawIndexed(S32 indexCount, S32 startIndex, S32 vertexOffset);

        void applyState(PipelineStateDX11 state);
        void applyState(InputAssemblerStateDX11 *iaState = nullptr, ShaderStageStateDX11 *vsState = nullptr, RasterizerStageStateDX11 *rsState = nullptr,
                        ShaderStageStateDX11 *psState = nullptr, OutputMergerStageStateDX11 *omState = nullptr);

        void clearColorBuffers(DirectX::XMFLOAT4 color);
        void clearDepthStencilBuffers(float depth = 1.0f, U32 stencil = 0);

        D3D11_MAPPED_SUBRESOURCE mapResource(S32 id, U32 subResource, D3D11_MAP actions, U32 flags);
        D3D11_MAPPED_SUBRESOURCE mapResource(ResourceViewDX11* resource, U32 subResource, D3D11_MAP actions, U32 flags);
        D3D11_MAPPED_SUBRESOURCE mapResource(ResourceDX11* resource, U32 subResource, D3D11_MAP actions, U32 flags);

        void unmapResource(S32 id, U32 subResource);
        void unmapResource(ResourceViewDX11* resource, U32 subResource);
        void unmapResource(ResourceDX11* resource, U32 subResource);

        void updateSubresource(S32 id, U32 subResource, const D3D11_BOX* pDst, const void *pSrcData, U32 rowPitch, U32 depthPitch);
        void updateSubresource(ResourceViewDX11* resource, U32 subResource, const D3D11_BOX* pDst, const void *pSrcData, U32 rowPitch, U32 depthPitch);
        void updateSubresource(ResourceDX11* resource, U32 subResource,const D3D11_BOX* pDst, const void *pSrcData, U32 rowPitch, U32 depthPitch);

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
