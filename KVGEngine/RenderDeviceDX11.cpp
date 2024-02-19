#include "RenderDeviceDX11.h"

using namespace KGV::Util;

namespace KGV::Render {
    bool RenderDeviceDX11::init() {
        return false;
    }

    Texture2dDX11 *RenderDeviceDX11::createTexture2D(Texture2dConfigDX11 &config, ResourceData &data, ResourceType type) {
        auto *texture = new Texture2dDX11();
        HRESULT hr = device->CreateTexture2D(&config.desc, reinterpret_cast<D3D11_SUBRESOURCE_DATA *>(&data), &texture->texture);

        if (FAILED(hr)) {
            logger.error("Failed to create D3D11 Texture2D");

            // Trigger a breakpoint if we're in a debug build.
            KGV_debugBreak();

            delete texture;
            texture = nullptr;
        }

        if (texture) {
            texture->actualDesc = config.desc;
            texture->desiredDesc = texture->actualDesc;
        }

        return texture;
    }

    BufferDX11 *RenderDeviceDX11::createBuffer(BufferConfigDX11 &config, ResourceData &data, ResourceType type) {
        auto *buff = new BufferDX11();
        HRESULT hr = device->CreateBuffer(&config.desc, reinterpret_cast<D3D11_SUBRESOURCE_DATA *>(&data), &buff->buffer);

        if (FAILED(hr)) {
            logger.error("Failed to create D3D11 Buffer!");
            // Trigger a breakpoint if we're in a debug build.
            KGV_debugBreak();
            delete buff;
            buff = nullptr;
        }

        if (buff) {
            buff->actualDesc = config.desc;
            buff->desiredDesc = buff->actualDesc;
        }

        return buff;
    }
}