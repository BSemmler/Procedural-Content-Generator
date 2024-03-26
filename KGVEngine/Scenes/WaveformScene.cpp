//
// Created by Brett on 2024-03-24.
//

#include "WaveformScene.h"
#include "imgui.h"
#include "render/renderers/simple/SimpleRenderer.h"

bool KGV::Engine::WaveformScene::Init(std::weak_ptr<Render::RenderDeviceDX11> device, std::weak_ptr<Render::PipelineManagerDX11> deviceContext,
                                      std::weak_ptr<Render::SimpleRenderer> renderer, std::shared_ptr<IShaderManager> shaderManager,
                                      S32 renderTargetId, S32 windowWidth, S32 windowHeight) {

    graphicsDevice = device;
    graphicsRenderer = renderer;
    graphicsDeviceContext = deviceContext;
    this->shaderManager = shaderManager;

    heightMapBuffer.resize(numVertices);
    SetupHeightMapTexture();
    SetupPrimaryCamera(windowWidth, windowHeight, renderTargetId);
    SetupEntities();
    GenerateWaveForm();

    return false;
}

void KGV::Engine::WaveformScene::Shutdown() {

}

void KGV::Engine::WaveformScene::Tick(float deltaTime) {
    auto regenWaveForm = false;
    ImGui::SeparatorText("Fractal Brownian Motion");
    if (isGradientNoise) {
        if (ImGui::SliderInt("Octaves", &terrainOctaves, 1, 16)) {
            regenWaveForm = true;
        }

    }

    if (ImGui::SliderFloat("Amplitude", &terrainAmplitude, 0.01f, 8.0f)) {
        regenWaveForm = true;
    }
    if (ImGui::SliderFloat("Frequency", &terrainFrequency, 0.01f, 8.0f)) {
        regenWaveForm = true;
    }

    if (isGradientNoise) {
        if (ImGui::SliderFloat("Gain/Persistence", &terrainPersistence, 0.01f, 4.0f)) {
            regenWaveForm = true;
        }
        if (ImGui::SliderFloat("Lacunarity", &terrainLacunarity, 0.01f, 4.0f)) {
            regenWaveForm = true;
        }
    }


    ImGui::SeparatorText("Noise Waveform");
    if (ImGui::RadioButton("Gradient", isGradientNoise)) {
        isGradientNoise = true;
        isRandomNoise = false;
        isCosNoise = false;
        isSinNoise = false;
        regenWaveForm = true;
    } else if (ImGui::RadioButton("Random", isRandomNoise)) {
        isGradientNoise = false;
        isRandomNoise = true;
        isCosNoise = false;
        isSinNoise = false;
        regenWaveForm = true;
    } else if (ImGui::RadioButton("Cos", isCosNoise)) {
        isGradientNoise = false;
        isRandomNoise = false;
        isCosNoise = true;
        isSinNoise = false;
        regenWaveForm = true;
    } else if (ImGui::RadioButton("Sin", isSinNoise)) {
        isGradientNoise = false;
        isRandomNoise = false;
        isCosNoise = false;
        isSinNoise = true;
        regenWaveForm = true;
    }

    if (regenWaveForm) {
        GenerateWaveForm();
    }

    if (auto renderer = graphicsRenderer.lock()) {
        renderer->renderScene(entities, cameras, nullptr, deltaTime);
    }
}

void KGV::Engine::WaveformScene::SetupPrimaryCamera(S32 width, S32 height, S32 renderTargetId) {
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
        heightMapCamera->camera->setOrthographicProject(160,
                                                        90, 0.01, 5);
        heightMapCamera->camera->setIsActive(true);
        heightMapCamera->transform.position.z = -1;
//        heightMapCamera->transform.position.x = numVertices / 2;
        cameras.emplace_back(heightMapCamera);
    }
}

void KGV::Engine::WaveformScene::SetupEntities() {
    if (auto renderer = graphicsRenderer.lock()) {
        std::vector<Render::Vertex> vertices;
        vertices.resize(numVertices);

        for (int i = 0; i < numVertices; ++i) {
            vertices[i].position = {static_cast<F32>(i - (numVertices / 2)) / 2, 0, 0};
            vertices[i].normal = { 0, 0, 0 };
            vertices[i].texCoord = { static_cast<F32>(i) / static_cast<F32>(numVertices), 0 };
        }

        std::vector<U32> indices;
        indices.reserve(numVertices * 2);
        for (int i = 0; i < numVertices - 1; i++) {
            indices.emplace_back(i);
            indices.emplace_back(i + 1);
//            indices[i] = i;
//            indices[i + 1] = i + 1;
        }


        // TODO: Need to fix material change detection to include textures so that single material pipeline definition can be reused.
        auto waveformMeshId = renderer->createMesh({vertices}, indices, Render::kImmutable, D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

        auto lineVertexShader = shaderManager->getShaderByName( "LineVertexShader", Render::eShaderType::kVertex);
        auto linePixelShader = shaderManager->getShaderByName( "LinePixelShader", Render::eShaderType::kPixel);
        auto inputLayoutId = shaderManager->getInputLayoutForShader(lineVertexShader); // all the shaders use the same layout.

        auto textureViewingPlane = std::make_shared<Engine::Entity>();
        textureViewingPlane->mesh = std::make_unique<Engine::MeshComponent>();
        textureViewingPlane->mesh->meshId = waveformMeshId;
        textureViewingPlane->material = std::make_unique<Engine::MaterialComponent>();
        textureViewingPlane->material->materialId = renderer->createMaterial(inputLayoutId, lineVertexShader, linePixelShader);
        textureViewingPlane->mesh->render = true;
        textureViewingPlane->material->mapTexture = heightMapTexture;
        textureViewingPlane->material->diffuse = { 1.0f, 1.0f, 1.0f, 1.0f};
        textureViewingPlane->material->displacement = 40;
//        textureViewingPlane->transform.rotation.z = 180;
//        textureViewingPlane->transform.scale.x = 9.0f;
//        textureViewingPlane->transform.scale.y = 9.0f;

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

void KGV::Engine::WaveformScene::GenerateWaveForm() {
    if (isGradientNoise) {
        for (int j = 0; j < numVertices; ++j) {
            auto freq = terrainFrequency;
            auto amplitude = terrainAmplitude;
            double sum = 0;
            double min = 0;
            double max = 0;
            for (int i = 0; i < terrainOctaves; ++i) {
                auto x = (static_cast<F32>(j) * freq) / static_cast<F32>(numVertices);
                auto noise = perlinNoiseGenerator.noiseDP(x, 0.5) * amplitude;
                sum += noise;
                min -= amplitude;
                max += amplitude;
                freq *= terrainLacunarity;
                amplitude *= terrainPersistence;
            }

            heightMapBuffer[j] = static_cast<F32>(sum / max);
        }
    } else if (isRandomNoise) {
        std::mt19937 mt(0);
        std::uniform_real_distribution<> distrib(-1.0,1.0);
        for (int i = 0; i < numVertices; ++i) {
            heightMapBuffer[i] = static_cast<F32>(distrib(mt)) * terrainAmplitude;
        }
    } else if (isSinNoise) {
        double step = 80 / 320;
        for (int i = 0; i < numVertices; ++i) {
            heightMapBuffer[i] = std::sin(static_cast<F64>(i) / ((numVertices / 16.0) * terrainFrequency)) * terrainAmplitude;
        }
    } else if (isCosNoise) {
        for (int i = 0; i < numVertices; ++i) {
            heightMapBuffer[i] = std::cos(static_cast<F64>(i) / ((numVertices / 16.0) * terrainFrequency)) * terrainAmplitude;
        }
    }


    if (auto deviceContext = graphicsDeviceContext.lock()) {

        int displacementRowPitch = sizeof(float);
        deviceContext->updateSubresource(heightMapTexture.get(), 0, nullptr, heightMapBuffer.data(), displacementRowPitch, numVertices * displacementRowPitch);
    }
}

void KGV::Engine::WaveformScene::SetupHeightMapTexture() {
    if (auto device = graphicsDevice.lock()) {
        Render::Texture1dConfigDX11 waveformTextureConfig;
        waveformTextureConfig.setColorTexture(numVertices);
        waveformTextureConfig.setFormat(DXGI_FORMAT_R32_FLOAT);

        Render::ShaderResourceViewConfigDX11 terrainMapSrvConfig{};

        D3D11_TEX1D_SRV srvDesc;
        srvDesc.MipLevels = 1;
        srvDesc.MostDetailedMip = 0;

        terrainMapSrvConfig.setTexture1D(srvDesc);
        heightMapTexture = device->CreateTexture1D(waveformTextureConfig, nullptr, &terrainMapSrvConfig);
    }
}
