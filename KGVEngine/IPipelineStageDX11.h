//
// Created by Brett on 2024-03-06.
//

#ifndef ENGINE_IPIPELINESTAGEDX11_H
#define ENGINE_IPIPELINESTAGEDX11_H

#include "pch.h"
#include "RenderDeviceDX11.h"

namespace KGV::Render {
    class IPipelineStageDX11 {
        virtual void applyDesiredState(ComPtr<ID3D11DeviceContext> context, RenderDeviceDX11* device) = 0;
    };
}



#endif //ENGINE_IPIPELINESTAGEDX11_H
