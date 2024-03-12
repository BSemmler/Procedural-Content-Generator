#include "ApplicationWin32.h"
using namespace DirectX;

std::vector<KGV::Render::Vertex> gVertices = {
    { XMFLOAT4A(-1.0f, -1.0f, -1.0f, 1.0f), XMFLOAT3A(), XMFLOAT2A() },
    { XMFLOAT4A(-1.0f, +1.0f, -1.0f, 1.0f), XMFLOAT3A(), XMFLOAT2A() },
    { XMFLOAT4A(+1.0f, +1.0f, -1.0f, 1.0f), XMFLOAT3A(), XMFLOAT2A() },
    { XMFLOAT4A(+1.0f, -1.0f, -1.0f, 1.0f), XMFLOAT3A(), XMFLOAT2A() },
    { XMFLOAT4A(-1.0f, -1.0f, +1.0f, 1.0f), XMFLOAT3A(), XMFLOAT2A() },
    { XMFLOAT4A(-1.0f, +1.0f, +1.0f, 1.0f), XMFLOAT3A(), XMFLOAT2A() },
    { XMFLOAT4A(+1.0f, +1.0f, +1.0f, 1.0f), XMFLOAT3A(), XMFLOAT2A() },
    { XMFLOAT4A(+1.0f, -1.0f, +1.0f, 1.0f), XMFLOAT3A(), XMFLOAT2A() }
};

std::vector<U32> gIndices = {
// front face
        0, 1, 2,
        0, 2, 3,
// back face
        4, 6, 5,
        4, 7, 6,
// left face
        4, 5, 1,
        4, 1, 0,
// right face
        3, 2, 6,
        3, 6, 7,
// top face
        1, 5, 6,
        1, 6, 2,
// bottom face
        4, 0, 3,
        4, 3, 7
};

void CalculatePerVertexNormals(std::vector<KGV::Render::Vertex>& vertices, const std::vector<unsigned int>& indices)
{
    // Clear normals
    for (auto& vertex : vertices)
    {
        vertex.normal = { 0.0f, 0.0f, 0.0f };
    }

    // Calculate face normals and accumulate them to vertices
    for (size_t i = 0; i < indices.size(); i += 3)
    {
        DirectX::XMVECTOR v0 = DirectX::XMLoadFloat4(&vertices[indices[i]].position);
        DirectX::XMVECTOR v1 = DirectX::XMLoadFloat4(&vertices[indices[i + 1]].position);
        DirectX::XMVECTOR v2 = DirectX::XMLoadFloat4(&vertices[indices[i + 2]].position);

        DirectX::XMVECTOR faceNormal = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(DirectX::XMVectorSubtract(v1, v0), DirectX::XMVectorSubtract(v2, v0)));

        for (int j = 0; j < 3; ++j)
        {
            DirectX::XMFLOAT3 normal;
            DirectX::XMStoreFloat3(&normal, faceNormal);

            vertices[indices[i + j]].normal.x += normal.x;
            vertices[indices[i + j]].normal.y += normal.y;
            vertices[indices[i + j]].normal.z += normal.z;
        }
    }

    // Normalize accumulated normals
    for (auto& vertex : vertices)
    {
        DirectX::XMVECTOR normal = DirectX::XMLoadFloat3(&vertex.normal);
        normal = DirectX::XMVector3Normalize(normal);
        DirectX::XMStoreFloat3(&vertex.normal, normal);
    }
}


XMVECTOR CalculateFaceNormal(XMVECTOR v0, XMVECTOR v1, XMVECTOR v2) {
    XMVECTOR edge1 = XMVectorSubtract(v1, v0);
    XMVECTOR edge2 = XMVectorSubtract(v2, v0);
    return XMVector3Normalize(XMVector3Cross(edge1, edge2));
}

bool KGV::System::ApplicationWin32::init() {
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
    window1 = std::make_unique<WindowWin32>( this, "KGV_Win32", 800, 600, 0, 0, "Hello World 1!", true );
//    window2 = std::make_unique<WindowWin32>( this, "KGV_Win32", 800, 600, 800, 0, "Hello World 2!", true );

    if ( !window1->getWin32Handle() ) {
        return false;
    }
//    } else if( !window2->getWin32Handle() )
//    {
//        g_log->writeToLog( Util::LogVerbosity::kInfo, Util::LogChannel::kSystem, "%s", "Failed to create Window 2" );
//        return false;
//    }

    window1->showWindow( true );
    auto log = spdlog::get("render");
    device = std::move(std::make_unique<Render::RenderDeviceDX11>(log));
    device->init();
    deviceContext = device->getPipelineManager();

    renderer = std::make_unique<Render::SimpleRenderer>(device.get(), deviceContext.get());
    spdlog::get("engine")->info("Initialization complete!");

    Render::SwapChainConfigDX11 swapChainConf;
    swapChainConf.setWidth(window1->getWidth());
    swapChainConf.setHeight(window1->getHeight());
    swapChainId = device->createSwapChain(window1->getWin32Handle(), swapChainConf);
    auto swapChain = device->getSwapChainById(swapChainId);

    rtvId = device->createRenderTargetView(swapChain->getResource()->getResourceId(), nullptr);

    vertexShaderId = device->loadShader("../KGVEngine/shaders/basicVertex.hlsl", Render::eShaderType::kVertex, false, "main", "vs_5_0");
    pixelShaderId = device->loadShader("../KGVEngine/shaders/basicPixel.hlsl", Render::eShaderType::kPixel, false, "main", "ps_5_0");

    std::vector<D3D11_INPUT_ELEMENT_DESC> inputElements = {
            {"POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
            {"NORMAL", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
            {"TEXCOORD", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
    };

    inputLayoutId = device->createInputLayout(vertexShaderId, inputElements);

    D3D11_VIEWPORT viewport;
    viewport.Width = static_cast<FLOAT>(window1->getWidth());
    viewport.Height = static_cast<FLOAT>(window1->getHeight()),
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;
    viewport.TopLeftX = 0.0f;
    viewport.TopLeftY = 0.0f;

    viewPortId = device->createViewPort(viewport);

    auto camera = std::make_shared<Engine::Entity>();
    camera->camera = std::make_unique<Engine::Camera>();
    camera->camera->setRtvId(rtvId);
    camera->camera->setViewPortId(viewPortId);
    camera->camera->setPerspectiveProject(XMConvertToRadians(70), (float)window1->getWidth()/(float)window1->getHeight(), 0.1, 100.0f);
    camera->transform.position.z = -3.0f;
    camera->transform.position.x = 0;
//    camera->camera->setIsWireframe(true);
    camera->transform.position.y = 2;
    camera->transform.rotation.x = XMConvertToRadians(30);
//    camera->transform.rotation.y = XMConvertToRadians(30.0f);

    cameras.emplace_back(camera);

    CalculatePerVertexNormals(gVertices, gIndices);
    cubeMeshId = renderer->createMesh({gVertices}, gIndices, Render::eBufferUpdateType::kImmutable);
    basicMatId = renderer->createMaterial(inputLayoutId, vertexShaderId, pixelShaderId);

    auto cube = std::make_shared<Engine::Entity>();
    cube->material = std::make_unique<Engine::MaterialComponent>();
    cube->material->color = { 1.0f, 0.5f, 0.31f, 1.0f };

    cube->mesh = std::make_unique<Engine::MeshComponent>();
    cube->mesh->meshId = cubeMeshId;
    cube->mesh->render = true;
    cube->transform.scale = { 0.5f, 0.5f, 0.5 };

    entities.emplace_back(cube);

    cube = std::make_shared<Engine::Entity>();
    cube->material = std::make_unique<Engine::MaterialComponent>();
    cube->material->color = { 1.0f, 1.0f, 1.0f, 1.0f };

    cube->mesh = std::make_unique<Engine::MeshComponent>();
    cube->mesh->meshId = cubeMeshId;
    cube->mesh->render = true;
    cube->transform.position = { 1.2f, 1.0f, -1.0f };
    cube->transform.scale = { 0.25f, 0.25f, 0.25 };
    cube->light = std::make_unique<Engine::LightComponent>();
    cube->light->color = { 1.0f, 1.0f, 1.0f, 1.0f };

    entities.emplace_back(cube);
    lights.emplace_back(cube);

    return true;
}

void KGV::System::ApplicationWin32::shutdown()
{
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
        float dt = duration_cast<duration<double>>(currentTime - lastTime).count();
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
        if ( wParam == VK_ESCAPE ) {
            DestroyWindow( hWnd );
            spdlog::get("engine")->info("Escape Key Pressed, exiting.", wParam);
        }
        spdlog::get("engine")->info("Keydown, Keycode: {}", wParam);
        break;
    case WM_DESTROY:
        DestroyWindow( hWnd );
        if ( window1 )
            PostQuitMessage( 0 );
        else if ( window1->getWin32Handle() == hWnd )
            window1.release();
//        else if ( window2->getWin32Handle() == hWnd )
//            window2.release();
        return 0;
    default:
        break;
    }

    return DefWindowProc( hWnd, msg, wParam, lParam );
}

int gAvgFps = 0;
float gAvgFrameTime = 0;
float cumalitiveTime = 0;

void KGV::System::ApplicationWin32::draw(F32 dt) {
    deviceContext->clearColorBuffers({0.1f, 0.1f, 0.1f, 1.0f});
    renderer->renderScene(entities, cameras, &lights, dt);
    constexpr float degPerSec = 1.0f;

//    entities[0]->transform.rotation.y = entities[0]->transform.rotation.y + degPerSec * dt;
    if (entities[0]->transform.rotation.y > 360.0f)
        entities[0]->transform.rotation.y -= 360.0f;
    else if (entities[0]->transform.rotation.y < 0)
        entities[0]->transform.rotation.y += 360.0f;

    device->presentSwapChain(swapChainId, 0, 0);
    gAvgFps = (1/dt + gAvgFps) / 2;
    gAvgFrameTime = (dt + gAvgFrameTime) / 2;
    cumalitiveTime += dt;

    if (cumalitiveTime > 0.25f) {
        window1->setWindowCaption(fmt::format("Frame rate: {:d}, Frame time: {:.3f}", gAvgFps, gAvgFrameTime));
        cumalitiveTime = 0.0f;
    }
}
