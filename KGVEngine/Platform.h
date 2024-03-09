
#if defined(_WIN32) || defined(_WIN64)
	// Debug definitions
#define KGV_WIN 1
#ifdef _DEBUG	
#undef KGV_ASSERTIONS_ENABLED
#undef KGV_SLOW_ASSERTIONS_ENABLED
#endif
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

// Include directx libraries
#include <d3d11_1.h>
#include <dxgi1_6.h>
#include <d3dcommon.h>
#include <DirectXMath.h>
#include <d3dcompiler.h>

// Include COM libraries.
#include <wrl.h>

using Microsoft::WRL::ComPtr;

#elif defined(__linux__)
#define KGV_LINUX
#endif

