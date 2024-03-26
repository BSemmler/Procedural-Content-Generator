#include "WindowWin32.h"
#ifdef KGV_WIN
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>


using namespace KGV::System;

/**
 * @brief This function provides us a hook to the window message procedure. The window
 * needs a window procedure and we cant immediately set the one we want to declare in the owner class.
 * So we store our desired window procedure inside the extra bytes of the window and then retrieve that and use it.
*/
LRESULT CALLBACK internalWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    // Get the pointer to the external WndProc that's stored inside the extra bytes of the window.
    LONG_PTR wndProcObjPtr = GetWindowLongPtr(hWnd, 0);

    // If the stored wndProc is null then call the default window procedure else use the stored procedure.
    if (wndProcObjPtr == 0) {
        return DefWindowProc(hWnd, msg, wParam, lParam);
    } else {
        return reinterpret_cast< IWndProc * >( wndProcObjPtr )->wndProc(hWnd, msg, wParam, lParam);
    }
}

WindowWin32::WindowWin32(IWndProc *procObj, const std::string &className, S32 width, S32 height, S32 xPos, S32 yPos,
                         const std::string &caption, bool showWindow) :
        Window(width, height, xPos, yPos, caption, showWindow) {
    // Check that all the numeric values are non-negative.
    // KGV_ASSERT_TRUE(width > 0 && height > 0 && xPos >= 0 && yPos >= 0);

//    spdlog::get("window"),
//                      "New Window is being instantiated. Caption: %s, Width: %d, Height: %d, Position: (%d,%d)",
//                      caption.c_str(), width, height, xPos, yPos);

    isVisible = showWindow;


    wndHandle = CreateWindowEx(NULL,
                               reinterpret_cast<LPCSTR>(className.c_str()),
                               reinterpret_cast<LPCSTR>(std::wstring(windowCaption.begin(),
                                                                     windowCaption.end()).c_str()),
//                               WS_OVERLAPPEDWINDOW,
                               WS_POPUP,
                               xPos, yPos,
                               width, height,
                               nullptr,
                               nullptr,
                               nullptr,
                               nullptr);

    if (!wndHandle) {
        spdlog::get("engine")->error("Failed to create window! Error code: {}", GetLastError());
        return;
    }
    // Store our window procedure object inside the extra window bytes.
    SetWindowLongPtr((HWND)wndHandle, 0, reinterpret_cast< LONG_PTR >( procObj ));

    ShowWindow((HWND)wndHandle, isVisible);
    UpdateWindow((HWND)wndHandle);
}

WindowWin32::~WindowWin32() {
    // Cleanup window.
    if (wndHandle) {
        DestroyWindow((HWND)wndHandle);
    }

    wndHandle = nullptr;
}

bool KGV::System::WindowWin32::isWindowVisible() {
    // Returns true if window has the WS_VISIBLE style. Can be obsecured and will still return true.
    return IsWindowVisible((HWND)wndHandle);
}

void WindowWin32::showWindow(bool showState) {
    ShowWindow((HWND)wndHandle, showState);
    UpdateWindow((HWND)wndHandle);
}

void WindowWin32::resizeWindow(S32 width, S32 height) {
    // Resize the window but keep the x/y and z order.
    SetWindowPos((HWND)wndHandle, nullptr, 0, 0, width, height, SWP_NOMOVE | SWP_NOZORDER);

    // Make sure the show state is consistent with before.
    ShowWindow((HWND)wndHandle, isVisible);
    UpdateWindow((HWND)wndHandle);
}

void KGV::System::WindowWin32::setWindowCaption(const std::string &caption) {
    SetWindowTextA((HWND)wndHandle, caption.c_str());
    UpdateWindow((HWND)wndHandle);
}

void WindowWin32::repositionWindow(S32 xPos, S32 yPos) {
    // Reposition the window but keep the width/height and z order.
    SetWindowPos((HWND)wndHandle, nullptr, xPos, yPos, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

    // Make sure the show state is consistent with before.
    ShowWindow((HWND)wndHandle, isVisible);
    UpdateWindow((HWND)wndHandle);
}

void* KGV::System::WindowWin32::getWin32Handle() {
    return wndHandle;
}

bool KGV::System::WindowWin32::registerClass(WNDCLASSEX &wc) {
    wc.lpfnWndProc = internalWndProc; // Set the proper procedure.

    if (!RegisterClassEx(&wc)) {
        spdlog::get("engine")->error("Failed to register window! Error code: {}", GetLastError());
        return false;
    }

    spdlog::get("engine")->info("Successfully registered window!");

    return true;
}

#endif