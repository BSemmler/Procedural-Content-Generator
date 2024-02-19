#include "pch.h"
#include "ApplicationWin32.h"

//#define WIN32_LEAN_AND_MEAN
//#include <Windows.h>
//
//HWND gMainWnd   = 0;
//HWND gMainWnd1  = 0;
//int gXPos       = 0;
//int gYPos       = 0;
//int gWidth      = 800;
//int gHeight     = 600;
//
//bool InitWindow( HINSTANCE instanceHandle, int show );
//
//LRESULT CALLBACK WndProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
//
//int Run();

//using namespace KGV::Util;

std::shared_ptr<KGV::Util::Log> g_log;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, int nShowCmd) 
{
	g_log = std::make_shared<KGV::Util::Log>();
	
	g_log->setFilters( KGV::Util::LogVerbosity::kInfo, KGV::Util::LogChannel::kAllChannels );
	g_log->writeToLog( KGV::Util::LogVerbosity::kError, KGV::Util::LogChannel::kSystem, "%s", "Hello World!" );

	KGV::System::ApplicationWin32 app;
	if (!app.init())
        return 1;

	app.run();

	return 0;
}

//bool InitWindow( HINSTANCE instanceHandle, int show ) 
//{
//    WNDCLASS wc;
//    ZeroMemory( &wc, sizeof( WNDCLASS ) ); // Initialize the structure.
//
//    wc.style            = CS_HREDRAW | CS_VREDRAW;
//    wc.lpfnWndProc      = WndProc;
//    wc.cbClsExtra       = 0;
//    wc.cbWndExtra       = 0;
//    wc.hInstance        = instanceHandle;
//    wc.hIcon            = LoadIcon( 0, IDI_APPLICATION );
//    wc.hCursor          = LoadCursor( 0, IDC_ARROW );
//    wc.hbrBackground    = ( HBRUSH ) GetStockObject( WHITE_BRUSH );
//    wc.lpszMenuName     = 0;
//    wc.lpszClassName    = L"BasicWndClass";
//
//    if ( !RegisterClass( &wc ) ) 
//    {
//        MessageBox( 0, L"RegisterClass FAILED", 0, 0 );
//        return false;
//    }
//
//    gMainWnd = CreateWindow( L"BasicWndClass", L"Hello, World", WS_OVERLAPPEDWINDOW, 
//                             gXPos, gYPos, gWidth, gHeight, 0, 0, instanceHandle, 0 );
//
//    gMainWnd1 = CreateWindow( L"BasicWndClass", L"Hello, World", WS_OVERLAPPEDWINDOW, gXPos,
//                             gYPos, gWidth, gHeight, 0, 0, instanceHandle, 0 );
//
//    if ( gMainWnd == 0 ) 	
//    {
//        MessageBox( 0, L"CreateWindow FAILED", 0, 0 );
//        return false;
//    }
//
//    ShowWindow( gMainWnd, show );
//    UpdateWindow( gMainWnd );
//
//    ShowWindow( gMainWnd1, show );
//    UpdateWindow( gMainWnd1 );
//
//    return true;
//}
//
//int Run() 	
// {
//    MSG msg = { 0 };
//
//    BOOL bRet = 1;
//    while ( ( bRet = GetMessage(&msg, 0, 0, 0 )) != 0) 	
//    {
//        if ( bRet == -1 ) 	
//        {
//            MessageBox( 0, L"GetMessage FAILED", L"Error", MB_OK );
//            break;
//        } else {
//            TranslateMessage( &msg );
//            DispatchMessage( &msg );
//        }
//    }
//
//    return ( int ) msg.wParam;
//}
//
//LRESULT CALLBACK WndProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam ) 	
// {
//    switch ( msg ) 	
//    {
//    case WM_LBUTTONDOWN:
//        if (hWnd == gMainWnd )
//            MessageBox( 0, L"Hello, World 1", L"Hello", MB_OK );
//        if ( hWnd == gMainWnd1 )
//            MessageBox( 0, L"Hello, World 2", L"Hello", MB_OK );
//        return 0;
//    case WM_KEYDOWN:
//        if ( wParam == VK_ESCAPE ) 
//        {
//            DestroyWindow( hWnd );
//        }
//        break;
//    case WM_DESTROY:
//        DestroyWindow( hWnd );
//        if ( gMainWnd == hWnd )
//            gMainWnd = 0;
//        else if ( gMainWnd1 == hWnd )
//            gMainWnd1 = 0;
//        if ( !gMainWnd && !gMainWnd1 )
//            PostQuitMessage( 0 );
//        
//        return 0;
//    default:
//        break;
//    }
//
//    return DefWindowProc( hWnd, msg, wParam, lParam );
//}