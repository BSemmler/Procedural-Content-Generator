//
// Created by Brett on 2024-02-19.
//
#include "SwapChainConfigDX11.h"

namespace KGV::Render {
    int SwapChainConfigDX11::getWidth() const {
        return width;
    }

    void SwapChainConfigDX11::setWidth(int _width) {
        SwapChainConfigDX11::width = _width;
    }

    int SwapChainConfigDX11::getHeight() const {
        return height;
    }

    void SwapChainConfigDX11::setHeight(int _height) {
        SwapChainConfigDX11::height = _height;
    }

    int SwapChainConfigDX11::getBufferCount() const {
        return bufferCount;
    }

    void SwapChainConfigDX11::setBufferCount(int _bufferCount) {
        SwapChainConfigDX11::bufferCount = _bufferCount;
    }

    int SwapChainConfigDX11::getSampleCount() const {
        return sampleCount;
    }

    void SwapChainConfigDX11::setSampleCount(int _sampleCount) {
        SwapChainConfigDX11::sampleCount = _sampleCount;
    }

    int SwapChainConfigDX11::getSampleQuality() const {
        return sampleQuality;
    }

    void SwapChainConfigDX11::setSampleQuality(int _sampleQuality) {
        SwapChainConfigDX11::sampleQuality = _sampleQuality;
    }

    DXGI_FORMAT SwapChainConfigDX11::getFormat() const {
        return format;
    }

    void SwapChainConfigDX11::setFormat(DXGI_FORMAT _format) {
        SwapChainConfigDX11::format = _format;
    }

    DXGI_USAGE SwapChainConfigDX11::getUsage() const {
        return usage;
    }

    void SwapChainConfigDX11::setUsage(DXGI_USAGE _usage) {
        SwapChainConfigDX11::usage = _usage;
    }

    DXGI_ALPHA_MODE SwapChainConfigDX11::getAlphaMode() const {
        return alphaMode;
    }

    const DXGI_SWAP_CHAIN_DESC1 &SwapChainConfigDX11::getDesc() const {
        DXGI_SWAP_CHAIN_DESC1 desc;
        desc.Width = width;
        desc.Height = height;
        desc.AlphaMode = alphaMode;
        desc.BufferCount = bufferCount;
        desc.BufferUsage = usage;
        desc.Flags = 0;
        desc.SampleDesc.Count = sampleCount;
        desc.SampleDesc.Quality = sampleQuality;
        desc.Format = format;
        desc.Stereo = false;
        desc.SwapEffect = swapEffect;
        desc.Scaling = scaling;
    }

    void SwapChainConfigDX11::setScaling(DXGI_SCALING scaling) {
        SwapChainConfigDX11::scaling = scaling;
    }

    DXGI_SCALING SwapChainConfigDX11::getScaling() const {
        return scaling;
    }

    void SwapChainConfigDX11::setSwapEffect(DXGI_SWAP_EFFECT _swapEffect) {
        SwapChainConfigDX11::swapEffect = _swapEffect;
    }

    DXGI_SWAP_EFFECT SwapChainConfigDX11::getSwapEffect() const {
        return swapEffect;
    }

    void SwapChainConfigDX11::setAlphaMode(DXGI_ALPHA_MODE _alphaMode) {
        SwapChainConfigDX11::alphaMode = _alphaMode;
    }

    SwapChainConfigDX11::SwapChainConfigDX11() {
        width = 0;
        height = 0;
        alphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
        bufferCount = 2;
        usage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        format = DXGI_FORMAT_R8G8B8A8_UNORM;
        scaling = DXGI_SCALING_NONE;
        sampleQuality = 0;
        sampleCount = 1;
        swapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    }
}