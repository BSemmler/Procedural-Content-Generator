#ifndef IWNDPROC_H_
#define IWNDPROC_H_
namespace KGV::System
{
	/**
	 * @brief This is an interface for a Win32 Window procedure function. A windowed application needs to have this
	 * interface implemented.
	 * 
	 * Provides method declaration for a window procedure. Needs to be implemented by the window owner so that messages.
	 * are handled by the application. Win32 Windows cannot be created otherwise.
	*/
	class IWndProc
	{
	public:
		virtual LRESULT __stdcall wndProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam ) = 0;
	};
}
#endif // IWNDPROC_H_