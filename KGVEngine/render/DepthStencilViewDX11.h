//
// Created by Brett on 3/14/2024.
//

#ifndef ENGINE_DEPTHSTENCILVIEWDX11_H
#define ENGINE_DEPTHSTENCILVIEWDX11_H

#include "pch.h"

namespace KGV::Render {
    class DepthStencilViewDX11 {
    public:
        explicit DepthStencilViewDX11(const ComPtr<ID3D11DepthStencilView> &view);

        const ComPtr<ID3D11DepthStencilView> &getView() const;

        void setView(const ComPtr<ID3D11DepthStencilView> &view);

    protected:
        ComPtr<ID3D11DepthStencilView> view;
    };
}



#endif //ENGINE_DEPTHSTENCILVIEWDX11_H
