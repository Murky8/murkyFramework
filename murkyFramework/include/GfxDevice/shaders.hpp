#pragma once
//------------------------------------------------------------------------------
// 2015 J. Coelho.
#include <murkyFramework/include/GfxDevice/version_gfxDevice.hpp>

#include <vector>
#include <string>

#include <murkyFramework/include/types.hpp>
#include <murkyFramework/include/vectorMatrix.hpp>
#include <murkyFramework/include/GfxDevice/gfxLowLevel.hpp>

namespace GfxDevice
{                    
    namespace Shaders
    {    
        extern	u32	uniforms_textureSamplerID;		
        extern	u32	uniformHandle_projectionMatrix;        
        void	initialise();
        void	deinitialise();
    }
    void setUniform_projectionMatrix(const float *pMat);
}
