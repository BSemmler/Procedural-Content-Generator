//
// Created by Brett on 2024-03-08.
//

#ifndef ENGINE_PIPELINESTATEDX11_H
#define ENGINE_PIPELINESTATEDX11_H

#include "pch.h"

namespace KGV::Render {
    class InputAssemblerStateDX11;
    class ShaderStageStateDX11;
    class RasterizerStageStateDX11;
    class OutputMergerStageStateDX11;

    class PipelineStateDX11 {
    public:
        PipelineStateDX11(InputAssemblerStateDX11 *iaState = nullptr,
                          ShaderStageStateDX11 *vsState = nullptr,
                          RasterizerStageStateDX11 *rsState = nullptr,
                          ShaderStageStateDX11 *psState = nullptr,
                          OutputMergerStageStateDX11 *omState = nullptr) : iaState(iaState),
                                                                            vsState(vsState),
                                                                            rsState(rsState),
                                                                            psState(psState),
                                                                            omState(omState) {}

        InputAssemblerStateDX11 *iaState;
        ShaderStageStateDX11 *vsState;
        RasterizerStageStateDX11 *rsState;
        ShaderStageStateDX11 *psState;
        OutputMergerStageStateDX11 *omState;
    };
}

#endif //ENGINE_PIPELINESTATEDX11_H
