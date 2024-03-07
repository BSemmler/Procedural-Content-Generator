#pragma once
#include "pch.h"
#include "Application.h"
#include "WindowWin32.h"
#include "RenderDeviceDX11.h"

namespace KGV::System
{
	class ApplicationWin32 : public Application, public IWndProc
	{
	public:
		// Inherited via Application
		virtual bool	init()		override;
		virtual void	shutdown()	override;
		virtual int		run()		override;

		// Inherited via IWndProc
		virtual LRESULT CALLBACK wndProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam ) override;

		std::unique_ptr<WindowWin32> window1;
		std::unique_ptr<WindowWin32> window2;
        std::unique_ptr<Render::RenderDeviceDX11> device;
        S32 swapChainId;
        S32 rtvId;
	};
}
