//
// Created by Brett on 2024-03-06.
//

#ifndef ENGINE_SHADERSTAGESTATEDX11_H
#define ENGINE_SHADERSTAGESTATEDX11_H

#include "pch.h"

namespace KGV::Render {
    class ShaderStageStateDX11 {
    public:
        S32 getShaderId() const;

        void setShaderId(S32 shaderId);

        const std::vector<S32> &getConstantBuffersIds() const;

        void setConstantBuffersIds(const std::vector<S32> &constantBuffersIds);

        const std::vector<S32> &getSrvIds() const;

        void setSrvIds(const std::vector<S32> &srvIds);

    protected:
        S32 shaderId;
        std::vector<S32> constantBuffersIds;
        std::vector<S32> srvIds;
    };
}



#endif //ENGINE_SHADERSTAGESTATEDX11_H
