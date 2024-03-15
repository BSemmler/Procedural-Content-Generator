cmake_minimum_required(VERSION 3.22)
project(Engine)

set(CMAKE_CXX_STANDARD 20)

set(SUBSYSTEM_LINKER_OPTIONS "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")

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
include_directories(KGVEngine)

if(NOT TARGET spdlog)
    # Stand-alone build
    find_package(spdlog REQUIRED)
endif()

add_executable(Engine
        KGVEngine/Application.cpp
        KGVEngine/Application.h
        KGVEngine/ApplicationWin32.cpp
        KGVEngine/ApplicationWin32.h
        KGVEngine/BufferConfigDX11.cpp
        KGVEngine/BufferConfigDX11.h
        KGVEngine/BufferDX11.cpp
        KGVEngine/BufferDX11.h
        KGVEngine/Error.h
        KGVEngine/IndexBufferDX11.cpp
        KGVEngine/IndexBufferDX11.h
        KGVEngine/IRenderContextDX11.h
        KGVEngine/IWndProc.h
        KGVEngine/main.cpp
        KGVEngine/pch.cpp
        KGVEngine/pch.h
        KGVEngine/Platform.h
        KGVEngine/RenderDeviceDX11.cpp
        KGVEngine/RenderDeviceDX11.h
        KGVEngine/RenderTargetViewDX11.h
        KGVEngine/ResourceViewDX11.h
        KGVEngine/ResourceData.h
        KGVEngine/ShaderResourceViewDX11.h
        KGVEngine/SwapChainConfigDX11.h
        KGVEngine/Texture2dConfigDX11.cpp
        KGVEngine/Texture2dConfigDX11.h
        KGVEngine/Texture2dDX11.cpp
        KGVEngine/Texture2dDX11.h
        KGVEngine/Types.h
        KGVEngine/VertexBufferDX11.cpp
        KGVEngine/VertexBufferDX11.h
        KGVEngine/Window.cpp
        KGVEngine/Window.h
        KGVEngine/WindowWin32.cpp
        KGVEngine/WindowWin32.h
        KGVEngine/SwapChainConfigDX11.cpp
        KGVEngine/ShaderResourceViewConfigDX11.cpp
        KGVEngine/ShaderResourceViewConfigDX11.h
        KGVEngine/RenderTargetViewConfigDX11.cpp
        KGVEngine/RenderTargetViewConfigDX11.h
        KGVEngine/ResourceViewDX11.cpp
        KGVEngine/ResourceDX11.cpp
        KGVEngine/ResourceDX11.h
        KGVEngine/RenderTargetViewDX11.cpp
        KGVEngine/ShaderResourceViewDX11.cpp
        KGVEngine/StringUtil.h
        KGVEngine/SwapChainDX11.cpp
        KGVEngine/SwapChainDX11.h
        KGVEngine/ShaderDX11.cpp
        KGVEngine/ShaderDX11.h
        KGVEngine/VertexShaderDX11.cpp
        KGVEngine/VertexShaderDX11.h
        KGVEngine/PixelShaderDX11.cpp
        KGVEngine/PixelShaderDX11.h
        KGVEngine/InputAssemblerStateDX11.cpp
        KGVEngine/InputAssemblerStateDX11.h
        KGVEngine/ShaderStageStateDX11.cpp
        KGVEngine/ShaderStageStateDX11.h
        KGVEngine/RasterizerStageStateDX11.cpp
        KGVEngine/RasterizerStageStateDX11.h
        KGVEngine/OutputMergerStageStateDX11.cpp
        KGVEngine/OutputMergerStageStateDX11.h
        KGVEngine/InputAssemblerStageDX11.cpp
        KGVEngine/InputAssemblerStageDX11.h
        KGVEngine/VertexShaderStageDX11.cpp
        KGVEngine/VertexShaderStageDX11.h
        KGVEngine/IPipelineStageDX11.h
        KGVEngine/ConstantBufferDX11.cpp
        KGVEngine/ConstantBufferDX11.h
        KGVEngine/PipelineManagerDX11.cpp
        KGVEngine/PipelineManagerDX11.h
        KGVEngine/PixelShaderStageDX11.cpp
        KGVEngine/PixelShaderStageDX11.h
        KGVEngine/RasterizerStageDX11.cpp
        KGVEngine/RasterizerStageDX11.h
        KGVEngine/OutputMergerStageDX11.cpp
        KGVEngine/OutputMergerStageDX11.h
        KGVEngine/PipelineStateDX11.h
        KGVEngine/Entity.cpp
        KGVEngine/Entity.h
        KGVEngine/MeshComponent.cpp
        KGVEngine/MeshComponent.h
        KGVEngine/TransformComponent.cpp
        KGVEngine/TransformComponent.h
        KGVEngine/Camera.cpp
        KGVEngine/Camera.h
        KGVEngine/SimpleRenderer.cpp
        KGVEngine/SimpleRenderer.h
        KGVEngine/RenderMesh.cpp
        KGVEngine/RenderMesh.h
        KGVEngine/RenderMaterial.cpp
        KGVEngine/RenderMaterial.h
        KGVEngine/MaterialComponent.cpp
        KGVEngine/MaterialComponent.h
        KGVEngine/LightComponent.cpp
        KGVEngine/LightComponent.h
        KGVEngine/GeometryFactory.cpp
        KGVEngine/GeometryFactory.h
        KGVEngine/DepthStencilViewConfigDX11.cpp
        KGVEngine/DepthStencilViewConfigDX11.h
        KGVEngine/DepthStencilViewDX11.cpp
        KGVEngine/DepthStencilViewDX11.h)

target_link_libraries(Engine PRIVATE spdlog::spdlog d3d11.lib dxgi.lib d3dcompiler.lib)
