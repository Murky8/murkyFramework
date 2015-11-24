#pragma once
//------------------------------------------------------------------------------
// 2015 J. Coelho.

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
