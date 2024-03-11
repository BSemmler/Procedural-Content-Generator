//
// Created by Brett on 2024-03-09.
//

#include "SimpleRenderer.h"
using namespace DirectX;

struct ObjectConstantsDef {
    DirectX::XMMATRIX worldMatrix;
};

struct CameraConstantsDef {
    DirectX::XMMATRIX viewProjectionMatrix;
};

struct MaterialConstantsDef {
    DirectX::XMFLOAT4 color;
};

struct LightConstantsDef {
    DirectX::XMFLOAT4 color;
    DirectX::XMFLOAT4 direction;
    DirectX::XMFLOAT4 position;
};


struct FrameConstantsDef {
    F32 deltaTime;
    F32 pad1;
    F32 pad2;
    F32 pad3;
};


KGV::Render::SimpleRenderer::SimpleRenderer(Render::RenderDeviceDX11* device, Render::PipelineManagerDX11* deviceContext) {
    this->device = device;
    this->deviceContext = deviceContext;
    currentMesh = -1;
    currentMaterial = -1;

    BufferConfigDX11 cbufferConf{};
    cbufferConf.setDefaultConstantBuffer(sizeof(ObjectConstantsDef), eBufferUpdateType::kDynamic);
    vsObjectConstantsBuffer = device->createConstantBuffer(cbufferConf, nullptr);

    cbufferConf.setDefaultConstantBuffer(sizeof(CameraConstantsDef), eBufferUpdateType::kDynamic);
    vsCameraConstantsBuffer = device->createConstantBuffer(cbufferConf, nullptr);

    cbufferConf.setDefaultConstantBuffer(sizeof(FrameConstantsDef), eBufferUpdateType::kDynamic);
    vsFrameConstantsBuffer = device->createConstantBuffer(cbufferConf, nullptr);

    cbufferConf.setDefaultConstantBuffer(sizeof(MaterialConstantsDef), eBufferUpdateType::kDynamic);
    psMaterialConstantsBuffer = device->createConstantBuffer(cbufferConf, nullptr);

    cbufferConf.setDefaultConstantBuffer(sizeof(LightConstantsDef), eBufferUpdateType::kDynamic);
    psLightConstantsBuffer = device->createConstantBuffer(cbufferConf, nullptr);

    D3D11_RASTERIZER_DESC rsDesc{};
    rsDesc.FillMode = D3D11_FILL_SOLID;
    rsDesc.AntialiasedLineEnable = false;
    rsDesc.CullMode = D3D11_CULL_NONE;
    solidFillRasterState = device->createRasterizerState(rsDesc);
    rsDesc.FillMode = D3D11_FILL_WIREFRAME;
    wireFrameFillRasterState = device->createRasterizerState(rsDesc);
}

void KGV::Render::SimpleRenderer::renderScene(std::vector<std::shared_ptr<Engine::Entity>>& entities, std::vector<std::shared_ptr<Engine::Entity>>& cameras, std::vector<std::shared_ptr<Engine::Entity>>* lights, F32 deltaTime) {
    auto frameConstants = deviceContext->mapResource(vsFrameConstantsBuffer.get(), 0, D3D11_MAP_WRITE_DISCARD, 0);
    memcpy(frameConstants.pData, &deltaTime, sizeof(F32));
    deviceContext->unmapResource(vsFrameConstantsBuffer.get(), 0);

    LightConstantsDef lc{};
    lc.color = { 1.0f, 1.0f, 1.0f, 1.0f };
    if (lights && lights->at(0)->light) {
        auto lightEntity = lights->at(0);
//        lc.position = XMFLOAT4(lightEntity->transform.position);
        lc.color = lightEntity->light->color;
    }

    auto lightConstants = deviceContext->mapResource(psLightConstantsBuffer.get(), 0, D3D11_MAP_WRITE_DISCARD, 0);
    memcpy(lightConstants.pData, &lc, sizeof(LightConstantsDef));
    deviceContext->unmapResource(psLightConstantsBuffer.get(), 0);

    for (const auto& cameraEntity : cameras) {
        // If the entity doesn't have a cameraEntity setup then ignore, likewise if a cameraEntity is disabled.
        if (!cameraEntity->camera) {
            continue;
        } else if (!cameraEntity->camera->isActive()) {
            continue;
        }

        // Calculate camera position and subsequently the view matrix, the projection matrix, and the view-projection matrix.
        // Finally, copy the viewProjection matrix to its constant buffer.
        auto position = XMLoadFloat3A(&cameraEntity->transform.position);
        auto rotation = XMQuaternionRotationRollPitchYawFromVector(XMLoadFloat3A(&cameraEntity->transform.rotation));
        auto forward = XMVector3Rotate(XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f), rotation);
        auto up = XMVector3Rotate(XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f), rotation);
        auto view = XMMatrixLookAtLH(position, XMVectorAdd(position, forward), up);

        auto viewProj = view * cameraEntity->camera->getProjectionMatrix();
        auto buffer = deviceContext->mapResource(vsCameraConstantsBuffer.get(), 0, D3D11_MAP_WRITE_DISCARD, 0);
        memcpy(buffer.pData, &viewProj, sizeof(XMMATRIX));
        deviceContext->unmapResource(vsCameraConstantsBuffer.get(), 0);


        RasterizerStageStateDX11 rsState;
        rsState.setViewportIds({cameraEntity->camera->getViewPortId()});
        rsState.setRasterizerState(cameraEntity->camera->isWireframe() ? wireFrameFillRasterState : solidFillRasterState);
        bool changeRsState = true;

        OutputMergerStageStateDX11 omState;
        omState.setRtvIds({cameraEntity->camera->getRtvId()});
        bool changeOmState = true;

        // Render each entity;
        for (auto entity : entities) {
            auto mesh = entity->mesh.get();
            auto material = entity->material.get();
            if (!mesh || !material) {
                continue;
            } else if (!mesh->render) {
                continue;
            }

            auto world = entity->transform.matrix();
            auto objectConstants =deviceContext->mapResource(vsObjectConstantsBuffer.get(), 0, D3D11_MAP_WRITE_DISCARD, 0);
            memcpy(objectConstants.pData, &world, sizeof(XMMATRIX));
            deviceContext->unmapResource(vsObjectConstantsBuffer.get(), 0);

            // TODO: Move this logic of keep unchanged state elements to the individual stages.
            // Get current IA state in case it changes.
            InputAssemblerStateDX11 iaState{};
            iaState.setTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
            iaState.setIndexFormat(DXGI_FORMAT_R32_UINT);
            if (currentMaterial >= 0 || currentMesh >= 0) {
                iaState.setInputLayout(materials[currentMaterial].inputLayoutId);
                iaState.setVertexBuffers(meshes[currentMesh].vertexBuffers);
                iaState.setIndexBuffer(meshes[currentMesh].indexBuffer->getResourceId());
            }

            bool changeIaState = false;
            RenderMesh* renderMesh = &meshes[mesh->meshId];
            if (currentMesh != mesh->meshId) {
                currentMesh = mesh->meshId;
                iaState.setIndexBuffer(renderMesh->indexBuffer->getResourceId());
                iaState.setVertexBuffers(renderMesh->vertexBuffers, {sizeof(Vertex)}, {0});
                changeIaState = true;
            }

            auto materialConstants = deviceContext->mapResource(psMaterialConstantsBuffer.get(), 0, D3D11_MAP_WRITE_DISCARD, 0);
            memcpy(materialConstants.pData, &material->color, sizeof(XMVECTOR));
            deviceContext->unmapResource(psMaterialConstantsBuffer.get(), 0);

            bool changeVsState = false;
            bool changePsState = false;
            ShaderStageStateDX11 vsState{};
            ShaderStageStateDX11 psState{};
            if (currentMaterial != material->materialId) {
                currentMaterial = material->materialId;
                RenderMaterial* renderMat = &materials[material->materialId];
                iaState.setInputLayout(renderMat->inputLayoutId);

                vsState.setShaderId(renderMat->vertexShaderId);
                vsState.setConstantBuffersIds({vsObjectConstantsBuffer->getResourceId(),
                                               vsCameraConstantsBuffer->getResourceId(),
                                               vsFrameConstantsBuffer->getResourceId()});

                psState.setShaderId(renderMat->pixelShaderId);
                psState.setConstantBuffersIds({psMaterialConstantsBuffer->getResourceId(), psLightConstantsBuffer->getResourceId()}, 3); // TODO: Define start slot enum for various cbuffer positions.

                changeIaState = true;
                changeVsState = true;
                changePsState = true;
            }

            deviceContext->applyState(changeIaState ? &iaState : nullptr,
                                      changeVsState ? &vsState : nullptr,
                                      changeRsState ? &rsState : nullptr,
                                      changePsState ? &psState : nullptr,
                                      changeOmState ? &omState : nullptr);
            changeOmState = false;
            changeRsState = false;
            deviceContext->drawIndexed(renderMesh->indexCount, 0, 0);
        }
    }
}

void KGV::Render::SimpleRenderer::updateMesh(S32 id, const std::vector<std::vector<Vertex>>& vertices, const std::vector<U32>& indices) {
    auto mesh = &meshes[id];

    if (vertices.size() > mesh->vertexBuffers.size()) {
        return;
    }

    switch (mesh->updateType) {
        case kImmutable: {
            break;
        }
        case kDefault: {
            for (auto vBuffer : mesh->vertexBuffers) {
                deviceContext->updateSubresource(vBuffer.get(), 0, nullptr, vertices.data(), 0, 0);
            }

            deviceContext->updateSubresource(mesh->indexBuffer.get(), 0, nullptr, indices.data(), 0, 0);
            break;
        }
        case kDynamic: {
            for (int i = 0; i < mesh->vertexBuffers.size(); i++) {
                auto vertMap = deviceContext->mapResource(mesh->vertexBuffers[i].get(), 0, D3D11_MAP_WRITE_DISCARD, 0);
                memcpy(vertMap.pData, vertices[i].data(), sizeof(Vertex) * vertices[i].size());
                deviceContext->unmapResource(mesh->vertexBuffers[i].get(), 0);
            }

            auto indexMap = deviceContext->mapResource(mesh->indexBuffer.get(), 0, D3D11_MAP_WRITE_DISCARD, 0);
            memcpy(indexMap.pData, indices.data(), sizeof(U32) * indices.size());
            deviceContext->unmapResource(mesh->indexBuffer.get(), 0);
            break;
        }
    }
}

S32 KGV::Render::SimpleRenderer::createMesh(const std::vector<std::vector<Vertex>>& vertices, std::vector<U32>& indices, eBufferUpdateType update) {

    std::vector<std::shared_ptr<ResourceViewDX11>> vertexBuffers;
    for (const auto& vertexSet : vertices) {

        BufferConfigDX11 vertBufferConfig{};
        vertBufferConfig.setDefaultVertexBuffer(sizeof(Vertex) * vertexSet.size(), update);

        ResourceData data{};
        data.Data = vertexSet.data();
        vertexBuffers.emplace_back(device->createVertexBuffer(vertBufferConfig, &data));
    }

    RenderMesh mesh;
    mesh.vertexCount = static_cast<S32>(vertices[0].size());
    mesh.indexCount = static_cast<S32>(indices.size());
    mesh.offset = 0;
    mesh.id = static_cast<S32>(meshes.size());

    BufferConfigDX11 bufferConfig{};
    bufferConfig.setDefaultIndexBuffer(sizeof(U32) * indices.size(), update);

    ResourceData data{};
    data.Data = indices.data();
    auto indexResource = device->createIndexBuffer(bufferConfig, &data);

    mesh.indexBuffer = indexResource;
    mesh.vertexBuffers = vertexBuffers;

    meshes.emplace_back(std::move(mesh));
    return mesh.id;
}

S32 KGV::Render::SimpleRenderer::createMaterial(S32 inputLayoutId, S32 vertexShaderId, S32 pixelShaderId) {
    RenderMaterial mat{};
    mat.inputLayoutId = inputLayoutId;
    mat.vertexShaderId = vertexShaderId;
    mat.pixelShaderId = pixelShaderId;

    materials.emplace_back(mat);
    return static_cast<S32>(materials.size()) - 1;
}
