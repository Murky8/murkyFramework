//------------------------------------------------------------------------------
// 2014 J. Coelho.
//
// Wrapper is agnostic
// Platform: OpenGL 4.x version
// TODO remove buffer access type? static/dynamic buffers have completely different usage?

#include <vector>
#include <glew/include/GL/glew.h> 
#include <glew/include/GL/wglew.h>

#include <gfxLowLevel/gfxPrimativeTypes.hpp>
#include <gfxLowLevel/vertexBuffer.hpp>
#include <gfxLowLevel/shaders.hpp>

// forward declarations


namespace GfxLowLevel
{
    void onGfxDeviceErrorTriggerBreakpoint();

    // constructor	
    VertexBufferDynamic::VertexBufferDynamic(
        VertexType vertexType, PrimativeType primativeType, ShaderId shaderProgram, GfxLowLevel::TextureId &texture ) :
        vertexType(vertexType), primativeType(primativeType), shaderProgram(shaderProgram), texture(texture)
    {				
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);
    
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);		
        
        glUseProgram( shaderProgram.getHandle() );
    
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
    void	VertexBufferDynamic::draw( void *data, int nPrimatives )
    {
        onGfxDeviceErrorTriggerBreakpoint();
        static int c = 0;
        c++;
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
        default:// Catch usage of unimplemented			            
            triggerBreakpoint();
        }

        //Vert_pct *d = static_cast<Vert_pct*>(data);
        
        glBindVertexArray(vao);		

        glUseProgram(shaderProgram.getHandle());
        glUniform1i(Shaders::uniforms_textureSamplerID, 0);
        
        glBindTexture( GL_TEXTURE_2D, texture.getHandle() );//is this already bound to vao???
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
    /*
    VertexBufferRef_Depreciate::VertexBufferRef_Depreciate(u32 in_capacity, VertexType vertexType, BufferAccessType bufferAccessType) :
        capacity(in_capacity), vertexType(vertexType), bufferAccessType(bufferAccessType)
    {
        glGenBuffers(1, (GLuint*)&bufferHandle);
        onGfxDeviceErrorTriggerBreakpoint();

        glBindBuffer(GL_ARRAY_BUFFER, bufferHandle);
        onGfxDeviceErrorTriggerBreakpoint();
        u32 sizeVertex;
        switch (vertexType)
        {
        case VertexType::posCol :
            sizeVertex = sizeof(Vert_pc);
            break;
        case VertexType::posColTex:
            sizeVertex = sizeof(Vert_pct);
            break;
        default:// Catch usage of unimplemented
            sizeVertex = 0;
            triggerBreakpoint();
        }

        GLuint gl_bufferAccessType;
        switch (bufferAccessType)
        {
        case BufferAccessType::dynamic:
            gl_bufferAccessType = GL_DYNAMIC_DRAW;
            break;
        default:// Catch usage of unimplemented
            gl_bufferAccessType = 0;
            triggerBreakpoint();
        }

        glBufferData(GL_ARRAY_BUFFER, capacity * sizeVertex, NULL, gl_bufferAccessType);
        onGfxDeviceErrorTriggerBreakpoint();

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        onGfxDeviceErrorTriggerBreakpoint();
    }

    u32 VertexBufferRef_Depreciate::getBufferHandle() const 
    {
        return bufferHandle;
    }
    */
}