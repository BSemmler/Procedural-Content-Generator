#include "Window.h"

using namespace KGV::System;

Window::Window(S32 width, S32 height, S32 xPos, S32 yPos, const std::string &caption, bool showWindow) {
    this->width = width;
    this->height = height;
    this->xPos = xPos;
    this->yPos = yPos;
    this->windowCaption = caption;
    this->isVisible = showWindow;
}

Window::~Window() {
}

S32 Window::getWidth() {
    return width;
}

S32 Window::getHeight() {
    return height;
}

S32 Window::getXPos() {
    return xPos;
}

S32 Window::getYPos() {
    return yPos;
}

void Window::setWindowCaption(const std::string &caption) {
    windowCaption = caption;
}

const std::string &Window::getWindowCaption() {
    return windowCaption;
}
