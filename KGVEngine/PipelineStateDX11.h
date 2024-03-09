//
// Created by Brett on 2024-03-08.
//

#ifndef ENGINE_PIPELINESTATEDX11_H
#define ENGINE_PIPELINESTATEDX11_H

#include "pch.h"
#include "InputAssemblerStateDX11.h"
#include "ShaderStageStateDX11.h"
#include "RasterizerStageStateDX11.h"
#include "OutputMergerStageStateDX11.h"

namespace KGV::Render {
    class PipelineStateDX11 {
    public:
        InputAssemblerStateDX11* iaState;
        ShaderStageStateDX11* vsState;
        RasterizerStageStateDX11* rsState;
        ShaderStageStateDX11* psState;
        OutputMergerStageStateDX11* omState;
    };
}

#endif //ENGINE_PIPELINESTATEDX11_H
