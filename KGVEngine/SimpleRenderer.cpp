//
// Created by Brett on 2024-03-09.
//

#include "SimpleRenderer.h"
using namespace DirectX;

struct DirectionalLight {
    DirectX::XMFLOAT4A ambient;
    DirectX::XMFLOAT4A diffuse;
    DirectX::XMFLOAT4A specular;
    DirectX::XMFLOAT3A direction;
};

struct PointLight {
    DirectX::XMFLOAT4A ambient;
    DirectX::XMFLOAT4A diffuse;
    DirectX::XMFLOAT4A specular;
    DirectX::XMFLOAT3 position;
    float range;
    DirectX::XMFLOAT3A attenuation;
};

struct SpotLight {
    DirectX::XMFLOAT4A ambient;
    DirectX::XMFLOAT4A diffuse;
    DirectX::XMFLOAT4A specular;

    DirectX::XMFLOAT3 position;
    float range;

    DirectX::XMFLOAT3 direction;
    float spot;

    DirectX::XMFLOAT3A attenuation;
};

struct ObjectConstantsDef {
    DirectX::XMMATRIX worldMatrix;
    DirectX::XMMATRIX worldInvTranspose;
};

struct CameraConstantsDef {
    DirectX::XMMATRIX viewProjectionMatrix;
    DirectX::XMVECTOR cameraPos;
};

struct MaterialConstantsDef {
    DirectX::XMFLOAT4A ambient;
    DirectX::XMFLOAT4A diffuse;
    DirectX::XMFLOAT4A specular;
    float displacement;
    float pad1;
    float pad2;
    float pad3;
};

//struct LightConstantsDef {
//    DirectX::XMFLOAT4A color;
//    DirectX::XMFLOAT4A position;
//    DirectX::XMFLOAT4A lightDir;
//};

struct FrameConstantsDef {
    DirectionalLight directionalLight;
    F32 deltaTime;
    F32 pad1;
    F32 pad2;
    F32 pad3;
};

XMMATRIX InverseTranspose(CXMMATRIX M) {
    auto A = M;
    A.r[3] = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);

    XMVECTOR det = XMMatrixDeterminant(A);
    return XMMatrixTranspose(XMMatrixInverse(&det, A));
}

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

//    cbufferConf.setDefaultConstantBuffer(sizeof(LightConstantsDef), eBufferUpdateType::kDynamic);
//    psLightConstantsBuffer = device->createConstantBuffer(cbufferConf, nullptr);

    D3D11_RASTERIZER_DESC rsDesc{};
    rsDesc.FillMode = D3D11_FILL_SOLID;
    rsDesc.AntialiasedLineEnable = false;
    rsDesc.CullMode = D3D11_CULL_BACK;
    solidFillRasterState = device->createRasterizerState(rsDesc);
    rsDesc.FillMode = D3D11_FILL_WIREFRAME;
    wireFrameFillRasterState = device->createRasterizerState(rsDesc);
}

void KGV::Render::SimpleRenderer::renderScene(std::vector<std::shared_ptr<Engine::Entity>>& entities, std::vector<std::shared_ptr<Engine::Entity>>& cameras, std::vector<std::shared_ptr<Engine::Entity>>* lights, F32 deltaTime) {
    FrameConstantsDef fcd{};
    fcd.deltaTime = deltaTime;
    if (lights) {
        auto lightEntity = (*lights)[0];
        auto light = lightEntity->light.get();
        fcd.directionalLight.ambient = light->ambient;
        fcd.directionalLight.diffuse = light->diffuse;
        fcd.directionalLight.specular = light->specular;

        // Calculate direction of the light.
        auto lightRotation = XMQuaternionRotationRollPitchYawFromVector(XMLoadFloat3A(&lightEntity->transform.rotation));
        XMStoreFloat3(&fcd.directionalLight.direction, XMVector3Rotate(XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f), lightRotation));
    }

    auto frameConstants = deviceContext->mapResource(vsFrameConstantsBuffer.get(), 0, D3D11_MAP_WRITE_DISCARD, 0);
    memcpy(frameConstants.pData, &fcd, sizeof(FrameConstantsDef));
    deviceContext->unmapResource(vsFrameConstantsBuffer.get(), 0);

    for (const auto& cameraEntity : cameras) {
        // If the entity doesn't have a cameraEntity setup then ignore, likewise if a cameraEntity is disabled.
        if (!cameraEntity->camera || !cameraEntity->camera->isActive()) {
            continue;
        }

        // Calculate camera camPos and subsequently the view matrix, the projection matrix, and the view-projection matrix.
        // Finally, copy the viewProjection matrix to its constant cameraConstantsBuff.
        auto camPos = XMLoadFloat3A(&cameraEntity->transform.position);
        auto camRotation = XMQuaternionRotationRollPitchYawFromVector(XMLoadFloat3A(&cameraEntity->transform.rotation));
        auto camForward = XMVector3Rotate(XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f), camRotation);
        auto camUp = XMVector3Rotate(XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f), camRotation);
        auto view = XMMatrixLookAtLH(camPos, XMVectorAdd(camPos, camForward), camUp);

        auto viewProj = view * cameraEntity->camera->getProjectionMatrix();
        auto cameraConstantsBuff = deviceContext->mapResource(vsCameraConstantsBuffer.get(), 0, D3D11_MAP_WRITE_DISCARD, 0);

        CameraConstantsDef ccd{};
        ccd.cameraPos = camPos;
        ccd.viewProjectionMatrix = viewProj;
        memcpy(cameraConstantsBuff.pData, &ccd, sizeof(CameraConstantsDef));
        deviceContext->unmapResource(vsCameraConstantsBuffer.get(), 0);

        RasterizerStageStateDX11 rsState;
        rsState.setViewportIds({cameraEntity->camera->getViewPortId()});
        rsState.setRasterizerState(cameraEntity->camera->isWireframe() ? wireFrameFillRasterState : solidFillRasterState);
        bool changeRsState = true;

        OutputMergerStageStateDX11 omState;
        omState.setRtvIds({cameraEntity->camera->getRtvId()});
        omState.setDsvId(cameraEntity->camera->getDsvId());
        omState.setDsvStateId(cameraEntity->camera->getDsvStateId());
        bool changeOmState = true;

        // Render each entity;
        for (const auto& entity : entities) {
            auto mesh = entity->mesh.get();
            auto material = entity->material.get();
            if (!mesh || !material || !mesh->render) {
                continue;
            }

            ObjectConstantsDef ocd{};
            ocd.worldMatrix = entity->transform.matrix();
            ocd.worldInvTranspose = InverseTranspose(ocd.worldMatrix);

            auto objectConstants =deviceContext->mapResource(vsObjectConstantsBuffer.get(), 0, D3D11_MAP_WRITE_DISCARD, 0);
            memcpy(objectConstants.pData, &ocd, sizeof(ObjectConstantsDef));
            deviceContext->unmapResource(vsObjectConstantsBuffer.get(), 0);

            // TODO: Move this logic of keep unchanged state elements to the individual stages.
            // Get current IA state in case it changes.
            InputAssemblerStateDX11 iaState{};
            iaState.setTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
            iaState.setIndexFormat(DXGI_FORMAT_R32_UINT);
            if (currentMaterial >= 0 || currentMesh >= 0) {
                iaState.setInputLayout(materials[currentMaterial].inputLayoutId);
                iaState.setVertexBuffers(meshes[currentMesh].vertexBuffers, {sizeof(Vertex)}, {0});
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
            MaterialConstantsDef mcd{};
            mcd.ambient = entity->material->ambient;
            mcd.diffuse = entity->material->diffuse;
            mcd.specular = entity->material->specular;
            mcd.displacement = entity->material->displacement;
            memcpy(materialConstants.pData, &mcd, sizeof(MaterialConstantsDef));
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
                                               vsFrameConstantsBuffer->getResourceId(),
                                               psMaterialConstantsBuffer->getResourceId()});
                if (material->mapTexture) {
                    auto m = material->mapTexture.get();
                    vsState.setSrvIds({m->getSrvId()});
//                    vsState.setSamplerIds({m->getSam})
                }

                psState.setShaderId(renderMat->pixelShaderId);
                psState.setConstantBuffersIds({
                    vsCameraConstantsBuffer->getResourceId(),
                    vsFrameConstantsBuffer->getResourceId(),
                    psMaterialConstantsBuffer->getResourceId()}, 1); // TODO: Define start slot enum for various cbuffer positions.

                if (!material->colorTextures.empty()) {
                    std::vector<S32> srvIds;
                    for(const auto& texture : material->colorTextures) {
                        srvIds.emplace_back(texture->getSrvId());
                    }

                    psState.setSamplerIds(material->samplerIds);
                    psState.setSrvIds(srvIds);
                }


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
            for (const auto& vBuffer : mesh->vertexBuffers) {
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
