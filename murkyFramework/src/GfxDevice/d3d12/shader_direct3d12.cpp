//------------------------------------------------------------------------------
// 2015 J. Coelho.
// Platform: C++11
#include <murkyFramework/src/pch.hpp>

namespace murkyFramework {
#define deviceObj  g_appDebug->render->gfxDevice

    namespace GfxDevice
    {
        using namespace DirectX;

        void setUniform_projectionMatrix(const float *pMat)
        {
        }
     
        void	Shaders::initialise()
        {      
            deviceObj->loadShadersInDir(L"src/GfxDevice/d3d12/shaders");
        }

        void	Shaders::deinitialise()
        {
        }
    
}//namespace GfxDevice
}//namespace murkyFramework
