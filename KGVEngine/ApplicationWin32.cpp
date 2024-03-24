#include <random>
#include "ApplicationWin32.h"
#include "GeometryFactory.h"
#include "NoiseBufferGenerator.h"
#include "TargaImage.h"
#include "Scenes/TerrainScene.h"
#include "Scenes/HeightMapScene.h"

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

    createHeightMapBuffers(mapSize);
    generateHeightMaps(mapSize, static_cast<S32>(mapSeed), mapScale);

    Render::SwapChainConfigDX11 swapChainConf;
    swapChainConf.setWidth(window1->getWidth());
    swapChainConf.setHeight(window1->getHeight());
    swapChainId = device->createSwapChain(window1->getWin32Handle(), swapChainConf);
    auto swapChain = device->getSwapChainById(swapChainId);
    rtvId = swapChain->getResource()->getRtvId();

    terrainScene = std::make_unique<Engine::TerrainScene>();
    heightMapScene = std::make_unique<Engine::HeightMapScene>();

    {
        auto t = dynamic_cast<Engine::TerrainScene*>(terrainScene.get());
        t->Init(device, renderer, rtvId, window1->getWidth(),
                window1->getHeight(), shaderManager, terrainMapDisplacementTextureF32, mapSize);

        auto h = dynamic_cast<Engine::HeightMapScene*>(heightMapScene.get());
        h->Init(device, renderer, rtvId, window1->getWidth(),
                window1->getHeight(), shaderManager, terrainMapTextureFinalRGBA);
    }

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

//            if (wParam == 0x5a)  { // Z key
//                cameras[0]->camera->setIsWireframe(!cameras[0]->camera->isWireframe());
//            }

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

    ImGui::SeparatorText("Select Scene");
    if (ImGui::RadioButton("Terrain Scene", selectedScene == 0)) {
        selectedScene = 0;
    } else if (ImGui::RadioButton("Heightmap Scene", selectedScene == 1)) {
        selectedScene = 1;
    } else if (ImGui::RadioButton("Wave Scene", selectedScene == 2)) {
        selectedScene = 2;
    }

    bool regenMap = false;
    if (selectedScene == 0 || selectedScene == 1) {
        // fBm Settings
        ImGui::SeparatorText("Fractal Brownian Motion");
        if (ImGui::SliderInt("Octaves", &terrainOctaves, 1, 16)) {
            regenMap = true;
        }
        if (ImGui::SliderFloat("Amplitude", &terrainAmplitude, 0.01f, 8.0f)) {
            regenMap = true;
        }
        if (ImGui::SliderFloat("Frequency", &terrainFrequency, 0.01f, 8.0f)) {
            regenMap = true;
        }
        if (ImGui::SliderFloat("Gain/Persistence", &terrainPersistence, 0.01f, 4.0f)) {
            regenMap = true;
        }
        if (ImGui::SliderFloat("Lacunarity", &terrainLacunarity, 0.01f, 4.0f)) {
            regenMap = true;
        }

        // Sharpness Slider
        ImGui::SeparatorText("Terrain Sharpness");
        ImGui::Text("Rough");
        ImGui::SameLine();
        if (ImGui::SliderFloat("##sharpeness", &terrainSharpness, -1.0f, 1.0f)) {
            regenMap = true;
        }
        ImGui::SameLine();
        ImGui::Text("Smooth");

        // Sharpness Enhance
        ImGui::Text("Sharpness Enhance");
        ImGui::Text("Rough");
        ImGui::SameLine();
        if (ImGui::SliderInt("##sharpenessEnhance", &terrainSharpnessEnhance, 1, 16)) {
            regenMap = true;
        }
        ImGui::SameLine();
        ImGui::Text("Smooth");


    }

    deviceContext->clearColorBuffers({0.1f, 0.1f, 0.1f, 1.0f});
    deviceContext->clearDepthStencilBuffers();

    switch (selectedScene) {
        case 0:
            terrainScene->Tick(deltaTime);
            break;
        case 1:
            heightMapScene->Tick(deltaTime);
        default:
            break;
    }

    ImGui::End();

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

    if (regenMap) {
        generateHeightMaps(mapSize, mapSeed, mapScale, 0, 0);
        regenMap = false;
    }

}

void KGV::System::ApplicationWin32::generateHeightMaps(int _textureSize, int _seed, double _scale, double xOffset, double yOffset) {
//    auto lagrangianScale = [](double lowScale, double highScale, double lowRaw, double highRaw, double value) {
//        return (highScale - 0)*((value - lowRaw)/(highRaw - lowRaw)) + lowScale;
//    };

    Procedural::NoiseBufferGenerator nbg;
    Procedural::PerlinNoise perlinNoise;
    Procedural::fBmConfig conf{};
    conf.octaves = terrainOctaves;
    conf.persistence = terrainPersistence;
    conf.amplitude = terrainAmplitude;
    conf.frequency = terrainFrequency;
    conf.lacunarity = terrainLacunarity;

    spdlog::info("octaves {}, persistence {}, amplitude {}, frequency {}, lacunarity {}",
                                conf.octaves, conf.persistence, conf.amplitude, conf.frequency, conf.lacunarity);

    std::vector<float> finalNoiseBuffer;
    finalNoiseBuffer.resize(_textureSize * _textureSize);

    std::vector<XMFLOAT2> octaveOffsets;
    std::mt19937 mt(_seed);

    auto lt = std::chrono::high_resolution_clock::now();
    std::uniform_real_distribution<> distrib(-50,50);
    for (int i = 0; i < conf.octaves; ++i) {
        octaveOffsets.emplace_back(distrib(mt) + xOffset, distrib(mt) + yOffset);
    }

    const auto sharpness = terrainSharpness;
    const auto sharpnessEnhance = terrainSharpnessEnhance;

//    _scale = 1;
    _scale /= 2;
    Procedural::NoiseOp sharpnessFilter = [sharpness, sharpnessEnhance, &conf, &perlinNoise, _scale, octaveOffsets]
            (double a, double xf, double yf, int width, int height) -> double {

        auto lerp = [](double a, double b, double t) -> double {
            return a + t * (b - a);
        };

        auto freq = conf.frequency;
        auto amplitude = conf.amplitude;
        double sum = 0;
        double min = 0;
        double max = 0;
        for (int i = 0; i < conf.octaves; ++i) {
            auto x = (xf * freq + octaveOffsets[i].x ) / _scale;
            auto y = (yf * freq + octaveOffsets[i].y) / _scale;
            auto noise = perlinNoise.noiseDP(x, y) * amplitude;

            auto billowNoise = std::abs(noise);
            auto ridgeNoise = 1 - billowNoise;

            auto billowLerped = lerp(noise, billowNoise, abs(sharpness));
            noise = lerp(billowLerped, ridgeNoise, -sharpness);
            noise = noise * std::pow(std::abs(noise), sharpnessEnhance);

            sum += noise;
            min -= amplitude;
            max += amplitude;
            freq *= conf.lacunarity;
            amplitude *= conf.persistence;
        }

        return sum / max;
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

    D3D11_BOX box;
    box.left = 0;
    box.right = _textureSize;
    box.top = 0;
    box.bottom = _textureSize;
    box.front = 0;
    box.back = 1; // Assuming 2D texture

    int displacementRowPitch = _textureSize * sizeof(float);
    deviceContext->updateSubresource(terrainMapDisplacementTextureF32.get(), 0, &box, finalNoiseBuffer.data(), displacementRowPitch, _textureSize * displacementRowPitch);

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

void KGV::System::ApplicationWin32::DrawGUI() {
    ImGui::Begin("Engine");

}

