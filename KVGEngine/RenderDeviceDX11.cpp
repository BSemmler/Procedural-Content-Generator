#include "RenderDeviceDX11.h"

using namespace KGV::Util;

namespace KGV::Render
{
    bool RenderDeviceDX11::init()
    {
        return false;
    }

    Texture2dDX11* RenderDeviceDX11::createTexture2D( Texture2dConfigDX11* config, ResourceData* data, ResourceType type )
    {
        Texture2dDX11* texture = new Texture2dDX11();
        HRESULT hr = device->CreateTexture2D( &config->desc, ( D3D11_SUBRESOURCE_DATA* ) &data, &texture->texture );

        if (FAILED( hr ))
        {
            g_log->writeToLog( LogVerbosity::kError, LogChannel::kRender, "%s\n", "Failed to create D3D11 buffer" );

            // Trigger a breakpoint if we're in a debug build.
            KGV_debugBreak();

            if (texture)
            {
                delete texture;
                texture = nullptr;
            }
        }

        texture->actualDesc = config->desc;
        texture->desiredDesc = texture->actualDesc;

        return texture;
    }

    BufferDX11* RenderDeviceDX11::createBuffer( BufferConfigDX11* config, ResourceData* data, ResourceType type )
    {
        KGV_ASSERT_TRUE( config );

        BufferDX11* buff = new BufferDX11();
        HRESULT hr = device->CreateBuffer( &config->desc, ( D3D11_SUBRESOURCE_DATA* ) &data, &buff->buffer );

        if ( FAILED( hr ) )
        {
            g_log->writeToLog( LogVerbosity::kError, LogChannel::kRender, "%s\n", "Failed to create D3D11 buffer" );

            // Trigger a breakpoint if we're in a debug build.
            KGV_debugBreak();
            
            if (buff)
            {
                delete buff;
                buff = nullptr;
            }
        }

        buff->actualDesc = config->desc;
        buff->desiredDesc = buff->actualDesc;

        return buff;
    }
}
