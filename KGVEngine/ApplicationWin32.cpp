#include "ApplicationWin32.h"

struct BasicVertex {
    DirectX::XMFLOAT4 position;
    DirectX::XMFLOAT4 color;
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

constexpr S32 gNumVertices = 3;
const BasicVertex gTriangle[gNumVertices] = {
        // pos(x, y z, 1)   color(r,g,b,a)
    { DirectX::XMFLOAT4( 1.0f, -1.0f, 0.0f, 1.0f ),  DirectX::XMFLOAT4( 1.0f, 0.0f, 0.0f, 1.0f ) }, // Bottom right.
    { DirectX::XMFLOAT4( -1.0f, -1.0f, 0.0f, 1.0f ), DirectX::XMFLOAT4( 0.0f, 1.0f, 0.0f, 1.0f ) }, // Bottom left.
    { DirectX::XMFLOAT4( 0.0f, 1.0f, 0.0f, 1.0f ),   DirectX::XMFLOAT4( 0.0f, 0.0f, 1.0f, 1.0f ) }, // Top.
};

constexpr S32 gNumIndices = 3;
const S32 gTriangleIndices[gNumVertices] = {1, 2, 0 }; // Clockwise order.

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
            {"POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
            {"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
    };

    inputLayoutId = device->createInputLayout(vertexShaderId, inputElements);

    Render::BufferConfigDX11 vertexBufferConf;
    vertexBufferConf.setDefaultVertexBuffer(sizeof(BasicVertex) * gNumVertices, false);
    ResourceData vertices = {.Data = gTriangle, .memPitch = 0, .memSlicePitch = 0};
    vertexBufferId = device->createVertexBuffer(vertexBufferConf, vertices);

    Render::BufferConfigDX11 indexBufferConf;
    indexBufferConf.setDefaultIndexBuffer(sizeof(S32) * gNumIndices, false);
    ResourceData indices = { .Data = gTriangleIndices, .memPitch = 0, .memSlicePitch = 0};
    indexBufferId = device->createIndexBuffer(indexBufferConf, indices);
    D3D11_VIEWPORT viewport;
    viewport.Width = static_cast<FLOAT>(window1->getWidth());
    viewport.Height = static_cast<FLOAT>(window1->getHeight()),
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;
    viewport.TopLeftX = 0.0f;
    viewport.TopLeftY = 0.0f;


    Render::InputAssemblerStateDX11 ia;
    ia.setInputLayout(inputLayoutId);
    ia.setIndexBuffer(indexBufferId->getResourceId());
    ia.setIndexFormat(DXGI_FORMAT_R32_UINT);
    ia.setVertexBuffers({vertexBufferId->getResourceId()}, {sizeof(BasicVertex)}, {0});
    ia.setTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    pipelineState.iaState = &ia;

    Render::ShaderStageStateDX11 vs;
    vs.setShaderId(vertexShaderId);
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

    while(msg.message != WM_QUIT) {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);

            if (msg.message == WM_QUIT)
                break;
        }

        immediateContext->drawIndexed(gNumIndices, 0, 0);
        device->presentSwapChain(swapChainId, 0, 0);
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
