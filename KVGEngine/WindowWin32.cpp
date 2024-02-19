#include "WindowWin32.h"

#ifdef KGV_WIN


using namespace KGV::System;
using namespace KGV::Util;

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
    KGV_ASSERT_TRUE(width > 0 && height > 0 && xPos >= 0 && yPos >= 0);

    g_log->writeToLog(LogVerbosity::kInfo, LogChannel::kSystem,
                      "New Window is being instantiated. Caption: %s, Width: %d, Height: %d, Position: (%d,%d)",
                      caption.c_str(), width, height, xPos, yPos);

    isVisible = showWindow;


    wndHandle = CreateWindowEx(NULL,
                               reinterpret_cast<LPCSTR>(className.c_str()),
                               reinterpret_cast<LPCSTR>(std::wstring(windowCaption.begin(),
                                                                     windowCaption.end()).c_str()),
                               WS_OVERLAPPEDWINDOW,
                               xPos, yPos,
                               width, height,
                               nullptr,
                               nullptr,
                               nullptr,
                               nullptr);

    if (!wndHandle) {
        g_log->writeToLog(Util::LogVerbosity::kError, Util::LogChannel::kSystem, "%s %d",
                          "Failed to create window! Error code: ", GetLastError());
        return;
    }
    // Store our window procedure object inside the extra window bytes.
    SetWindowLongPtr(wndHandle, 0, reinterpret_cast< LONG_PTR >( procObj ));

    ShowWindow(wndHandle, isVisible);
    UpdateWindow(wndHandle);
}

WindowWin32::~WindowWin32() {
    // Cleanup window.
    if (wndHandle) {
        DestroyWindow(wndHandle);
    }

    wndHandle = nullptr;
}

bool KGV::System::WindowWin32::isWindowVisible() {
    // Returns true if window has the WS_VISIBLE style. Can be obsecured and will still return true.
    return IsWindowVisible(wndHandle);
}

void WindowWin32::showWindow(bool showState) {
    ShowWindow(wndHandle, showState);
    UpdateWindow(wndHandle);
}

void WindowWin32::resizeWindow(S32 width, S32 height) {
    // Resize the window but keep the x/y and z order.
    SetWindowPos(wndHandle, nullptr, 0, 0, width, height, SWP_NOMOVE | SWP_NOZORDER);

    // Make sure the show state is consistent with before.
    ShowWindow(wndHandle, isVisible);
    UpdateWindow(wndHandle);
}

void KGV::System::WindowWin32::setWindowCaption(const std::string &caption) {
    SetWindowTextA(wndHandle, caption.c_str());
    UpdateWindow(wndHandle);
}

void WindowWin32::repositionWindow(S32 xPos, S32 yPos) {
    // Reposition the window but keep the width/height and z order.
    SetWindowPos(wndHandle, nullptr, xPos, yPos, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

    // Make sure the show state is consistent with before.
    ShowWindow(wndHandle, isVisible);
    UpdateWindow(wndHandle);
}

HWND KGV::System::WindowWin32::getWin32Handle() {
    return wndHandle;
}

bool KGV::System::WindowWin32::registerClass(WNDCLASSEX &wc) {
    wc.lpfnWndProc = internalWndProc; // Set the proper procedure.

    if (!RegisterClassEx(&wc)) {
        g_log->writeToLog(Util::LogVerbosity::kError, Util::LogChannel::kSystem, "%s %d",
                          "Failed to register window class! Error code: ", GetLastError());
        return false;
    }

    g_log->writeToLog(Util::LogVerbosity::kError, Util::LogChannel::kSystem, "%s",
                      "Class successfully registered ");

    return true;
}

#endif