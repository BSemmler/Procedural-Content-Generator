#include "ApplicationWin32.h"

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
        g_log->writeToLog(Util::LogVerbosity::kInfo, Util::LogChannel::kSystem, "%s", "Failed to create Window 1");
        return false;
    }
//    } else if( !window2->getWin32Handle() )
//    {
//        g_log->writeToLog( Util::LogVerbosity::kInfo, Util::LogChannel::kSystem, "%s", "Failed to create Window 2" );
//        return false;
//    }

    window1->showWindow( true );
    g_log->writeToLog( Util::LogVerbosity::kInfo, Util::LogChannel::kSystem, "%s", "Init Complete" );

    return true;
}

void KGV::System::ApplicationWin32::shutdown()
{
}

int KGV::System::ApplicationWin32::run() {
    MSG msg;
    memset(&msg, 0, sizeof(MSG));

    BOOL bRet = 1;
    while ( ( bRet = GetMessage(&msg, 0, 0, 0 )) != 0) {
        if ( bRet == -1 ) {
            MessageBox(nullptr, "GetMessage FAILED", "Error", MB_OK );
            break;
        } else {
            TranslateMessage( &msg );
            DispatchMessage( &msg );
        }
    }

    return ( int ) msg.wParam;
}

LRESULT KGV::System::ApplicationWin32::wndProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam ) {
    switch ( msg ) 	
    {
    case WM_LBUTTONDOWN:
        if (hWnd == window1->getWin32Handle() )
            MessageBox( nullptr, "Hello, World 1", "Hello", MB_OK );
        if ( hWnd == window2->getWin32Handle() )
            MessageBox( nullptr, "Hello, World 2", "Hello", MB_OK );

        g_log->writeToLog( Util::LogVerbosity::kInfo, Util::LogChannel::kSystem, "%s", "Left mouse button clicked" );
    case WM_KEYDOWN:
        if ( wParam == VK_ESCAPE ) {
            DestroyWindow( hWnd );
            g_log->writeToLog( Util::LogVerbosity::kInfo, Util::LogChannel::kSystem, "%s", "Escape Key Pressed. Exiting!" );
        }
        g_log->writeToLog( Util::LogVerbosity::kInfo, Util::LogChannel::kSystem, "%s Keycode: %d", "Keydown" );
        break;
    case WM_DESTROY:
        DestroyWindow( hWnd );
        if ( window1 && window2 )
            PostQuitMessage( 0 );
        else if ( window1->getWin32Handle() == hWnd )
            window1.release();
        else if ( window2->getWin32Handle() == hWnd )
            window2.release();


        return 0;
    default:
        break;
    }

    return DefWindowProc( hWnd, msg, wParam, lParam );
}
