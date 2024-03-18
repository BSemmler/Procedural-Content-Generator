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

        void setConstantBuffersIds(const std::vector<S32> &constantBuffersIds, S32 startSlot = 0);

        const std::vector<S32> &getSrvIds() const;

        void setSrvIds(const std::vector<S32> &srvIds);

        S32 getCbufferStartSlot() const;

        void setCbufferStartSlot(S32 cbufferStartSlot);

        const std::vector<S32> &getSamplerIds() const;

        void setSamplerIds(const std::vector<S32> &samplerIds);

    protected:
        S32 shaderId;
        S32 cbufferStartSlot;
        std::vector<S32> constantBuffersIds;
        std::vector<S32> srvIds;
        std::vector<S32> samplerIds;
    };
}



#endif //ENGINE_SHADERSTAGESTATEDX11_H
