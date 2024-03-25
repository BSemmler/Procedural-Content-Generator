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

find_package(imgui CONFIG REQUIRED)

add_executable(Engine
        KGVEngine/Application.cpp
        KGVEngine/Application.h
        KGVEngine/ApplicationWin32.cpp
        KGVEngine/ApplicationWin32.h
        KGVEngine/render/resources/config/BufferConfigDX11.cpp
        KGVEngine/render/resources/config/BufferConfigDX11.h
        KGVEngine/render/resources/containers/BufferDX11.cpp
        KGVEngine/render/resources/containers/BufferDX11.h
        KGVEngine/utility/Error.h
        KGVEngine/render/resources/containers/IndexBufferDX11.cpp
        KGVEngine/render/resources/containers/IndexBufferDX11.h
        KGVEngine/IWndProc.h
        KGVEngine/main.cpp
        KGVEngine/pch.cpp
        KGVEngine/pch.h
        KGVEngine/Platform.h
        KGVEngine/render/RenderDeviceDX11.cpp
        KGVEngine/render/RenderDeviceDX11.h
        KGVEngine/render/resources/views/RenderTargetViewDX11.h
        KGVEngine/render/resources/ResourceViewDX11.h
        KGVEngine/render/resources/ResourceData.h
        KGVEngine/render/resources/views/ShaderResourceViewDX11.h
        KGVEngine/render/resources/config/SwapChainConfigDX11.h
        KGVEngine/render/resources/config/Texture2dConfigDX11.cpp
        KGVEngine/render/resources/config/Texture2dConfigDX11.h
        KGVEngine/render/resources/containers/Texture2dDX11.cpp
        KGVEngine/render/resources/containers/Texture2dDX11.h
        KGVEngine/Types.h
        KGVEngine/render/resources/containers/VertexBufferDX11.cpp
        KGVEngine/render/resources/containers/VertexBufferDX11.h
        KGVEngine/Window.cpp
        KGVEngine/Window.h
        KGVEngine/WindowWin32.cpp
        KGVEngine/WindowWin32.h
        KGVEngine/render/resources/config/SwapChainConfigDX11.cpp
        KGVEngine/render/resources/views/ShaderResourceViewConfigDX11.cpp
        KGVEngine/render/resources/views/ShaderResourceViewConfigDX11.h
        KGVEngine/render/resources/views/RenderTargetViewConfigDX11.cpp
        KGVEngine/render/resources/views/RenderTargetViewConfigDX11.h
        KGVEngine/render/resources/ResourceViewDX11.cpp
        KGVEngine/render/resources/ResourceDX11.cpp
        KGVEngine/render/resources/ResourceDX11.h
        KGVEngine/render/resources/views/RenderTargetViewDX11.cpp
        KGVEngine/render/resources/views/ShaderResourceViewDX11.cpp
        KGVEngine/utility/StringUtil.h
        KGVEngine/render/resources/containers/SwapChainDX11.cpp
        KGVEngine/render/resources/containers/SwapChainDX11.h
        KGVEngine/render/shader/ShaderDX11.cpp
        KGVEngine/render/shader/ShaderDX11.h
        KGVEngine/render/resources/containers/VertexShaderDX11.cpp
        KGVEngine/render/resources/containers/VertexShaderDX11.h
        KGVEngine/render/shader/PixelShaderDX11.cpp
        KGVEngine/render/shader/PixelShaderDX11.h
        KGVEngine/render/pipeline/InputAssemblerStateDX11.cpp
        KGVEngine/render/pipeline/InputAssemblerStateDX11.h
        KGVEngine/render/pipeline/ShaderStageStateDX11.cpp
        KGVEngine/render/pipeline/ShaderStageStateDX11.h
        KGVEngine/render/pipeline/RasterizerStageStateDX11.cpp
        KGVEngine/render/pipeline/RasterizerStageStateDX11.h
        KGVEngine/render/pipeline/OutputMergerStageStateDX11.cpp
        KGVEngine/render/pipeline/OutputMergerStageStateDX11.h
        KGVEngine/render/pipeline/InputAssemblerStageDX11.cpp
        KGVEngine/render/pipeline/InputAssemblerStageDX11.h
        KGVEngine/render/shader/VertexShaderStageDX11.cpp
        KGVEngine/render/shader/VertexShaderStageDX11.h
        KGVEngine/render/pipeline/IPipelineStageDX11.h
        KGVEngine/render/resources/containers/ConstantBufferDX11.cpp
        KGVEngine/render/resources/containers/ConstantBufferDX11.h
        KGVEngine/render/pipeline/PipelineManagerDX11.cpp
        KGVEngine/render/pipeline/PipelineManagerDX11.h
        KGVEngine/render/pipeline/PixelShaderStageDX11.cpp
        KGVEngine/render/pipeline/PixelShaderStageDX11.h
        KGVEngine/render/pipeline/RasterizerStageDX11.cpp
        KGVEngine/render/pipeline/RasterizerStageDX11.h
        KGVEngine/render/pipeline/OutputMergerStageDX11.cpp
        KGVEngine/render/pipeline/OutputMergerStageDX11.h
        KGVEngine/render/pipeline/PipelineStateDX11.h
        KGVEngine/engine/Entity.cpp
        KGVEngine/engine/Entity.h
        KGVEngine/engine/entitycomponents/MeshComponent.cpp
        KGVEngine/engine/entitycomponents/MeshComponent.h
        KGVEngine/engine/entitycomponents/TransformComponent.cpp
        KGVEngine/engine/entitycomponents/TransformComponent.h
        KGVEngine/engine/Camera.cpp
        KGVEngine/engine/Camera.h
        KGVEngine/render/renderers/simple/SimpleRenderer.cpp
        KGVEngine/render/renderers/simple/SimpleRenderer.h
        KGVEngine/render/renderers/simple/RenderMesh.cpp
        KGVEngine/render/renderers/simple/RenderMesh.h
        KGVEngine/render/renderers/simple/RenderMaterial.cpp
        KGVEngine/render/renderers/simple/RenderMaterial.h
        KGVEngine/engine/entitycomponents/MaterialComponent.cpp
        KGVEngine/engine/entitycomponents/MaterialComponent.h
        KGVEngine/engine/entitycomponents/LightComponent.cpp
        KGVEngine/engine/entitycomponents/LightComponent.h
        KGVEngine/utility/GeometryFactory.cpp
        KGVEngine/utility/GeometryFactory.h
        KGVEngine/render/resources/config/DepthStencilViewConfigDX11.cpp
        KGVEngine/render/resources/config/DepthStencilViewConfigDX11.h
        KGVEngine/render/resources/containers/DepthStencilViewDX11.cpp
        KGVEngine/render/resources/containers/DepthStencilViewDX11.h
        KGVEngine/procedural/PerlinNoise.h
        KGVEngine/procedural/PerlinNoise.cpp
        KGVEngine/render/resources/config/SamplerStateDX11.cpp
        KGVEngine/render/resources/config/SamplerStateDX11.h
        KGVEngine/procedural/MultithreadedNoiseExecutor.cpp
        KGVEngine/procedural/MultithreadedNoiseExecutor.h
        KGVEngine/utility/TargaImage.cpp
        KGVEngine/utility/TargaImage.h
        KGVEngine/Scenes/IScene.cpp
        KGVEngine/Scenes/IScene.h
        KGVEngine/Scenes/TerrainScene.cpp
        KGVEngine/Scenes/TerrainScene.h
        KGVEngine/engine/IShaderManager.cpp
        KGVEngine/engine/IShaderManager.h
        KGVEngine/render/shader/ShaderManager.cpp
        KGVEngine/render/shader/ShaderManager.h
        KGVEngine/Scenes/HeightMapScene.cpp
        KGVEngine/Scenes/HeightMapScene.h
        KGVEngine/Scenes/WaveformScene.cpp
        KGVEngine/Scenes/WaveformScene.h
        KGVEngine/render/resources/config/Texture1dConfigDX11.cpp
        KGVEngine/render/resources/config/Texture1dConfigDX11.h
        KGVEngine/render/resources/containers/Texture1dDX11.cpp
        KGVEngine/render/resources/containers/Texture1dDX11.h
)

target_link_libraries(Engine PRIVATE spdlog::spdlog imgui::imgui  d3d11.lib dxgi.lib d3dcompiler.lib)
