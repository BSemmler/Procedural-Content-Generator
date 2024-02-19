cmake_minimum_required(VERSION 3.22)
project(Engine)

set(CMAKE_CXX_STANDARD 20)

#include_directories(Dependencies/glfw-3.3.2.bin.WIN64)
#include_directories(Dependencies/glfw-3.3.2.bin.WIN64/include)
#include_directories(Dependencies/glfw-3.3.2.bin.WIN64/include/GLFW)
#include_directories(Dependencies/glm/glm)
#include_directories(Dependencies/glm/glm/detail)
#include_directories(Dependencies/glm/glm/ext)
#include_directories(Dependencies/glm/glm/gtc)
#include_directories(Dependencies/glm/glm/gtx)
#include_directories(Dependencies/glm/glm/simd)
include_directories($ENV{UniversalCRTSdkDir}/Include/ucrt)
include_directories($ENV{UniversalCRTSdkDir}/Include/10.0.10056.0/ucrt)
include_directories(KVGEngine)

add_executable(Engine WIN32
        KVGEngine/Application.cpp
        KVGEngine/Application.h
        KVGEngine/ApplicationWin32.cpp
        KVGEngine/ApplicationWin32.h
        KVGEngine/BufferConfigDX11.cpp
        KVGEngine/BufferConfigDX11.h
        KVGEngine/BufferDX11.cpp
        KVGEngine/BufferDX11.h
        KVGEngine/Error.h
        KVGEngine/IndexBufferDX11.cpp
        KVGEngine/IndexBufferDX11.h
        KVGEngine/IRenderContextDX11.h
        KVGEngine/IWndProc.h
        KVGEngine/Log.cpp
        KVGEngine/Log.h
        KVGEngine/main.cpp
        KVGEngine/pch.cpp
        KVGEngine/pch.h
        KVGEngine/Platform.h
        KVGEngine/RenderDeviceDX11.cpp
        KVGEngine/RenderDeviceDX11.h
        KVGEngine/RenderTargetViewDX11.h
        KVGEngine/Resource.h
        KVGEngine/ResourceData.h
        KVGEngine/ShaderResourceViewDX11.h
        KVGEngine/StringUtil.cpp
        KVGEngine/StringUtil.h
        KVGEngine/SwapchainConfigDX11.h
        KVGEngine/Texture2dConfigDX11.cpp
        KVGEngine/Texture2dConfigDX11.h
        KVGEngine/Texture2dDX11.cpp
        KVGEngine/Texture2dDX11.h
        KVGEngine/Types.h
        KVGEngine/VertexBufferDX11.cpp
        KVGEngine/VertexBufferDX11.h
        KVGEngine/Window.cpp
        KVGEngine/Window.h
        KVGEngine/WindowWin32.cpp
        KVGEngine/WindowWin32.h)
