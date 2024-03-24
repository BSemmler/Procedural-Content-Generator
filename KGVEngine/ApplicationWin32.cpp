#include <random>
#include "ApplicationWin32.h"
#include "GeometryFactory.h"
#include "NoiseBufferGenerator.h"
#include "TargaImage.h"
#include "Scenes/TerrainScene.h"

using namespace DirectX;

bool KGV::System::ApplicationWin32::init() {
    for (auto &key : keyDown)
        key = false;

    WNDCLASSEX wc;
    memset( &wc, 0, sizeof( WNDCLASSEX ) );
    wc.cbSize = sizeof( WNDCLASSEX );
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.cbWndExtra = sizeof( *this );
    wc.cbClsExtra = 0;
    wc.hInstance = nullptr;
    wc.hIcon = LoadIcon( nullptr, IDI_APPLICATION );
    wc.hCursor = LoadCursor( nullptr, IDC_ARROW );
    wc.hbrBackground = ( HBRUSH ) ( GetStockObject( WHITE_BRUSH ) );
    wc.lpszMenuName = nullptr;
    wc.lpszClassName = "KGV_Win32";
    wc.hIconSm = LoadIcon( nullptr, IDI_APPLICATION );
    
    WindowWin32::registerClass( wc );
    window1 = std::make_unique<WindowWin32>( this, "KGV_Win32", 1600, 900, 0, 0, "Hello World 1!", true );

    if ( !window1->getWin32Handle() ) {
        return false;
    }

    window1->showWindow( true );
    auto log = spdlog::get("render");
    device = std::move(std::make_unique<Render::RenderDeviceDX11>(log));
    device->init();
    deviceContext = device->getPipelineManager();

    renderer = std::make_shared<Render::SimpleRenderer>(device.get(), deviceContext.get());
    spdlog::get("engine")->info("Initialization complete!");

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
//    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // IF using Docking Branch
    io.MouseDrawCursor = true;

// Setup Platform/Renderer backends
    ImGui_ImplWin32_Init(window1->getWin32Handle());
    ImGui_ImplDX11_Init(device->getID3D11Device().Get(), device->getID3D11DeviceContext1().Get());

    shaderManager = std::make_shared<Engine::ShaderManager>(device);
    std::string basicVsStr("../shaders/basicTransformVertexShader.hlsl");
    auto basicVs = shaderManager->loadShader("../shaders/basicTransformVertexShader.hlsl",  "main",
                              "basicShaderVs", Render::eShaderType::kVertex,  "vs_5_0");

    auto displacementVs = shaderManager->loadShader("../shaders/displacementVertexShader.hlsl",  "main",
                               "displacementShaderVs", Render::eShaderType::kVertex,  "vs_5_0");

    shaderManager->loadShader( "../shaders/gradientPixelShader.hlsl",  "main",
                               "gradientTextureShaderPs", Render::eShaderType::kPixel,  "ps_5_0");

    shaderManager->loadShader( "../shaders/basicLightingPixelShader.hlsl",  "main",
                               "basicLitShaderPs", Render::eShaderType::kPixel,  "ps_5_0");

    shaderManager->loadShader( "../shaders/unlitTexturedPixelShader.hlsl",  "main",
                               "unlitShaderPs", Render::eShaderType::kPixel,  "ps_5_0");

    std::vector<D3D11_INPUT_ELEMENT_DESC> inputElements = {
            {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
            {"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
            {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
    };

    auto layoutId = shaderManager->createInputLayout(inputElements, basicVs);
    if (layoutId < 0)
        return false;

    shaderManager->setInputLayoutForShader(displacementVs, layoutId);


//    basicShaderVs = device->loadShader("../shaders/basicTransformVertexShader.hlsl", Render::eShaderType::kVertex, false, "VS", "vs_5_0");
//    displacementShaderVs = device->loadShader("../shaders/displacementVertexShader.hlsl", Render::eShaderType::kVertex, false, "VS", "vs_5_0");
//    textureGradientShaderPs = device->loadShader("../shaders/gradientPixelShader.hlsl", Render::eShaderType::kPixel, false, "PS", "ps_5_0");
//    basicLightingShaderPs = device->loadShader("../shaders/basicLightingPixelShader.hlsl", Render::eShaderType::kPixel, false, "PS", "ps_5_0");
//    unlitTexturedShaderPs = device->loadShader("../shaders/unlitTexturedPixelShader.hlsl", Render::eShaderType::kPixel, false, "PS", "ps_5_0");

    createHeightMapBuffers(mapSize);
    generateHeightMaps(mapSize, static_cast<S32>(mapSeed), mapScale);

    float viewPortHeightRatio = 0.25f;
    float viewPortWidthRatio = 1.0f;

    Render::SwapChainConfigDX11 swapChainConf;
    swapChainConf.setWidth(window1->getWidth());
    swapChainConf.setHeight(window1->getHeight());
    swapChainId = device->createSwapChain(window1->getWin32Handle(), swapChainConf);
    auto swapChain = device->getSwapChainById(swapChainId);
    rtvId = swapChain->getResource()->getRtvId();

    terrainScene = std::make_unique<Engine::TerrainScene>();

    {
        auto t = dynamic_cast<Engine::TerrainScene*>(terrainScene.get());
        t->Init(device, renderer, rtvId, window1->getWidth(),
                window1->getHeight(), shaderManager, terrainMapDisplacementTextureF32, mapSize);
    }




//    setupPrimaryCamera(window1->getWidth(), window1->getHeight(), 0, 0);
//    setupTextureViewer(window1->getWidth() * viewPortWidthRatio, window1->getWidth() * viewPortHeightRatio, 0, 0);

    return true;
}

void KGV::System::ApplicationWin32::shutdown()
{
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
}

int KGV::System::ApplicationWin32::run() {
    using namespace std::chrono;
    MSG msg;
    memset(&msg, 0, sizeof(MSG));

    lastTime = high_resolution_clock::now();
    while(msg.message != WM_QUIT) {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);

            if (msg.message == WM_QUIT)
                break;
        }

        auto currentTime = high_resolution_clock::now();
        float dt = duration_cast<duration<float>>(currentTime - lastTime).count();
        lastTime = currentTime;
        draw(dt);
    }

    return ( int ) msg.wParam;
}

LRESULT KGV::System::ApplicationWin32::wndProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam ) {
    switch ( msg ) 	
    {
        case WM_LBUTTONDOWN:
            break;
        case WM_KEYDOWN:
            keyDown[wParam] = true;
            if ( wParam == VK_ESCAPE ) {
                DestroyWindow( hWnd );
                spdlog::get("engine")->info("Escape Key Pressed, exiting.", wParam);
            }

            if (wParam == 0x5a)  { // Z key
                cameras[0]->camera->setIsWireframe(!cameras[0]->camera->isWireframe());
            }

            spdlog::get("engine")->info("Keydown, Keycode: {}", wParam);
            break;
        case WM_KEYUP:
            keyDown[wParam] = false;
            break;
        case WM_DESTROY:
            DestroyWindow( hWnd );
            if ( window1 )
                PostQuitMessage( 0 );
//            else if ( window1->getWin32Handle() == hWnd )
//                window1.release();
            return 0;
        default:
            break;
    }

    extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    return DefWindowProc( hWnd, msg, wParam, lParam );
}

int gAvgFps = 0;
float gAvgFrameTime = 0;
float cumulativeTime = 0;

void KGV::System::ApplicationWin32::draw(F32 deltaTime) {
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    ImGui::ShowDemoWindow();
    ImGui::Begin("Engine");


    ImGui::Text("Select Scene");
    if (ImGui::RadioButton("Terrain Scene", selectedScene == 0)) {
        selectedScene = 0;
    } else if (ImGui::RadioButton("Heightmap Scene", selectedScene == 1)) {
        selectedScene = 1;
    } else if (ImGui::RadioButton("Wave Scene", selectedScene == 2)) {
        selectedScene = 2;
    }

//    ImGui::
//    ImGui::

    ImGui::End();

    deviceContext->clearColorBuffers({0.1f, 0.1f, 0.1f, 1.0f});
    deviceContext->clearDepthStencilBuffers();

    switch (selectedScene) {
        case 0:
            terrainScene->Tick(deltaTime);
            break;
        default:
            break;
    }
//    terrainScene->Tick(deltaTime);
//    renderer->renderScene(entities, cameras, &lights, deltaTime);

    std::vector tC = {texturePlaneCamera};
//    renderer->renderScene(texturePlanes, tC, &lights, deltaTime);


    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

    device->presentSwapChain(swapChainId, 1, 0);
    gAvgFps = static_cast<S32>(1.0f / deltaTime + static_cast<F32>(gAvgFps)) / 2;
    gAvgFrameTime = (deltaTime + gAvgFrameTime) / 2;
    cumulativeTime += deltaTime;

    if (cumulativeTime > 0.25f) {
        window1->setWindowCaption(fmt::format("Frame rate: {:d}, Frame time: {:.3f}", gAvgFps, gAvgFrameTime));
        cumulativeTime = 0.0f;
    }

    bool regenMap = false;
    if (keyDown[VK_UP] && seedChangeKeyDebounce > 0.3) {
        mapSeed++;
        seedChangeKeyDebounce = 0;
        regenMap = true;
    } else if (keyDown[VK_DOWN] && seedChangeKeyDebounce > 0.3) {
        mapSeed--;
        seedChangeKeyDebounce = 0;
        regenMap = true;
    }

    static double xOffset = 0.0;
    static double yOffset = 0.0;
    if (keyDown[0x55]) {
        yOffset += 30 * deltaTime;
        regenMap = true;
    } else if (keyDown[0x4A]) {
        yOffset -= 30 * deltaTime;
        regenMap = true;
    }

    if (keyDown[0x48]) {
        xOffset += 30 * deltaTime;
        regenMap = true;
    } else if (keyDown[0x4B]) {
        xOffset -= 30 * deltaTime;
        regenMap = true;
    }

    // Q key
    static bool waterKeyPressed = false;
    if (keyDown[0x51] && !waterKeyPressed) {
        waterPlane->mesh->render = !waterPlane->mesh->render;
        waterKeyDebounceTime = 0;
        waterKeyPressed = true;
    } else if (!keyDown[0x51]) {
        waterKeyPressed = false;
    }

    if (keyDown[VK_RIGHT]) {
        mapScale += 10 * deltaTime;
        regenMap = true;
    } else if (keyDown[VK_LEFT]) {
        mapScale -= 10 * deltaTime;
        regenMap = true;
    }

    if (regenMap) {
        generateHeightMaps(mapSize, mapSeed, mapScale, xOffset, yOffset);
        regenMap = false;
    }

    seedChangeKeyDebounce += deltaTime;
    waterKeyDebounceTime += deltaTime;
}

void KGV::System::ApplicationWin32::generateHeightMaps(int _textureSize, int _seed, double _scale, double xOffset, double yOffset) {
    auto lagrangianScale = [](double lowScale, double highScale, double lowRaw, double highRaw, double value) {
        return (highScale - 0)*((value - lowRaw)/(highRaw - lowRaw)) + lowScale;
    };

    Procedural::NoiseBufferGenerator nbg;
    Procedural::PerlinNoise perlinNoise;
    Procedural::fBmConfig conf{};
    conf.octaves = 8;
    conf.persistence = 0.45;
    conf.amplitude = 1;
    conf.frequency = 1;
    conf.lacunarity = 2;


    const double sharpness = -1;
    const double sharpnessEnhance = 2;

    auto lerp = [](double a, double b, double t) -> double {
        return a + t * (b - a);
    };

    std::vector<float> finalNoiseBuffer;
    finalNoiseBuffer.resize(_textureSize * _textureSize);

    std::vector<XMFLOAT2> octaveOffsets;
    std::mt19937 mt(_seed);

    auto lt = std::chrono::high_resolution_clock::now();
    std::uniform_real_distribution<> distrib(-50,50);
    for (int i = 0; i < conf.octaves; ++i) {
        octaveOffsets.emplace_back(distrib(mt) + xOffset, distrib(mt) + yOffset);
    }


//    _scale = 1;
    _scale /= 2;
    Procedural::NoiseOp sharpnessFilter = [&lerp, sharpness, sharpnessEnhance, &conf, &perlinNoise, _scale, octaveOffsets]
            (double a, double xf, double yf, int width, int height) -> double {

        auto freq = conf.frequency;
        auto amplitude = conf.amplitude;
        double sum = 0;
        double min = 0;
        double max = 0;
        float halfWidth = static_cast<float>(width) / 2.0f;
        for (int i = 0; i < conf.octaves; ++i) {
            auto x = (xf * freq + octaveOffsets[i].x ) / _scale;
            auto y = (yf * freq + octaveOffsets[i].y) / _scale;
//            auto x = (xf) / _scale * freq + octaveOffsets[i].x;
//            auto y = (yf) / _scale * freq + octaveOffsets[i].y;
            auto noise = perlinNoise.noiseDP(x, y);

            auto billowNoise = std::abs(noise);
            auto ridgeNoise = 1 - billowNoise;
//            noise = noise + (ridgeNoise * billowNoise);

//            noise = std::abs(noise);
//            noise = lagrangianScale(-1, 1, 0, 1, noise);
//            noise = lerp(-1, 1, noise);

            auto billowLerped = lerp(noise, billowNoise, abs(sharpness));
            noise = lerp(billowLerped, ridgeNoise, -sharpness);
//            noise = sharpness >= 0 ? lerp(noise, billowNoise, sharpness) : lerp(noise, ridgeNoise, -sharpness);
            noise = noise * std::pow(std::abs(noise), sharpnessEnhance);

            sum += noise * amplitude;
            min -= amplitude;
            max += amplitude;
            freq *= conf.lacunarity;
            amplitude *= conf.persistence;
        }

        return sum / max;
//        return (sum - min) / (max - min);
    };

    nbg.execOp(finalNoiseBuffer.data(), _textureSize, _textureSize, sharpnessFilter);

    double min = 1000;
    double max = -1000;
    for (auto val : finalNoiseBuffer) {
        if (val < min)
            min = val;

        if (val > max)
            max = val;
    }

    double newMin = 1000;
    double newMax = -1000;
    for (auto &val : finalNoiseBuffer) {
        val = (val - min)/(max - min);

        if (val < newMin)
            newMin = val;

        if (val > newMax)
            newMax = val;
    }

    auto ct = std::chrono::high_resolution_clock::now();
    spdlog::info("Time to generate generate perlin noise for {} points: {}s", finalNoiseBuffer.size(), std::chrono::duration_cast<std::chrono::duration<double>>(ct - lt).count());

    spdlog::info("min: {}, max: {}", min, max);
    spdlog::info("new min: {}, max: {}", newMin, newMax);

    D3D11_BOX box;
    box.left = 0;
    box.right = _textureSize;
    box.top = 0;
    box.bottom = _textureSize;
    box.front = 0;
    box.back = 1; // Assuming 2D texture

    int displacementRowPitch = _textureSize * sizeof(float);
    deviceContext->updateSubresource(terrainMapDisplacementTextureF32.get(), 0, &box, finalNoiseBuffer.data(), displacementRowPitch, _textureSize * displacementRowPitch);


//    double radius = 1000;
//    Procedural::NoiseOp circularGradientOp = [radius](double val, double xf, double yf, int width, int height) -> double {
//        double centerX = width / 2;
//        double centerY = height / 2;
//        double distX = abs(xf - centerX);
//        double distY = abs(yf - centerY);
//
//        double distance = std::sqrt(distX * distX + distY * distY);
//        return distance > radius ? 0 : val;
//    };




    std::vector<float> baseNoiseBuffer;
    baseNoiseBuffer.resize(_textureSize * _textureSize);
//    Procedural::fBmConfig conf{};
//    conf.octaves = 1;
//    conf.persistence = 0.2;
//    conf.amplitude = 1;
//    conf.frequency = 1;
//    conf.lacunarity = 2;
//    nbg.generateNoiseTexture2D(conf, baseNoiseBuffer.data(), gridOffset, gridOffset, textureSize, textureSize);
////    nbg.execOp(baseNoiseBuffer.data(), textureSize, textureSize, billowNoiseOp);

    Procedural::NoiseOp ridgeNoiseOp = [](double val, double xf, double yf, int width, int height) -> double {
        return 1 - abs(val);
    };

    std::vector<float> terrainRidgeNoiseBuffer;
    terrainRidgeNoiseBuffer.resize(_textureSize * _textureSize);
    Procedural::fBmConfig ridgeFBM{};
    ridgeFBM.octaves = 1;
    ridgeFBM.persistence = 0.5;
    ridgeFBM.amplitude = 1;
    ridgeFBM.frequency = 0.001;
    ridgeFBM.lacunarity = 2;
    double ridgeOffset = 0;
//    nbg.generateNoiseTexture2D(ridgeFBM, terrainRidgeNoiseBuffer.data(), ridgeOffset, ridgeOffset, textureSize, textureSize);
//    nbg.execOp(terrainRidgeNoiseBuffer.data(), textureSize, textureSize, ridgeNoiseOp);

    Procedural::NoiseOp billowNoiseOp = [](double val, double xf, double yf, int width, int height) -> double {
        return abs(val);
    };

    std::vector<float> billowNoiseBuffer;
    billowNoiseBuffer.resize(_textureSize * _textureSize);
    Procedural::fBmConfig billowFBM{};
    billowFBM.octaves = 1;
    billowFBM.persistence = 0.5;
    billowFBM.amplitude = 1;
    billowFBM.frequency = 0.001;
    billowFBM.lacunarity = 2;
    double billowOffset = 0;
//    nbg.generateNoiseTexture2D(billowFBM, billowNoiseBuffer.data(), billowOffset, billowOffset , textureSize, textureSize);
//    nbg.execOp(billowNoiseBuffer.data(), textureSize, textureSize, billowNoiseOp);

    // Combine all of the noise layers into a single buffer.
    Procedural::Combine2NoiseOp multiplicativeCombination = []
            (double val1, double val2, double xf, double yf, int width, int height) -> double {
        return val1 * val2;
    };

//    std::vector<float> finalNoiseBuffer;
//    finalNoiseBuffer.resize(textureSize * textureSize);

//    nbg.combine(baseNoiseBuffer.data(), terrainRidgeNoiseBuffer.data(), billowNoiseBuffer.data(), finalNoiseBuffer.data(), textureSize, textureSize, sharpnessFilter);

    double highScale = 255;
    double lowScale = 0;
    double lowRaw = 0;
    double highRaw = 1;
    Procedural::ImageOp imageOp = [highScale, lowScale, highRaw, lowRaw]
            (double val, double xf, double yf, int width, int height) -> unsigned int {
        auto _value = (highScale - 0)*((val - lowRaw)/(highRaw - lowRaw)) + lowScale;
        auto intValue = static_cast<unsigned int>(_value);
        unsigned int retValue = intValue; // Alpha channel to max
        retValue |= intValue << 8;
        retValue |= intValue << 16;
        retValue |= 255 << 24; // Alpha channel to max
        return retValue;
    };


    std::vector<unsigned int> heightMapDisplayBuffer;
    heightMapDisplayBuffer.resize(_textureSize * _textureSize);

    unsigned int heightMapTextureRowPitch = _textureSize * sizeof(unsigned int);
    nbg.createPixelBufferFromData(heightMapDisplayBuffer.data(), finalNoiseBuffer.data(), _textureSize, _textureSize, imageOp);
    deviceContext->updateSubresource(terrainMapTextureFinalRGBA.get(), 0, &box, heightMapDisplayBuffer.data(), heightMapTextureRowPitch, heightMapTextureRowPitch * _textureSize);
}

void KGV::System::ApplicationWin32::createHeightMapBuffers(int textureSize) {
    //Height map
    Render::Texture2dConfigDX11 terrainMapTexConfig;
    terrainMapTexConfig.setColorTexture(textureSize, textureSize);
    terrainMapTexConfig.setFormat(DXGI_FORMAT_R32_FLOAT);

    Render::ShaderResourceViewConfigDX11 terrainMapSrvConfig{};

    D3D11_TEX2D_SRV srvDesc;
    srvDesc.MipLevels = 1;
    srvDesc.MostDetailedMip = 0;

    terrainMapSrvConfig.setTexture2D(srvDesc);
    terrainMapDisplacementTextureF32 = device->CreateTexture2D(terrainMapTexConfig, nullptr, &terrainMapSrvConfig);

    // Heightmap Display
    Render::Texture2dConfigDX11 terrainMapDisplayTexConfig;
    terrainMapDisplayTexConfig.setColorTexture(textureSize, textureSize);
    terrainMapDisplayTexConfig.setFormat(DXGI_FORMAT_R8G8B8A8_UNORM);



    Render::ShaderResourceViewConfigDX11 terrainMapDisplaySrvConfig{};
    terrainMapDisplaySrvConfig.setTexture2D(srvDesc);
    terrainMapTextureFinalRGBA = device->CreateTexture2D(terrainMapDisplayTexConfig, nullptr, &terrainMapDisplaySrvConfig);
}

void KGV::System::ApplicationWin32::setupPrimaryCamera(int width, int height, int topX, int topY) {
    Render::SwapChainConfigDX11 swapChainConf;
    swapChainConf.setWidth(window1->getWidth());
    swapChainConf.setHeight(window1->getHeight());
    swapChainId = device->createSwapChain(window1->getWin32Handle(), swapChainConf);
    auto swapChain = device->getSwapChainById(swapChainId);

//    spdlog::get("engine")->info("RTV ID: {}", swapChain->getResource()->getRtvId());
//    rtvId = device->createRenderTargetView(swapChain->getResource()->getResourceId(), nullptr);
    rtvId = swapChain->getResource()->getRtvId();
    Render::Texture2dConfigDX11 texConfig;
    texConfig.setDepthTexture(window1->getWidth(), window1->getHeight());
    texConfig.setFormat(DXGI_FORMAT_D32_FLOAT);

    Render::DepthStencilViewConfigDX11 dsvConfig;
    dsvConfig.setFormat(DXGI_FORMAT_D32_FLOAT);
    D3D11_TEX2D_DSV dsvState;
    dsvState.MipSlice = 0;
    dsvConfig.SetTexture2D(dsvState);
    depthBuffer = device->CreateTexture2D(texConfig, nullptr, nullptr, nullptr, &dsvConfig);

    D3D11_VIEWPORT viewport;
    viewport.Width = width;
    viewport.Height = height,
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;
    viewport.TopLeftX = topX;
    viewport.TopLeftY = topY;

    viewPortId = device->createViewPort(viewport);

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

    auto camera = std::make_shared<Engine::Entity>();
    camera->camera = std::make_unique<Engine::Camera>();
    camera->camera->setDsvStateId(device->createDepthStencilState(dsDesc));
    camera->camera->setDsvId(depthBuffer->getDsvId());
    camera->camera->setRtvId(rtvId);
    camera->camera->setViewPortId(viewPortId);
    camera->camera->setPerspectiveProject(XMConvertToRadians(70), (float)width / (float)height, 1, 2000.0f);
    camera->transform.position.z = -600.0f;
    camera->transform.position.x = 0;
    camera->transform.position.y = 500;
    camera->transform.rotation.x = XMConvertToRadians(45);

    cameras.emplace_back(camera);
}

void KGV::System::ApplicationWin32::setupTextureViewer(int width, int height, int topX, int topY) {
    std::vector<Render::Vertex> planeVertices;
    std::vector<U32> planeIndices;
    Engine::GeometryFactory::getPlane(planeVertices, planeIndices);

    // TODO: Need to fix material change detection to include textures so that single material pipeline definition can be reused.
    auto planeMeshId = renderer->createMesh({planeVertices}, planeIndices, Render::kImmutable);

    auto textureViewingPlane = std::make_shared<Engine::Entity>();
    textureViewingPlane->mesh = std::make_unique<Engine::MeshComponent>();
    textureViewingPlane->mesh->meshId = planeMeshId;
    textureViewingPlane->material = std::make_unique<Engine::MaterialComponent>();
//    textureViewingPlane->material->materialId = renderer->createMaterial(inputLayoutId, planeVertexShaderId, unlitTexturedShaderPs);
    textureViewingPlane->mesh->render = false;
//    textureViewingPlane->material->colorTextures.emplace_back(terrainMapTextureFinalRGBA);
    textureViewingPlane->transform.position.x = -1.5;
    textureViewingPlane->transform.rotation.z = 180;

    texturePlaneEntity = textureViewingPlane;
    texturePlanes.emplace_back(textureViewingPlane);

    D3D11_VIEWPORT vp;
    vp.MinDepth = 0;
    vp.MaxDepth = 1;
    vp.TopLeftX = topX;
    vp.TopLeftY = topY;
    vp.Width = width;
    vp.Height = height;

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

    auto heightMapCamera = std::make_shared<Engine::Entity>();
    heightMapCamera->camera = std::make_unique<Engine::Camera>();
    heightMapCamera->camera->setDsvStateId(device->createDepthStencilState(dsDesc));
    heightMapCamera->camera->setDsvId(depthBuffer->getDsvId());
    heightMapCamera->camera->setRtvId(rtvId);
    heightMapCamera->camera->setViewPortId(device->createViewPort(vp));
    heightMapCamera->camera->setOrthographicProject(texturePlanes.size(), 1, 0.01, 5);
    heightMapCamera->camera->setIsActive(true);
    heightMapCamera->transform.position.z = -1;
    texturePlaneCamera = heightMapCamera;
}

void KGV::System::ApplicationWin32::DrawGUI() {
    ImGui::Begin("Engine");

}

