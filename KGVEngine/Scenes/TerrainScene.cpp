//
// Created by Brett on 2024-03-23.
//

#include "TerrainScene.h"

#include <utility>
#include "SimpleRenderer.h"
#include "TargaImage.h"
#include "GeometryFactory.h"
#include "imgui.h"

using namespace DirectX;

bool KGV::Engine::TerrainScene::Init(std::weak_ptr<Render::RenderDeviceDX11> device, std::weak_ptr<Render::SimpleRenderer> renderer,
                                     S32 renderTargetId, S32 windowWidth, S32 windowHeight) {

    graphicsDevice = device;
    graphicsRenderer = renderer;
    LoadTextures();
    SetupPrimaryCamera(windowWidth, windowHeight, renderTargetId);
    SetupEntities();

    return true;
}

void KGV::Engine::TerrainScene::Shutdown() {

}

void KGV::Engine::TerrainScene::Tick(float deltaTime) {
    ImGui::Text("Terrain Height");
    ImGui::SliderFloat("##TerrainHeight", &entities[0]->transform.position.y, -256, 256);

    ImGui::Checkbox("Draw Water", &entities[1]->mesh->render);

    if (auto renderer = graphicsRenderer.lock()) {
        renderer->renderScene(entities, cameras, &lights, deltaTime);
    }
}

bool KGV::Engine::TerrainScene::Init(std::weak_ptr<Render::RenderDeviceDX11> device, std::weak_ptr<Render::SimpleRenderer> renderer,
                                     S32 renderTargetId, S32 windowWidth, S32 windowHeight, std::shared_ptr<IShaderManager> shaderManager,
                                     const std::shared_ptr<Render::ResourceViewDX11>& displacementTexture, S32 mapSize) {

    terrainMapDisplacementTextureF32 = displacementTexture;
    this->shaderManager = std::move(shaderManager);
    this->mapSize = mapSize;
    Init(std::move(device), std::move(renderer), renderTargetId, windowWidth, windowHeight);
    return true;
}

bool KGV::Engine::TerrainScene::LoadTextures() {

    if (auto device = graphicsDevice.lock()) {
        /**
         * Rock Texture
         */
        Engine::TargaImage rockTextureImage;
        Engine::LoadTargaFromFile("../assets/textures/rock_1/rocks_COLOR.tga", rockTextureImage);

        Render::Texture2dConfigDX11 rockTextureConfig;
        rockTextureConfig.setColorTexture(rockTextureImage.getWidth(), rockTextureImage.getHeight());
        rockTextureConfig.setFormat(DXGI_FORMAT_B8G8R8A8_UNORM);
        rockTextureConfig.setUsage(D3D11_USAGE_IMMUTABLE); // We don't intend to update this.

        D3D11_TEX2D_SRV srvDesc;
        srvDesc.MipLevels = 1;
        srvDesc.MostDetailedMip = 0;

        Render::ShaderResourceViewConfigDX11 rockTextureSrvConfig{};
        rockTextureSrvConfig.setTexture2D(srvDesc);

        ResourceData rockTexResourceData{};
        rockTexResourceData.Data = rockTextureImage.getData();
        rockTexResourceData.memPitch = sizeof(unsigned int) * rockTextureImage.getWidth();

        std::unique_ptr<unsigned int[]> rockImageBuffer;
        if (rockTextureImage.getBitDepth() < 32) {
            unsigned int numPixels = rockTextureImage.getWidth() * rockTextureImage.getHeight();
            rockImageBuffer = std::make_unique<unsigned int[]>(numPixels);
            Engine::Convert24BitTo32Bit(rockTextureImage.getData(), numPixels, rockImageBuffer.get());
            rockTexResourceData.Data = rockImageBuffer.get();
        }

        rockTexture = device->CreateTexture2D(rockTextureConfig, &rockTexResourceData, &rockTextureSrvConfig);

        /**
         * Sand Texture
         */
        Engine::TargaImage sandTextureImage;
        Engine::LoadTargaFromFile("../assets/textures/sand_3/sand_3_1.tga", sandTextureImage);

        Render::Texture2dConfigDX11 sandTextureConfig;
        sandTextureConfig.setColorTexture(sandTextureImage.getWidth(), sandTextureImage.getHeight());
        sandTextureConfig.setFormat(DXGI_FORMAT_B8G8R8A8_UNORM);
        sandTextureConfig.setUsage(D3D11_USAGE_IMMUTABLE); // We don't intend to update this.

        ResourceData sandTexResourceData{};
        sandTexResourceData.Data = sandTextureImage.getData();
        sandTexResourceData.memPitch = sizeof(unsigned int) * sandTextureImage.getWidth();

        std::unique_ptr<unsigned int[]> sandImageBuffer;
        if (sandTextureImage.getBitDepth() < 32) {
            unsigned int numPixels = sandTextureImage.getWidth() * sandTextureImage.getHeight();
            sandImageBuffer = std::make_unique<unsigned int[]>(numPixels);
            Engine::Convert24BitTo32Bit(sandTextureImage.getData(), numPixels, sandImageBuffer.get());
            sandTexResourceData.Data = sandImageBuffer.get();
        }

        sandTexture = device->CreateTexture2D(sandTextureConfig, &sandTexResourceData, &rockTextureSrvConfig);

        /**
         * Grass Texture
         */
        Engine::TargaImage grassTextureImage;
        Engine::LoadTargaFromFile("../assets/textures/ground_veg_19/ground_veg_19.tga", grassTextureImage);

        Render::Texture2dConfigDX11 grassTextureConfig;
        grassTextureConfig.setColorTexture(grassTextureImage.getWidth(), grassTextureImage.getHeight());
        grassTextureConfig.setFormat(DXGI_FORMAT_B8G8R8A8_UNORM);
        grassTextureConfig.setUsage(D3D11_USAGE_IMMUTABLE); // We don't intend to update this.

        ResourceData grassTexResourceData{};
        grassTexResourceData.Data = grassTextureImage.getData();
        grassTexResourceData.memPitch = sizeof(unsigned int) * grassTextureImage.getWidth();

        std::unique_ptr<unsigned int[]> grassImageBuffer;
        if (grassTextureImage.getBitDepth() < 32) {
            unsigned int numPixels = grassTextureImage.getWidth() * grassTextureImage.getHeight();
            grassImageBuffer = std::make_unique<unsigned int[]>(numPixels);
            Engine::Convert24BitTo32Bit(grassTextureImage.getData(), numPixels, grassImageBuffer.get());
            grassTexResourceData.Data = grassImageBuffer.get();
        }

        grassTexture = device->CreateTexture2D(grassTextureConfig, &grassTexResourceData, &rockTextureSrvConfig);

        /**
        * snow Texture
        */
        Engine::TargaImage snowTextureImage;
        Engine::LoadTargaFromFile("../assets/textures/snow_2.tga", snowTextureImage);

        Render::Texture2dConfigDX11 snowTextureConfig;
        snowTextureConfig.setColorTexture(snowTextureImage.getWidth(), snowTextureImage.getHeight());
        snowTextureConfig.setFormat(DXGI_FORMAT_B8G8R8A8_UNORM);
        snowTextureConfig.setUsage(D3D11_USAGE_IMMUTABLE); // We don't intend to update this.

        ResourceData snowTexResourceData{};
        snowTexResourceData.Data = snowTextureImage.getData();
        snowTexResourceData.memPitch = sizeof(unsigned int) * snowTextureImage.getWidth();

        std::unique_ptr<unsigned int[]> snowImageBuffer;
        if (snowTextureImage.getBitDepth() < 32) {
            unsigned int numPixels = snowTextureImage.getWidth() * snowTextureImage.getHeight();
            snowImageBuffer = std::make_unique<unsigned int[]>(numPixels);
            Engine::Convert24BitTo32Bit(snowTextureImage.getData(), numPixels, snowImageBuffer.get());
            snowTexResourceData.Data = snowImageBuffer.get();
        }

        snowTexture = device->CreateTexture2D(snowTextureConfig, &snowTexResourceData, &rockTextureSrvConfig);

        D3D11_SAMPLER_DESC samplerDesc{};
        samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
        samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
        samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
        samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
        samplerDesc.MaxAnisotropy = 16;

        terrainWrapSampler = device->createSamplerState(samplerDesc);
        return true;
    }

    return false;
}

bool KGV::Engine::TerrainScene::SetupPrimaryCamera(S32 width, S32 height, S32 renderTargetId) {
    if (auto device = graphicsDevice.lock()) {
        Render::Texture2dConfigDX11 texConfig;
        texConfig.setDepthTexture(width, height);
        texConfig.setFormat(DXGI_FORMAT_D32_FLOAT);

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

        auto depthStencilState = device->createDepthStencilState(dsDesc);
        if (depthStencilState < 0)
            return false;

        D3D11_VIEWPORT viewport;
        viewport.Width = static_cast<F32>(width);
        viewport.Height = static_cast<F32>(height),
        viewport.MinDepth = 0.0f;
        viewport.MaxDepth = 1.0f;
        viewport.TopLeftX = 0.0f;
        viewport.TopLeftY = 0.0f;

        auto viewPortId = device->createViewPort(viewport);
        if (viewPortId < 0)
            return false;

        auto camera = std::make_shared<Engine::Entity>();
        camera->camera = std::make_unique<Engine::Camera>();
        camera->camera->setDsvStateId(depthStencilState);
        camera->camera->setDsvId(depthBuffer->getDsvId());
        camera->camera->setRtvId(renderTargetId);
        camera->camera->setViewPortId(viewPortId);
        camera->camera->setPerspectiveProject(XMConvertToRadians(70), static_cast<F32>(width) / static_cast<F32>(height), 1, 2000.0f);
        camera->transform.position.z = -600.0f;
        camera->transform.position.x = 0;
        camera->transform.position.y = 500;
        camera->transform.rotation.x = XMConvertToRadians(45);

        cameras.emplace_back(camera);
        return true;
    }
    return false;
}

void KGV::Engine::TerrainScene::SetupEntities() {
    if (auto renderer = graphicsRenderer.lock()) {
        auto light = std::make_shared<Engine::Entity>();
        light->transform.rotation.x = XMConvertToRadians(60);
        light->transform.rotation.y = XMConvertToRadians(0);
        light->light = std::make_unique<Engine::LightComponent>();
        light->light->ambient = {0.4f, 0.4f, 0.4f, 1.0f};
        light->light->diffuse = {1.0f, 1.0f, 1.0f, 1.0f};
        light->light->specular = {0.1f, 0.1f, 0.1f, 1.0f};

        lights.emplace_back(light);

        std::vector<U32> gridIndices;
        std::vector<Render::Vertex> gridVertices;
        auto lt = std::chrono::high_resolution_clock::now();

        Engine::GeometryFactory::getVertexGridU32(mapSize, gridVertices, gridIndices, 1.0f);
        auto ct = std::chrono::high_resolution_clock::now();
        spdlog::info("Time to generate grid of {} vertices, {} triangles: {}s", gridVertices.size(), gridIndices.size() / 3,
                     std::chrono::duration_cast<std::chrono::duration<double>>(ct - lt).count());

        auto gridMeshId = renderer->createMesh({gridVertices}, gridIndices, Render::eBufferUpdateType::kImmutable);

        auto displacementVertexShader = shaderManager->getShaderByName( "displacementShaderVs", Render::eShaderType::kVertex);
        auto gradTexturePixelShader = shaderManager->getShaderByName( "gradientTextureShaderPs", Render::eShaderType::kPixel);
        auto basicTransformVertexShader = shaderManager->getShaderByName( "basicShaderVs", Render::eShaderType::kVertex);
        auto basicLitPixelShader = shaderManager->getShaderByName( "basicLitShaderPs", Render::eShaderType::kPixel);

        auto inputLayoutId = shaderManager->getInputLayoutForShader(displacementVertexShader); // all of the following shaders use the same layout.

        auto grid = std::make_shared<Engine::Entity>();
        grid->transform.position.y = 0;
        grid->transform.rotation.y = 0;
        grid->material = std::make_unique<Engine::MaterialComponent>();
        grid->material->displacement = 256.0f;
        grid->material->ambient = {0.4f, 0.4f, 0.4f, 1.0f};
        grid->material->diffuse = {1.0f, 1.0f, 1.0f, 1.0f};
        grid->material->specular = {0.5f, 0.5f, 0.5f, 0.001 * 128};
        grid->material->materialId = renderer->createMaterial(inputLayoutId, displacementVertexShader, gradTexturePixelShader);
        grid->material->mapTexture = terrainMapDisplacementTextureF32;
        grid->material->colorTextures.emplace_back(rockTexture);
        grid->material->colorTextures.emplace_back(grassTexture);
        grid->material->colorTextures.emplace_back(sandTexture);
        grid->material->colorTextures.emplace_back(snowTexture);
        grid->material->samplerIds.emplace_back(terrainWrapSampler);

        grid->mesh = std::make_unique<Engine::MeshComponent>();
        grid->mesh->meshId = gridMeshId;
        grid->mesh->render = true;
        entities.emplace_back(grid);

        auto water = std::make_shared<Engine::Entity>();
        water->transform.position.y = 0;
        water->material = std::make_unique<Engine::MaterialComponent>();
        water->material->ambient = {0, 0, 1, 1.0f};
        water->material->diffuse = {0, 0, 1.0f, 1.0f};
        water->material->specular = {0, 0, 1.0f, 0.6 * 128};
        water->material->materialId = renderer->createMaterial(inputLayoutId, basicTransformVertexShader, basicLitPixelShader);

        water->mesh = std::make_unique<Engine::MeshComponent>();
        water->mesh->meshId = gridMeshId;
        water->mesh->render = true;
        waterPlane = water;
        entities.emplace_back(water);
    }

}