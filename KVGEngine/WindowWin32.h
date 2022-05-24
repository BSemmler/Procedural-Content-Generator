#pragma once
#include "Window.h"
#ifdef KGV_WIN
#include "IWndProc.h"

namespace KGV::System 
{
	/**
	 * @brief This class provides the implementation of a window that utilizes the Win32 sub system 
	 * on Windows. 
	*/
	class WindowWin32 : public Window 
	{
	public:
		/**
		 * @brief Creates the window with the desired parameters.
		 * @param procObj An object that implements the IWndProc interface for handeling the window procedure of this window.
		 * @param width [OPTIONAL] Sets the desired width in pixels of the window. Default = 800
		 * @param height [OPTIONAL] Sets the desired height in pixels of the window. Default = 600
		 * @param xPos [OPTIONAL] Sets the desired x position of the window in pixels of the window. Default = 0 (Left)
		 * @param yPos [OPTIONAL] Sets the desired y position of the window in pixels of the window. Default = 0 (Top)
		 * @param caption [OPTIONAL] Sets the desired text (caption) at the top of the window. Default = "Hello World"
		 * @param showState [OPTIONAL] Sets the window as visible if true and hidden if false. 
		*/
		WindowWin32( IWndProc* procObj, const std::wstring& className, S32 width = 800, S32 height = 600,
					 S32 xPos = 0, S32 yPos = 0, const std::string& caption = "Hello World", 
					 bool showWindow = false );

		virtual ~WindowWin32();

		// Inherited via Window
		virtual bool isWindowVisible()								override;
		virtual void showWindow( bool showState )					override;
		virtual void resizeWindow( S32 width, S32 height )			override;
		virtual void repositionWindow( S32 xPos, S32 yPos )			override;
		virtual void setWindowCaption( const std::string& caption ) override;
		virtual HWND getWin32Handle();
		static	bool registerClass(WNDCLASSEX &wc);
	protected:
		HWND wndHandle;
		
		// Inherited via Window
		
	};
}
#endif