//
// Created by Brett on 3/7/2024.
//

#include "PipelineManagerDX11.h"

void KGV::Render::PipelineManagerDX11::drawIndexed(S32 indexCount, S32 startIndex, S32 vertexOffset) {
    context->DrawIndexed(indexCount, startIndex, vertexOffset);
}

void KGV::Render::PipelineManagerDX11::applyState(KGV::Render::PipelineStateDX11 state) {
    applyState(state.iaState, state.vsState, state.rsState, state.psState, state.omState);
}

void KGV::Render::PipelineManagerDX11::applyState(InputAssemblerStateDX11 *iaState, KGV::Render::ShaderStageStateDX11 *vsState,
                                                  KGV::Render::RasterizerStageStateDX11 *rsState,
                                                  KGV::Render::ShaderStageStateDX11 *psState,
                                                  KGV::Render::OutputMergerStageStateDX11 *omState) {

    if (iaState)
        inputAsmStage.setDesiredState(*iaState);

    if (vsState)
        vertexShaderStage.setDesiredState(*vsState);

    if (vsState)
        rasterStage.setDesiredState(*rsState);

    if (vsState)
        pixelShaderStage.setDesiredState(*psState);

    if (vsState)
        outputMergerStage.setDesiredState(*omState);
}

void KGV::Render::PipelineManagerDX11::clearColorBuffers(DirectX::XMFLOAT4 color) {
    auto renderTargetIds = outputMergerStage.getCurrentState().getRtvIds();
    for (auto id : renderTargetIds) {
        // TODO: This cast feels hacky...
        context->ClearRenderTargetView(device->getRtvById(id)->getView().Get(), static_cast<const FLOAT *>(&color.x));
    }
}

void KGV::Render::PipelineManagerDX11::mapResource(S32 id, U32 subResource, D3D11_MAP actions, U32 flags) {

}

void KGV::Render::PipelineManagerDX11::mapResource(KGV::Render::ResourceViewDX11 *resource, U32 subResource, D3D11_MAP actions, U32 flags) {

}

void KGV::Render::PipelineManagerDX11::mapResource(KGV::Render::ResourceDX11 *resource, U32 subResource, D3D11_MAP actions, U32 flags) {

}

void KGV::Render::PipelineManagerDX11::unmapResource(S32 id, U32 subResource) {

}

void KGV::Render::PipelineManagerDX11::unmapResource(KGV::Render::ResourceViewDX11 *resource, U32 subResource) {

}

void KGV::Render::PipelineManagerDX11::unmapResource(KGV::Render::ResourceDX11 *resource, U32 subResource) {

}

KGV::Render::PipelineManagerDX11::PipelineManagerDX11(KGV::Render::RenderDeviceDX11 *device) {
    device = device;
}
