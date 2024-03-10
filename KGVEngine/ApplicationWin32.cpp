#include "ApplicationWin32.h"
using namespace DirectX;
struct BasicVertex {
    DirectX::XMFLOAT3 position;
    DirectX::XMFLOAT4 color;
};

struct MatrixBufferType
{
    XMMATRIX world;
    XMMATRIX view;
    XMMATRIX projection;
};

/*
*              Triangle vertex coordinates (2D)
*                       
*                           x (0.0, 1.0)  
*                         /  \
*                        /    \
*                       /      \
*                      /        \
*                     /          \
*                    /            \
*                   /______________\
*     (-1.0, -1.0) x                x (1.0, -1.0)
* 
*/

constexpr S32 gNumVertices = 8;
//const BasicVertex gTriangle[gNumVertices] = {
//        // pos(x, y z, 1)   color(r,g,b,a)
//    { DirectX::XMFLOAT4( 1.0f, -1.0f, 0.0f, 1.0f ),  DirectX::XMFLOAT4( 1.0f, 0.0f, 0.0f, 1.0f ) }, // Bottom right.
//    { DirectX::XMFLOAT4( -1.0f, -1.0f, 0.0f, 1.0f ), DirectX::XMFLOAT4( 0.0f, 1.0f, 0.0f, 1.0f ) }, // Bottom left.
//    { DirectX::XMFLOAT4( 0.0f, 1.0f, 0.0f, 1.0f ),   DirectX::XMFLOAT4( 0.0f, 0.0f, 1.0f, 1.0f ) }, // Top.
//};


BasicVertex gVertices[] = {
    { XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT4( 1.0f, 0.0f, 0.0f, 1.0f ) },
    { XMFLOAT3(-1.0f, +1.0f, -1.0f), XMFLOAT4( 0.0f, 1.0f, 0.0f, 1.0f ) },
    { XMFLOAT3(+1.0f, +1.0f, -1.0f), XMFLOAT4( 0.0f, 0.0f, 1.0f, 1.0f ) },
    { XMFLOAT3(+1.0f, -1.0f, -1.0f), XMFLOAT4( 1.0f, 0.0f, 0.0f, 1.0f ) },
    { XMFLOAT3(-1.0f, -1.0f, +1.0f), XMFLOAT4( 0.0f, 1.0f, 0.0f, 1.0f ) },
    { XMFLOAT3(-1.0f, +1.0f, +1.0f), XMFLOAT4( 0.0f, 0.0f, 1.0f, 1.0f ) },
    { XMFLOAT3(+1.0f, +1.0f, +1.0f), XMFLOAT4( 1.0f, 0.0f, 0.0f, 1.0f ) },
    { XMFLOAT3(+1.0f, -1.0f, +1.0f), XMFLOAT4( 0.0f, 1.0f, 0.0f, 1.0f ) }
};

U32 gIndices[] = {
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

constexpr S32 gNumIndices = 36;
//const S32 gTriangleIndices[gNumVertices] = {1, 2, 0 }; // Clockwise order.

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
    immediateContext = device->getPipelineManager();
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
            {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
            {"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
    };

    inputLayoutId = device->createInputLayout(vertexShaderId, inputElements);

    Render::BufferConfigDX11 vertexBufferConf{};
    vertexBufferConf.setDefaultVertexBuffer(sizeof(BasicVertex) * gNumVertices, false);
    ResourceData vertices = {.Data = gVertices, .memPitch = 0, .memSlicePitch = 0};
    vertexBuffer = device->createVertexBuffer(vertexBufferConf, &vertices);

    Render::BufferConfigDX11 indexBufferConf{};
    indexBufferConf.setDefaultIndexBuffer(sizeof(S32) * gNumIndices, false);
    ResourceData indices = { .Data = gIndices, .memPitch = 0, .memSlicePitch = 0};
    indexBuffer = device->createIndexBuffer(indexBufferConf, &indices);
    D3D11_VIEWPORT viewport;
    viewport.Width = static_cast<FLOAT>(window1->getWidth());
    viewport.Height = static_cast<FLOAT>(window1->getHeight()),
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;
    viewport.TopLeftX = 0.0f;
    viewport.TopLeftY = 0.0f;

    triangle.mesh = std::make_shared<Engine::Mesh>();
    triangle.mesh->vertexBuffer = vertexBuffer;
    triangle.mesh->vertexCount = gNumVertices;
    triangle.mesh->indexBuffer = indexBuffer;
    triangle.mesh->indexCount = gNumIndices;
    triangle.mesh->offset = 0;

    triangle.transform.position = { 1.0f, 1.0f, 1.0f, 1.0f };
    camera.setPerspectiveProject(90, static_cast<float>(window1->getWidth() ) / static_cast<float>(window1->getHeight()), 0.5f, 10.0f);

    Render::BufferConfigDX11 constantBufferConf{};
    constantBufferConf.setDefaultConstantBuffer(sizeof(MatrixBufferType), true);
    constantBuffer = device->createConstantBuffer(constantBufferConf, nullptr);

    Render::InputAssemblerStateDX11 ia;
    ia.setInputLayout(inputLayoutId);
    ia.setIndexBuffer(indexBuffer->getResourceId());
    ia.setIndexFormat(DXGI_FORMAT_R32_UINT);
    ia.setVertexBuffers({vertexBuffer->getResourceId()}, {sizeof(BasicVertex)}, {0});
    ia.setTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    pipelineState.iaState = &ia;

    Render::ShaderStageStateDX11 vs;
    vs.setShaderId(vertexShaderId);
    vs.setConstantBuffersIds({constantBuffer->getResourceId()});
    pipelineState.vsState = &vs;

    Render::RasterizerStageStateDX11 rs;
    viewPortId = device->createViewPort(viewport);
    rs.setViewportIds({viewPortId});
    pipelineState.rsState = &rs;

    Render::ShaderStageStateDX11 ps;
    ps.setShaderId(pixelShaderId);
    pipelineState.psState = &ps;

    Render::OutputMergerStageStateDX11 om;
    om.setRtvIds({rtvId});
    pipelineState.omState = &om;

    immediateContext->applyState(pipelineState);

    return true;
}

void KGV::System::ApplicationWin32::shutdown()
{
}

int KGV::System::ApplicationWin32::run() {
    using namespace std::chrono;
    MSG msg;
    memset(&msg, 0, sizeof(MSG));

    BOOL bRet = 1;
//    while ( ( bRet = GetMessage(&msg, 0, 0, 0 )) != 0) {
//        if ( bRet == -1 ) {
//            MessageBox(nullptr, "GetMessage FAILED", "Error", MB_OK );
//            break;
//        } else {
//            TranslateMessage( &msg );
//            DispatchMessage( &msg );
//        }
//    }

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
        if (hWnd == window1->getWin32Handle() )
            MessageBox( nullptr, "Hello, World 1", "Hello", MB_OK );
//        if ( hWnd == window2->getWin32Handle() )
//            MessageBox( nullptr, "Hello, World 2", "Hello", MB_OK );


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

void KGV::System::ApplicationWin32::draw(F32 dt) {
    static float degPerSec = 5.0f;
    spdlog::get("engine")->info("Delta time: {}", dt);

    immediateContext->clearColorBuffers({0.0f, 0.0f, 0.0f, 1.0f});

    triangle.transform.rotation.y = triangle.transform.rotation.y + degPerSec * dt;
    if (triangle.transform.rotation.y > 360.0f)
        triangle.transform.rotation.y -= 360.0f;

    spdlog::get("engine")->info("Rotation y: {}", triangle.transform.rotation.y);

    auto world = XMMatrixIdentity();
    world = world * XMMatrixScalingFromVector(XMLoadFloat4(&triangle.transform.scale));
    world = world * XMMatrixRotationRollPitchYawFromVector(XMLoadFloat4(&triangle.transform.rotation));
    world = world * XMMatrixTranslationFromVector(XMLoadFloat4(&triangle.transform.position));
//    world = world * XMMatrixRotationRollPitchYaw(0.0, degPerSec * dt, 0.0f);
//        world = XMMatrixTranslation();

    auto view = XMMatrixLookAtLH({0.0f, 0.0f, -5.0f},
                                 {0.0f, 0.0f, 1.0f},
                                 {0.0f, 1.0f, 0.0f});
    auto proj = XMMatrixPerspectiveFovLH(90.0f, static_cast<float>(window1->getWidth() ) / static_cast<float>(window1->getHeight()), 0.1f, 10.f);
    auto t = immediateContext->mapResource(constantBuffer.get(), 0, D3D11_MAP_WRITE_DISCARD, 0);
    MatrixBufferType* mat;
    mat = (MatrixBufferType*)t.pData;
    mat->world = world;
    mat->view = view;
    mat->projection = proj;
    immediateContext->unmapResource(constantBuffer.get(), 0);
    immediateContext->drawIndexed(gNumIndices, 0, 0);

//        immediateContext->applyState(pipelineState);
    device->presentSwapChain(swapChainId, 0, 0);
}
