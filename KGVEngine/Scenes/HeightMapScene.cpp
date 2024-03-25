//
// Created by Brett on 2024-03-24.
//

#include "HeightMapScene.h"
#include "render/renderers/simple/SimpleRenderer.h"
#include "utility/GeometryFactory.h"

bool KGV::Engine::HeightMapScene::Init(std::weak_ptr<Render::RenderDeviceDX11> device, std::weak_ptr<Render::PipelineManagerDX11> deviceContext,
                                       std::weak_ptr<Render::SimpleRenderer> renderer, std::shared_ptr<IShaderManager> shaderManager,
                                       S32 renderTargetId, S32 windowWidth, S32 windowHeight) {
    graphicsDevice = device;
    graphicsRenderer = renderer;
    this->shaderManager = std::move(shaderManager);
    SetupEntities();
    SetupPrimaryCamera(windowWidth, windowHeight, renderTargetId);

    return true;
}

bool KGV::Engine::HeightMapScene::Init(std::weak_ptr<Render::RenderDeviceDX11> device, std::weak_ptr<Render::PipelineManagerDX11> deviceContext,
                                       std::weak_ptr<Render::SimpleRenderer> renderer, std::shared_ptr<IShaderManager> shaderManager,
                                       S32 renderTargetId, S32 windowWidth, S32 windowHeight,
                                       const std::shared_ptr<Render::ResourceViewDX11>& displacementTexture) {

    heightMapTexture = displacementTexture;
    Init(std::move(device), std::move(deviceContext), std::move(renderer), std::move(shaderManager), renderTargetId, windowWidth, windowHeight);
    return true;
}

void KGV::Engine::HeightMapScene::Shutdown() {

}

void KGV::Engine::HeightMapScene::Tick(float deltaTime) {
    if (auto renderer = graphicsRenderer.lock()) {
        renderer->renderScene(entities, cameras, nullptr, deltaTime);
    }
}



void KGV::Engine::HeightMapScene::SetupEntities() {
    if (auto renderer = graphicsRenderer.lock()) {
        std::vector<Render::Vertex> planeVertices;
        std::vector<U32> planeIndices;
        Engine::GeometryFactory::getPlane(planeVertices, planeIndices);

        // TODO: Need to fix material change detection to include textures so that single material pipeline definition can be reused.
        auto planeMeshId = renderer->createMesh({planeVertices}, planeIndices, Render::kImmutable);

        auto basicTransformVertexShader = shaderManager->getShaderByName( "basicShaderVs", Render::eShaderType::kVertex);
        auto unlitShaderPixelShader = shaderManager->getShaderByName( "unlitShaderPs", Render::eShaderType::kPixel);
        auto inputLayoutId = shaderManager->getInputLayoutForShader(basicTransformVertexShader); // all of the following shaders use the same layout.

        auto textureViewingPlane = std::make_shared<Engine::Entity>();
        textureViewingPlane->mesh = std::make_unique<Engine::MeshComponent>();
        textureViewingPlane->mesh->meshId = planeMeshId;
        textureViewingPlane->material = std::make_unique<Engine::MaterialComponent>();
        textureViewingPlane->material->materialId = renderer->createMaterial(inputLayoutId, basicTransformVertexShader, unlitShaderPixelShader);
        textureViewingPlane->mesh->render = true;
        textureViewingPlane->material->colorTextures.emplace_back(heightMapTexture);
        textureViewingPlane->transform.rotation.z = 180;
        textureViewingPlane->transform.scale.x = 9.0f;
        textureViewingPlane->transform.scale.y = 9.0f;

        if (auto device = graphicsDevice.lock()) {
            D3D11_SAMPLER_DESC samplerDesc{};
            samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
            samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
            samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;

            textureViewingPlane->material->samplerIds.emplace_back(device->createSamplerState(samplerDesc));
        }

        entities.emplace_back(textureViewingPlane);
    }
}

void KGV::Engine::HeightMapScene::SetupPrimaryCamera(S32 width, S32 height, S32 renderTargetId) {
    if (auto device = graphicsDevice.lock()) {
        Render::Texture2dConfigDX11 texConfig;
        texConfig.setDepthTexture(width, height);
        texConfig.setFormat(DXGI_FORMAT_D32_FLOAT);

        DXGI_SAMPLE_DESC sampleDesc;
        sampleDesc.Quality = D3D11_STANDARD_MULTISAMPLE_PATTERN;
        sampleDesc.Count = 4;
        texConfig.setSampleDesc(sampleDesc);

        Render::DepthStencilViewConfigDX11 dsvConfig;
        dsvConfig.setFormat(DXGI_FORMAT_D32_FLOAT);
        D3D11_TEX2D_DSV dsvState;
        dsvState.MipSlice = 0;
        dsvConfig.SetTexture2D(dsvState);
        depthBuffer = device->CreateTexture2D(texConfig, nullptr, nullptr, nullptr, &dsvConfig);

        D3D11_DEPTH_STENCIL_DESC dsDesc;
        // Depth test parameters
        dsDesc.DepthEnable = true;
        dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
        dsDesc.DepthFunc = D3D11_COMPARISON_LESS;

        // Stencil test parameters
        dsDesc.StencilEnable = true;
        dsDesc.StencilReadMask = 0xFF;
        dsDesc.StencilWriteMask = 0xFF;

        // Stencil operations if pixel is front-facing
        dsDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
        dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
        dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
        dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

        // Stencil operations if pixel is back-facing
        dsDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
        dsDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
        dsDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
        dsDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

        D3D11_VIEWPORT vp;
        vp.MinDepth = 0;
        vp.MaxDepth = 1;
        vp.TopLeftX = 0;
        vp.TopLeftY = 0;
        vp.Width = static_cast<F32>(width);
        vp.Height = static_cast<F32>(height);

        auto heightMapCamera = std::make_shared<Engine::Entity>();
        heightMapCamera->camera = std::make_unique<Engine::Camera>();
        heightMapCamera->camera->setDsvStateId(device->createDepthStencilState(dsDesc));
        heightMapCamera->camera->setDsvId(depthBuffer->getDsvId());
        heightMapCamera->camera->setRtvId(renderTargetId);
        heightMapCamera->camera->setViewPortId(device->createViewPort(vp));
        heightMapCamera->camera->setOrthographicProject(16.0f, 9.0, 0.01, 5);
        heightMapCamera->camera->setIsActive(true);
        heightMapCamera->transform.position.z = -1;
        cameras.emplace_back(heightMapCamera);
    }
}
