//------------------------------------------------------------------------------
// 2015 J. Coelho.
// Platform: C++11
#include <murkyFramework/src/private/pch.hpp>
#include <murkyFramework/include/version.hpp>
#include <murkyFramework/include/GfxDevice/version_gfxDevice.hpp>
#ifdef USE_OPENGL
#include <glew/include/GL/glew.h> 
#include <glew/include/GL/wglew.h>
#include <murkyFramework/src/GfxDevice/public/gfxDevice.hpp>
#include <murkyFramework/include/GfxDevice/vertexBuffer.hpp>
#include <murkyFramework/include/GfxDevice/shaders.hpp>

namespace GfxDevice
{
	// forward declarations of external stuff  
		    
    // constructor	
    VertexBufferWrapper::VertexBufferWrapper(
        VertexType vertexType, PrimativeType primativeType,
		ShaderWrapper shaderId, TextureWrapper texture,
        u32 nVerts) :
        vertexType(vertexType), primativeType(primativeType), 		
		shaderId(shaderId), texture(texture),
        capacity(nVerts)
    {				        		
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
				
        glUseProgram( shaderId.value );

        // layout
        int szVertex = sizeof(Vert_pct);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, szVertex, 0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, szVertex, (void*)(sizeof(vec3)));//col      
        glEnableVertexAttribArray(1);

        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, szVertex, (void*)(sizeof(vec3) + sizeof(vec3)));//tex        
        glEnableVertexAttribArray(2);

        //f32 dummy[10000];
        //glBufferData(GL_ARRAY_BUFFER, sizeof(dummy), dummy, GL_DYNAMIC_DRAW); // unsure; test
        glBufferData(GL_ARRAY_BUFFER, 0, NULL, GL_DYNAMIC_DRAW); // unsure; test

        // reset state		
        glBindBuffer(GL_ARRAY_BUFFER, 0); 
        glBindVertexArray( 0 );

        glUseProgram(0);// unsure; test

        onGfxDeviceErrorTriggerBreakpoint();
    }

    // methods
    void	VertexBufferWrapper::draw( void *data, u32 nPrimatives )
    {
        if (nPrimatives >= capacity)
            triggerBreakpoint();

        onGfxDeviceErrorTriggerBreakpoint();
   
        if (nPrimatives == 0)
            return;

        u32 sizeVertex=0;	
        switch( vertexType )
        {
        case VertexType::posCol:
            sizeVertex = sizeof(Vert_pc);
            triggerBreakpoint();
            break;
        case VertexType::posColTex:
            sizeVertex = sizeof(Vert_pct);
            break;
        default:// Catch usage of unimplemented			
            sizeVertex = 0;	
            triggerBreakpoint();
        }

        int nVerticiesPerPrimative=0;
        GLuint glPrimativeType = 0;

        switch (primativeType)
        {
        case PrimativeType::triangle:
            nVerticiesPerPrimative = 3;
            glPrimativeType = GL_TRIANGLES;
            break;

        case PrimativeType::line:
            nVerticiesPerPrimative = 2;
            glPrimativeType = GL_LINES;
            break;
        default:// Catch usage of unimplemented			            
            triggerBreakpoint();
        }

        //Vert_pct *d = static_cast<Vert_pct*>(data);

        glBindVertexArray(vao);		

        glUseProgram(shaderId.value);
        glUniform1i(Shaders::uniforms_textureSamplerID, 0);

        glBindTexture( GL_TEXTURE_2D, texture.value );//is this already bound to vao???
        onGfxDeviceErrorTriggerBreakpoint();

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        //glBufferSubData(GL_ARRAY_BUFFER, 0, nPrimatives*nVerticiesPerPrimative*sizeVertex, data);
        glBufferData(GL_ARRAY_BUFFER, nPrimatives*nVerticiesPerPrimative*sizeVertex, data, GL_DYNAMIC_DRAW);

        onGfxDeviceErrorTriggerBreakpoint();
        // change data

        glDrawArrays(glPrimativeType, 0, nPrimatives*nVerticiesPerPrimative);		
        onGfxDeviceErrorTriggerBreakpoint();

        //reset state
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glBindTexture( GL_TEXTURE_2D, 0 );
        //glUseProgram( 0 );
        //glBindVertexArray( 0 );
        onGfxDeviceErrorTriggerBreakpoint();

    }
} 
#endif USE_OPENGL

  