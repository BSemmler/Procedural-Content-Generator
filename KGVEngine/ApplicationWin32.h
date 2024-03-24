#pragma once
#include "pch.h"
#include "Application.h"
#include "WindowWin32.h"
//#include "RenderDeviceDX11.h"
//#include "Camera.h"
//#include "Entity.h"
#include "SimpleRenderer.h"
#include "ShaderManager.h"
#include "IScene.h"
#include <imgui.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx11.h>

namespace KGV::System
{
	class ApplicationWin32 : public Application, public IWndProc
	{
	public:
		// Inherited via Application
		bool	init()		override;
		void	shutdown()	override;
		int		run()		override;
        void draw(F32 deltaTime);

        void generateHeightMaps(int textureSize, int seed, double scale, double xOffset = 0, double yOffset = 0);
        void createHeightMapBuffers(int textureSize);

//        void setupPrimaryCamera(int width, int height, int topX, int topY);
//        void setupTextureViewer(int width, int height, int topX, int topY);
        void DrawGUI();

		// Inherited via IWndProc
		virtual LRESULT CALLBACK wndProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam ) override;

		std::unique_ptr<WindowWin32> window1;
		std::unique_ptr<WindowWin32> window2;
        std::shared_ptr<Render::RenderDeviceDX11> device;
        std::shared_ptr<Render::PipelineManagerDX11> deviceContext;
        std::shared_ptr<Render::SimpleRenderer> renderer;
        std::unique_ptr<Engine::IScene> terrainScene;
        S32 swapChainId;
        S32 rtvId;
        std::shared_ptr<Render::ResourceViewDX11> depthBuffer;
        std::shared_ptr<Render::ResourceViewDX11> terrainMapDisplacementTextureF32;
        std::shared_ptr<Render::ResourceViewDX11> terrainMapTextureFinalRGBA;
        std::shared_ptr<Engine::IShaderManager> shaderManager;
        std::chrono::high_resolution_clock::time_point lastTime;
        F64 waterKeyDebounceTime = 0;
        F64 seedChangeKeyDebounce = 0;
        F64 mapScale = 1024;
        S32 mapSize = 1024;
        S32 selectedScene = 0;
        S32 terrainOctaves = 8;
        F32 terrainAmplitude = 1.0f;
        F32 terrainFrequency = 1.0f;
        F32 terrainPersistence = 0.45f;
        F32 terrainLacunarity = 2.0f;
        F32 terrainSharpness = 0.0f;
        S32 terrainSharpnessEnhance = 2;
        unsigned int mapSeed = 0;
        bool keyDown[0xFF];

	};
}
