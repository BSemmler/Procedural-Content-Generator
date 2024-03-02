#pragma once

#include "pch.h"

namespace KGV::Render {
    class SwapChainConfigDX11 {
    public:
        SwapChainConfigDX11();

        int getWidth() const;

        void setWidth(int _width);

        int getHeight() const;

        void setHeight(int _height);

        int getBufferCount() const;

        void setBufferCount(int _bufferCount);

        int getSampleCount() const;

        void setSampleCount(int _sampleCount);

        int getSampleQuality() const;

        void setSampleQuality(int _sampleQuality);

        DXGI_FORMAT getFormat() const;

        void setFormat(DXGI_FORMAT _format);

        DXGI_USAGE getUsage() const;

        void setUsage(DXGI_USAGE _usage);

        DXGI_ALPHA_MODE getAlphaMode() const;

        void setAlphaMode(DXGI_ALPHA_MODE _alphaMode);

        DXGI_SWAP_EFFECT getSwapEffect() const;

        void setSwapEffect(DXGI_SWAP_EFFECT _swapEffect);

        DXGI_SCALING getScaling() const;

        void setScaling(DXGI_SCALING scaling);

        const DXGI_SWAP_CHAIN_DESC1 &getDesc() const;

    protected:
        int width = 0;
        int height = 0;
        int bufferCount = 0;
        int sampleCount;
        int sampleQuality;
        DXGI_FORMAT format;
        DXGI_USAGE usage;
        DXGI_ALPHA_MODE alphaMode;
        DXGI_SWAP_EFFECT swapEffect;
        DXGI_SCALING scaling;

        friend class RenderDeviceDX11;
    };


}