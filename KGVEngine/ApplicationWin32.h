#pragma once
#include "pch.h"
#include "Application.h"
#include "WindowWin32.h"
//#include "RenderDeviceDX11.h"
//#include "Camera.h"
//#include "Entity.h"
#include "SimpleRenderer.h"

namespace KGV::System
{
	class ApplicationWin32 : public Application, public IWndProc
	{
	public:
		// Inherited via Application
		bool	init()		override;
		void	shutdown()	override;
		int		run()		override;
        void draw(F32 dt);

        void createHeightMaps();

		// Inherited via IWndProc
		virtual LRESULT CALLBACK wndProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam ) override;

		std::unique_ptr<WindowWin32> window1;
		std::unique_ptr<WindowWin32> window2;
        std::shared_ptr<Render::RenderDeviceDX11> device;
        std::shared_ptr<Render::PipelineManagerDX11> deviceContext;
        std::unique_ptr<Render::SimpleRenderer> renderer;
        S32 swapChainId;
        S32 rtvId;
        S32 inputLayoutId;
        std::shared_ptr<Render::ResourceViewDX11> vertexBuffer;
        std::shared_ptr<Render::ResourceViewDX11> indexBuffer;
        std::shared_ptr<Render::ResourceViewDX11> constantBuffer;
        std::shared_ptr<Render::ResourceViewDX11> depthBuffer;
        std::shared_ptr<Render::ResourceViewDX11> terrainMapTextureF32;
        std::shared_ptr<Render::ResourceViewDX11> terrainMapTextureRGBA;
        std::shared_ptr<Render::ResourceViewDX11> terrainColorTexture;
        std::shared_ptr<Render::ResourceViewDX11> waterMapTexture;
        S32 vertexShaderId;
        S32 terrainVertexShaderId;
        S32 pixelShaderId;
        S32 planeVertexShaderId;
        S32 planePixelShaderId;
        S32 viewPortId;
        std::vector<std::shared_ptr<Engine::Entity>> cameras;
        std::vector<std::shared_ptr<Engine::Entity>> lights;
        std::vector<std::shared_ptr<Engine::Entity>> entities;
        std::shared_ptr<Engine::Entity> texturePlaneEntity;
        std::shared_ptr<Engine::Entity> texturePlaneCamera;
        std::shared_ptr<Engine::Entity> waterPlane;
        S32 cubeMeshId;
        S32 gridMeshId;
        S32 basicMatId;
        std::chrono::high_resolution_clock::time_point lastTime;
	};
}
