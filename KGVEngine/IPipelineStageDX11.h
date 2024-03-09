//
// Created by Brett on 2024-03-06.
//

#ifndef ENGINE_IPIPELINESTAGEDX11_H
#define ENGINE_IPIPELINESTAGEDX11_H

#include "pch.h"

namespace KGV::Render {
    class RenderDeviceDX11;

    class IPipelineStageDX11 {
    public:
        virtual void applyDesiredState(ComPtr<ID3D11DeviceContext> context, RenderDeviceDX11* device) = 0;
    };
}



#endif //ENGINE_IPIPELINESTAGEDX11_H
