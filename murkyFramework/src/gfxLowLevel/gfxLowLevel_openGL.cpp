//------------------------------------------------------------------------------
// 2015 J. Coelho.
// Platform: C++11
#include <murkyFramework/include/version.hpp>
#ifdef USE_OPENGL

#include <vector>

#include <murkyFramework/include/appFramework.hpp>
#include <murkyFramework/include/types.hpp>  
#include <murkyFramework/include/gfxLowLevel/gfxPrimativeTypes.hpp>
#include <murkyFramework/include/gfxLowLevel/vertexBuffer.hpp>
#include <murkyFramework/include/gfxLowLevel/shaders.hpp>
#include <murkyFramework/include/gfxLowLevel/texture.hpp>

#include <murkyFramework/include/gfxLowLevel/gfxLowLevel.hpp>
#include <vector>

#include <external/glew/include/GL/glew.h> 
#include <external/glew/include/GL/wglew.h>

#define GLM_FORCE_RADIANS
#include <external/glm/glm.hpp>
#include <external/glm/vec2.hpp>
#include <external/glm/vec3.hpp>
#include <external/glm/vec4.hpp>
#include <external/glm/mat4x4.hpp>
#include <external/glm/gtc/matrix_transform.hpp>

//#include <gli/gli.hpp>

#include <murkyFramework/include/debugUtils.hpp>
#include <murkyFramework/include/system.hpp>
#include <murkyFramework/include/vectorMatrix.hpp>
#include <murkyFramework/include/appFramework.hpp>

namespace GfxLowLevel
{   
    
    // Forward declarations

    //extern ShaderProgram *shaderProgram_line_pc;    //TODO change
    //extern ShaderProgram *shaderProgram_posColText; //TODO change

    // Data
    mat4 projectionMatrix(Unit::UNIT);    
        
    //void serializeState();1

    void onGfxDeviceErrorTriggerBreakpoint()
    {
        GLenum err;
        bool stop = false;
        while(( err = glGetError() ) != GL_NO_ERROR)
        {
            std::wstring s;
            switch (err)
            {
            case 0x0500 : s = L"GL_INVALID_ENUM"; break;
            case 0x0501 : s = L"GL_INVALID_VALUE"; break;
            case 0x0502 : s = L"GL_INVALID_OPERATION"; break;
            case 0x0503 : s = L"GL_STACK_OVERFLOW"; break;
            case 0x0504 : s = L"GL_STACK_UNDERFLOW"; break;
            case 0x0505 : s = L"GL_OUT_OF_MEMORY"; break;
            default : s = L"" +err;
            }
            debugLog << L"GL error: " << s << L"\n";
            stop = true;
        }

        if( stop == true )
            triggerBreakpoint();
    }    

    void setStateToAppDefault()
    {
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
                
        //glEnable(GL_CULL_FACE);
        //glFrontFace(GL_CW);
        //glCullFace(GL_BACK);
    }

   

    void drawBegin()
    {
        glViewport(0, 0, Gapp.screenResX, Gapp.screenResY);

        glClearColor(0.4f, 0.6f, 0.9f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        onGfxDeviceErrorTriggerBreakpoint();	
    }

    /*void setShaderUniform_projMat(mat4 &projMat)
    {        
        glUniformMatrix4fv(GfxLowLevel::uniforms_projMatrix, 1, GL_FALSE, &(projMat.v[0][0]));    
    }
*/
    void drawEnd()
    {
        glFlush();
    }    
    
    /*
    void drawLines_pc(VertexBufferRef_Depreciate vertexBufferRef, std::vector<Line_pc> &lines)
    {
        onGfxDeviceErrorTriggerBreakpoint(); 

        // Check parameters
        if (vertexBufferRef.bufferAccessType != BufferAccessType::dynamic)
            triggerBreakpoint();

        if (vertexBufferRef.vertexType != VertexType::posCol)
            triggerBreakpoint();

        if(lines.size()*2 > vertexBufferRef.capacity)
            triggerBreakpoint();//nElements=vertex count not line count
        // Check parameters ^

        if (lines.size() == 0) 
            return;//GL breaks if try to draw 0 elements. '0' valid parameter though
     
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);        

        glBindBuffer(GL_ARRAY_BUFFER, vertexBufferRef.getBufferHandle());

        glBufferData(GL_ARRAY_BUFFER, lines.size() * sizeof(Line_pc), lines.data(), GL_DYNAMIC_DRAW);        

        int szVertex = sizeof(Vert_pc);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, szVertex, 0);//pos
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, szVertex, (void*)(sizeof(vec3)));//col      

        glDrawArrays(GL_LINES, 0, lines.size() * 2);

        glDisableVertexAttribArray(1);
        glDisableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);       

        lines.clear();             

        onGfxDeviceErrorTriggerBreakpoint();
    }

    void drawTriangles_pc(VertexBufferRef_Depreciate vertexBufferRef, std::vector<Triangle_pc> &triangles)
    {
        onGfxDeviceErrorTriggerBreakpoint();
        
        // Check parameters
        if (vertexBufferRef.bufferAccessType != BufferAccessType::dynamic)
            triggerBreakpoint();

        if (vertexBufferRef.vertexType != VertexType::posCol)
            triggerBreakpoint();

        if (triangles.size() * 3 > vertexBufferRef.capacity)
            triggerBreakpoint();//nElements=vertex count not tri count
        // Check parameters ^

        if (triangles.size() == 0)
            return;//GL breaks if try to draw 0 elemnts. '0' valid parameter though

        glUseProgram(shaderProgram_line_pc->getHandle());

        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ARRAY_BUFFER, vertexBufferRef.getBufferHandle());

        glBufferData(GL_ARRAY_BUFFER, triangles.size() * sizeof(Triangle_pc), triangles.data(), GL_DYNAMIC_DRAW);

        int szVertex = sizeof(Vert_pc);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, szVertex, 0);//pos
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, szVertex, (void*)(sizeof(vec3)));//col      

        glDrawArrays(GL_TRIANGLES, 0, triangles.size() * 3);// no of verts not triansgles

        glDisableVertexAttribArray(1);
        glDisableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);

        triangles.clear();

        glUseProgram(0);

        onGfxDeviceErrorTriggerBreakpoint();
    }
    
    void drawTriangles_pct(VertexBufferRef_Depreciate vertexBufferRef, std::vector<Triangle_pct> &triangles, TextureRef &texture)
    {
        onGfxDeviceErrorTriggerBreakpoint();
        
        // Check parameters
        if (triangles.size() == 0)
            return;//GL breaks if try to draw 0 elemnts. '0' valid parameter though

        if (vertexBufferRef.bufferAccessType != BufferAccessType::dynamic)
            triggerBreakpoint();

        if (vertexBufferRef.vertexType != VertexType::posColTex)
            triggerBreakpoint();

        if (triangles.size() * 3 > vertexBufferRef.capacity)
            triggerBreakpoint();//nElements=vertex count not tri count
        // Check parameters ^
        
        ShaderProgram &shader = *shaderProgram_posColText;
        
        glBindTexture(GL_TEXTURE_2D, texture.getHandle());
                
        glUseProgram(shader.getHandle() );        
        glUniform1i(GfxLowLevel::uniforms_texture, 0);

        if(0)
        {//26012015                          
            glm::mat4 projMatrix = glm::perspective(
                2.0f,   // The horizontal Field of View, in degrees : the amount of "zoom". Think "camera lens". Usually between 90° (extra wide) and 30° (quite zoomed in)
                1.0f,   // Aspect Ratio. Depends on the size of your window. Notice that 4/3 == 800/600 == 1280/960, sounds familiar ?
                0.1f,  // Near clipping plane. Keep as big as possible, or you'll get precision issues.
                2.0f);  // Far clipping plane. Keep as little as possible.
                
            projMatrix[2][0] *= -1.0f;
            projMatrix[2][1] *= -1.0f;
            projMatrix[2][2] *= -1.0f;
            projMatrix[2][3] *= -1.0f;

            //projMatrix[0][0] = 1.0f;
        }//26012015

        glUniformMatrix4fv(GfxLowLevel::uniforms_projMatrix, 1, GL_FALSE, &(projectionMatrix.v[0][0]));
        
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);
        
        glBindBuffer(GL_ARRAY_BUFFER, vertexBufferRef.getBufferHandle());
        
        glBufferData(GL_ARRAY_BUFFER, triangles.size() * sizeof(Triangle_pct), triangles.data(), GL_DYNAMIC_DRAW);
        
        int szVertex = sizeof(Vert_pct);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, szVertex, 0);//pos
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, szVertex, (void*)(sizeof(vec3)));//col      
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, szVertex, (void*)(sizeof(vec3)+sizeof(vec3)));//tex        
        onGfxDeviceErrorTriggerBreakpoint();

        glDrawArrays(GL_TRIANGLES, 0, triangles.size() * 3);// no of verts not triansgles        		
        onGfxDeviceErrorTriggerBreakpoint();


        glDisableVertexAttribArray(2);
        glDisableVertexAttribArray(1);
        glDisableVertexAttribArray(0);
        
        glBindBuffer(GL_ARRAY_BUFFER, 0);      
        triangles.clear();
        glUseProgram(0);
        glBindTexture(GL_TEXTURE_2D, 0);        
        onGfxDeviceErrorTriggerBreakpoint();
    }
    */
}

#endif // USE_OPENGL
