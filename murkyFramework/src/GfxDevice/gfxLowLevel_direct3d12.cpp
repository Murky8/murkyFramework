//------------------------------------------------------------------------------
// 2015 J. Coelho.
// Platform: C++11
#include <murkyFramework/include/version.hpp>
#include <murkyFramework/include/GfxDevice/version_gfxDevice.hpp>
#ifdef USE_DIRECT3D12

#include <windows.h>
#include <murkyFramework/include/vectorMatrix.hpp>
#include <murkyFramework/include/GfxDevice/gfxLowLevel.hpp>
#include <murkyFramework/include/GfxDevice/gfxPrimativeTypes.hpp>
#include <murkyFramework/include/GfxDevice/vertexBuffer.hpp>
#include <murkyFramework/include/GfxDevice/shaders.hpp>
#include <murkyFramework/include/GfxDevice/texture.hpp>

// forward declarations
namespace Render  // todo: delete
{        
    extern GfxDevice::TextureManager *textureManager;
}

namespace GfxDevice
{       
    struct HandleDeviceTexture
    {
        ID3D11ShaderResourceView *deviceTexture;
    };

    HDC     hDC;   // a copy from main.cpp
    HGLRC   hRC;
    HWND    hWnd;
  
    // data
    mat4 projectionMatrix(unit);    

    void onGfxDeviceErrorTriggerBreakpoint()
    {            
    }    

    void setStateToAppDefault()
    {
    }        


    void drawBegin()
    {     
    }

    void drawEnd()
    {     
    }        
}
#endif // USE_DIRECT3D11
    