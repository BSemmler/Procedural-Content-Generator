
#if defined(_WIN32) || defined(_WIN64)
	// Debug definitions
#define KGV_WIN
#define WIN32_LEAN_AND_MEAN

#ifdef _DEBUG	
#define KGV_ASSERTIONS_ENABLED
#define KGV_SLOW_ASSERTIONS_ENABLED
#endif

#include <Windows.h>

// Include directx libraries
#include <d3d11_1.h>
#include <dxgi1_6.h>
#include <d3dcommon.h>
#include <DirectXMath.h>

// Include COM libraries.
#include <wrl.h>

using Microsoft::WRL::ComPtr;

#elif defined(__linux__)
#define KGV_LINUX
#endif

