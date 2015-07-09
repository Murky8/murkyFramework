#pragma once
//------------------------------------------------------------------------------
// 2015 J. Coelho.
#include <murkyFramework/include/version.hpp>

#include <vector>
#include <string>

#include <murkyFramework/include/types.hpp>
#include <murkyFramework/include/vectorMatrix.hpp>
#include <murkyFramework/include/gfxLowLevel/gfxLowLevel.hpp>

namespace GfxLowLevel
{    
    class ShaderId;
    
    namespace Shaders
    {    
        extern	u32	uniforms_textureSamplerID;		
        extern	u32	uniformHandle_projectionMatrix;
        extern	ShaderId posColText;
        void	initialise();
        void	deinitialise();
    }

    class ShaderId
    {
    public:
        u32 getHandle() const { return handle; }
        u32 handle;
        u32 handle2;
    };        

    void setUniform_projectionMatrix(const mat4 *pMat);
}
