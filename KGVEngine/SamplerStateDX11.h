//
// Created by Brett on 2024-03-16.
//

#ifndef ENGINE_SAMPLERSTATEDX11_H
#define ENGINE_SAMPLERSTATEDX11_H

#include <d3d11.h>

namespace KGV::Render {
    class SamplerStateDX11 : D3D11_SAMPLER_DESC {
    public:
        void setDefaults();
    protected:

    };
}


#endif //ENGINE_SAMPLERSTATEDX11_H
