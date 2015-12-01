#pragma once
//------------------------------------------------------------------------------
// 2015 J. Coelho.


namespace GfxDevice
{        
    namespace Shaders
    {                    
        void	initialise();
        void	deinitialise();
    }
    
    void setUniform_projectionMatrix(const float *pMat);
}
