//
// Created by Brett on 2024-03-06.
//

#ifndef ENGINE_INPUTASSEMBLERSTATEDX11_H
#define ENGINE_INPUTASSEMBLERSTATEDX11_H

#include "pch.h"
#include "VertexBufferDX11.h"
#include "IndexBufferDX11.h"

namespace KGV::Render {
    class InputAssemblerStateDX11 {
    public:
        static constexpr S32 maxSlots = 16;

        S32 getAvailableSlots() const;

        const std::vector<S32> &getVertexBuffers() const;

        void setVertexBuffers(const std::vector<S32> &vertexBuffers);

        const std::vector<S32> &getStrides() const;

        void setStrides(const std::vector<S32> &strides);

        const std::vector<S32> &getOffsets() const;

        void setOffsets(const std::vector<S32> &offsets);

        S32 getInputLayout() const;

        void setInputLayout(S32 inputLayout);

        S32 getIndexBuffer() const;

        void setIndexBuffer(S32 indexBuffer);

        DXGI_FORMAT getIndexFormat() const;

        void setIndexFormat(DXGI_FORMAT indexFormat);

        D3D11_PRIMITIVE_TOPOLOGY getTopology() const;

        void setTopology(D3D11_PRIMITIVE_TOPOLOGY topology);

    protected:
        std::vector<S32> vertexBuffers;
        std::vector<S32> strides;
        std::vector<S32> offsets;
        S32 inputLayout;
        S32 indexBuffer;
        DXGI_FORMAT indexFormat;
        D3D11_PRIMITIVE_TOPOLOGY topology;
    };
}



#endif //ENGINE_INPUTASSEMBLERSTATEDX11_H
