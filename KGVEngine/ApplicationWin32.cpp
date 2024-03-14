#include "ApplicationWin32.h"
#include "GeometryFactory.h"
using namespace DirectX;

std::vector<KGV::Render::Vertex> gVertices = {
    { XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT3(), XMFLOAT2() },
    { XMFLOAT3(-1.0f, +1.0f, -1.0f), XMFLOAT3(), XMFLOAT2() },
    { XMFLOAT3(+1.0f, +1.0f, -1.0f), XMFLOAT3(), XMFLOAT2() },
    { XMFLOAT3(+1.0f, -1.0f, -1.0f), XMFLOAT3(), XMFLOAT2() },
    { XMFLOAT3(-1.0f, -1.0f, +1.0f), XMFLOAT3(), XMFLOAT2() },
    { XMFLOAT3(-1.0f, +1.0f, +1.0f), XMFLOAT3(), XMFLOAT2() },
    { XMFLOAT3(+1.0f, +1.0f, +1.0f), XMFLOAT3(), XMFLOAT2() },
    { XMFLOAT3(+1.0f, -1.0f, +1.0f), XMFLOAT3(), XMFLOAT2() }
};

std::vector<U32> gIndices = {
//// front face
//        0, 1, 2,
//        0, 2, 3,
//// back face
//        4, 6, 5,
//        4, 7, 6,
//// left face
//        4, 5, 1,
//        4, 1, 0,
//// right face
//        3, 2, 6,
//        3, 6, 7,
//// top face
//        1, 5, 6,
//        1, 6, 2,
//// bottom face
//        4, 0, 3,
//        4, 3, 7

// Front face
        0, 1, 2,
        0, 2, 3,

// Back face
        4, 6, 5,
        4, 7, 6,

// Left face
        4, 5, 1,
        4, 1, 0,

// Right face
        3, 2, 6,
        3, 6, 7,

// Top face
        1, 5, 6,
        1, 6, 2,

// Bottom face
        4, 0, 3,
        4, 3, 7
};

// This method is flawed. If a vertice is connect to 5 faces the normal will be skewed to one direction. Only way to do it properly would be through quads.
//void CalculatePerVertexNormals(std::vector<KGV::Render::Vertex>& vertices, const std::vector<unsigned int>& indices)
//{
//    // Clear normals
//    for (auto& vertex : vertices)
//    {
//        vertex.normal = { 0.0f, 0.0f, 0.0f };
//    }
//
//    constexpr int vertsPerTriangle = 3;
//
//    // Calculate face normals and accumulate them to vertices
//    for (size_t i = 0; i < indices.size() / vertsPerTriangle; i++)
//    {
//        U32 i0 = indices[i * vertsPerTriangle + 0];
//        U32 i1 = indices[i * vertsPerTriangle + 1];
//        U32 i2 = indices[i * vertsPerTriangle + 2];
//
//        XMVECTOR v0 = DirectX::XMLoadFloat4(&vertices[i0].position);
//        XMVECTOR v1 = DirectX::XMLoadFloat4(&vertices[i1].position);
//        XMVECTOR v2 = DirectX::XMLoadFloat4(&vertices[i2].position);
//
//        XMVECTOR e0 = DirectX::XMVectorSubtract(v1, v0);
//        XMVECTOR e1 = DirectX::XMVectorSubtract(v2, v0);
//        XMVECTOR faceNormal = DirectX::XMVector3Cross(e0, e1);
//
//        XMVECTOR n1 = XMVectorAdd(XMLoadFloat3A(&vertices[i0].normal), faceNormal);
//        XMVECTOR n2 = XMVectorAdd(XMLoadFloat3A(&vertices[i1].normal), faceNormal);
//        XMVECTOR n3 = XMVectorAdd(XMLoadFloat3A(&vertices[i2].normal), faceNormal);
//
//        XMStoreFloat3A(&vertices[i0].normal, n1);
//        XMStoreFloat3A(&vertices[i1].normal, n2);
//        XMStoreFloat3A(&vertices[i2].normal, n3);
//    }
//
//    // Normalize accumulated normals
//    for (auto& vertex : vertices)
//    {
//        DirectX::XMVECTOR normal = DirectX::XMLoadFloat3(&vertex.normal);
//        normal = DirectX::XMVector3Normalize(normal);
//        DirectX::XMStoreFloat3(&vertex.normal, normal);
//    }
//}

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

    vertexShaderId = device->loadShader("../KGVEngine/shaders/basicLighting.hlsl", Render::eShaderType::kVertex, false, "VS", "vs_5_0");
    pixelShaderId = device->loadShader("../KGVEngine/shaders/basicLighting.hlsl", Render::eShaderType::kPixel, false, "PS", "ps_5_0");

    std::vector<D3D11_INPUT_ELEMENT_DESC> inputElements = {
            {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
            {"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
            {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
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
    camera->transform.position.y = 2;
    camera->transform.rotation.x = XMConvertToRadians(30);

    cameras.emplace_back(camera);


    std::vector<XMFLOAT3> verts;
    std::vector<XMFLOAT3> normals;
    std::vector<U32> indices;
    Engine::GeometryFactory::getCube(true, verts, normals, indices);

    std::vector<Render::Vertex> combinedCubeVerts;
    combinedCubeVerts.reserve(verts.size());
    for (int i = 0; i < verts.size(); ++i) {
        combinedCubeVerts.emplace_back(Render::Vertex{.position = verts[i], .normal = normals[i], .texCoord { 0.0f, 0.0f}});
    }

    cubeMeshId = renderer->createMesh({combinedCubeVerts}, indices, Render::eBufferUpdateType::kImmutable);
    basicMatId = renderer->createMaterial(inputLayoutId, vertexShaderId, pixelShaderId);

    auto cube = std::make_shared<Engine::Entity>();
    cube->transform.position.y = 1;
    cube->material = std::make_unique<Engine::MaterialComponent>();
    cube->material->ambient = { 0.0215f, 0.1745f, 0.0215f, 1.0f };
    cube->material->diffuse = { 0.07568f, 0.61424f, 0.07568f, 1.0f };
    cube->material->specular = { 0.633f, 0.727811f, 0.633f, 0.6 * 128 };

    cube->mesh = std::make_unique<Engine::MeshComponent>();
    cube->mesh->meshId = cubeMeshId;
    cube->mesh->render = true;

    entities.emplace_back(cube);

    auto light = std::make_shared<Engine::Entity>();
    light->transform.rotation.x = XMConvertToRadians(30);
    light->transform.rotation.y = XMConvertToRadians(5.15);
    light->light = std::make_unique<Engine::LightComponent>();
    light->light->ambient = {0.2f, 0.2f, 0.2f, 1.0f };
    light->light->diffuse = {0.7f, 0.7f, 0.7f, 1.0f };
    light->light->specular = { 1.0f, 1.0f, 1.0f, 1.0f };

    entities.emplace_back(light);
    lights.emplace_back(light);

    std::vector<XMFLOAT3> gridVerts;
    std::vector<XMFLOAT3> gridNormals;
    std::vector<U32> gridIndices;
    Engine::GeometryFactory::getVertexGridU32(5, gridVerts, gridNormals, gridIndices, 0.5);

    std::vector<Render::Vertex> combinedGridVerts;
    combinedCubeVerts.reserve(gridVerts.size());
    for (int i = 0; i < gridVerts.size(); ++i) {
        combinedGridVerts.emplace_back(Render::Vertex{.position = gridVerts[i], .normal = gridNormals[i], .texCoord { 0.0f, 0.0f}});
    }

    gridMeshId = renderer->createMesh({combinedGridVerts}, gridIndices, Render::eBufferUpdateType::kImmutable);

    auto grid = std::make_shared<Engine::Entity>();
    grid->transform.position.y = 0;
    grid->material = std::make_unique<Engine::MaterialComponent>();
    grid->material->ambient = { 0.0215f, 0.1745f, 0.0215f, 1.0f };
    grid->material->diffuse = { 0.07568f, 0.61424f, 0.07568f, 1.0f };
    grid->material->specular = { 0.633f, 0.727811f, 0.633f, 0.6 * 128 };

    grid->mesh = std::make_unique<Engine::MeshComponent>();
    grid->mesh->meshId = gridMeshId;
    grid->mesh->render = true;
    entities.emplace_back(grid);

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

        if (wParam == 0x5a)  { // Z key
            cameras[0]->camera->setIsWireframe(!cameras[0]->camera->isWireframe());
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
//
//    auto cameraRotate = XMMatrixRotationY(XMConvertToRadians(degPerSec * dt));
//    auto camForward = XMVector4Transform(XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f), cameraRotate);
////    auto orbit = cameraRotate * XMMatrixTranslation(0, 0, -5);
//
//    auto camPos = XMLoadFloat3A(&cameras[0]->transform.position);
//    auto newPos = XMVector3Transform(camPos, cameraRotate);
////    auto toOrigin = XMVector4Dot(XMVector4Normalize(cameraForward), { 0, 0, 0, 0});

//     XMStoreFloat3(&cameras[0]->transform.position, newPos);
    entities[0]->transform.rotation.x += degPerSec * dt;
    if (entities[0]->transform.rotation.x > 360.0f)
        entities[0]->transform.rotation.x -= 360.0f;
    else if (entities[0]->transform.rotation.x < 0)
        entities[0]->transform.rotation.x += 360.0f;

    entities[0]->transform.rotation.y += degPerSec * dt;
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
