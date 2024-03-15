//
// Created by Brett on 3/7/2024.
//

#include "PipelineManagerDX11.h"
#include "RenderDeviceDX11.h"

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

    if (iaState) {
        inputAsmStage.setDesiredState(*iaState);
        inputAsmStage.applyDesiredState(context, device);
    } else {

    }

    if (vsState) {
        vertexShaderStage.setDesiredState(*vsState);
        vertexShaderStage.applyDesiredState(context, device);
    }

    if (rsState) {
        rasterStage.setDesiredState(*rsState);
        rasterStage.applyDesiredState(context, device);
    }

    if (psState) {
        pixelShaderStage.setDesiredState(*psState);
        pixelShaderStage.applyDesiredState(context, device);
    }

    if (omState) {
        outputMergerStage.setDesiredState(*omState);
        outputMergerStage.applyDesiredState(context, device);
    }
}

void KGV::Render::PipelineManagerDX11::clearColorBuffers(DirectX::XMFLOAT4 color) {
    auto renderTargetIds = outputMergerStage.getCurrentState().getRtvIds();
    float colorArr[] = { color.x, color.y, color.z, color.w};

    for (auto id : renderTargetIds) {
        // TODO: This cast feels hacky...
        context->ClearRenderTargetView(device->getRtvById(id)->getView().Get(), colorArr);
    }
}

D3D11_MAPPED_SUBRESOURCE KGV::Render::PipelineManagerDX11::mapResource(S32 id, U32 subResource, D3D11_MAP actions, U32 flags) {
    auto r = device->getResourceById(id);
    return mapResource(r, subResource, actions, flags);
}

D3D11_MAPPED_SUBRESOURCE KGV::Render::PipelineManagerDX11::mapResource(KGV::Render::ResourceViewDX11 *resource, U32 subResource, D3D11_MAP actions, U32 flags) {
    auto r = device->getResourceById(resource->getResourceId());
    return mapResource(r, subResource, actions, flags);
}

D3D11_MAPPED_SUBRESOURCE KGV::Render::PipelineManagerDX11::mapResource(KGV::Render::ResourceDX11 *resource, U32 subResource, D3D11_MAP actions, U32 flags) {
    D3D11_MAPPED_SUBRESOURCE data;
    data.pData = nullptr;
    data.RowPitch = 0;
    data.DepthPitch = 0;

    // TODO: Add logging
    if (resource == nullptr) {
        return data;
    }

    ComPtr<ID3D11Resource> r = resource->getResource();
    if (!r) {
        return data;
    }

    context->Map(r.Get(), subResource, actions, flags, &data);
    return data;
}

void KGV::Render::PipelineManagerDX11::unmapResource(S32 id, U32 subResource) {
    auto r = device->getResourceById(id);
    return unmapResource(r, subResource);
}

void KGV::Render::PipelineManagerDX11::unmapResource(KGV::Render::ResourceViewDX11 *resource, U32 subResource) {
    auto r = device->getResourceById(resource->getResourceId());
    return unmapResource(r, subResource);
}

void KGV::Render::PipelineManagerDX11::unmapResource(KGV::Render::ResourceDX11 *resource, U32 subResource) {

    if (resource == nullptr) {
        return;
    }

    ComPtr<ID3D11Resource> r = resource->getResource();
    if (!r) {
        return;
    }

    context->Unmap(r.Get(), subResource);
}

KGV::Render::PipelineManagerDX11::PipelineManagerDX11(KGV::Render::RenderDeviceDX11* device, ComPtr<ID3D11DeviceContext> context, ComPtr<ID3D11DeviceContext1> context1) {
    this->device = device;
    this->context = context;
    this->context1 = context1;
}

void KGV::Render::PipelineManagerDX11::updateSubresource(S32 id, U32 subResource, const D3D11_BOX* pDst, const void *pSrcData, U32 rowPitch,
                                                         U32 depthPitch) {
    auto r = device->getResourceById(id);
    updateSubresource(r, subResource, pDst, pSrcData, rowPitch, depthPitch);
}

void KGV::Render::PipelineManagerDX11::updateSubresource(KGV::Render::ResourceViewDX11 *resource, U32 subResource, const D3D11_BOX* pDst, const void *pSrcData, U32 rowPitch,
                                                         U32 depthPitch) {
    auto r = device->getResourceById(resource->getResourceId());
    updateSubresource(r, subResource, pDst, pSrcData, rowPitch, depthPitch);
}

void KGV::Render::PipelineManagerDX11::updateSubresource(ResourceDX11* resource, U32 subResource, const D3D11_BOX* pDst, const void *pSrcData, U32 rowPitch,
                                                         U32 depthPitch) {
    if (!resource) {
        return;
    }

    auto r = resource->getResource();
    if (!r) {
        return;
    }

    context->UpdateSubresource(r, subResource, pDst, pSrcData, rowPitch, depthPitch);
}

void KGV::Render::PipelineManagerDX11::clearDepthStencilBuffers(float depth, U32 stencil) {
    auto depthStencilBuff = device->getDsvById(outputMergerStage.getCurrentState().getDsvId());

    if (!depthStencilBuff) {
        return;
    }

    context->ClearDepthStencilView(depthStencilBuff->getView().Get(), D3D11_CLEAR_DEPTH, depth, stencil);
}
