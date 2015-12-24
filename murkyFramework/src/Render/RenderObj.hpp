#pragma once
//------------------------------------------------------------------------------
// 2015 J. Coelho.
// Platform: C++11
namespace murkyFramework {
    namespace Render {
    

        class RenderObj
        {
        public:
            // construction
            RenderObj() = delete;
            RenderObj(GfxDevice::GfxDeviceObj_initStruct  *const initStruct);
            // destruction
            ~RenderObj();

            // methods    
            void drawAll();

            // objects/components
            GfxDevice::GfxDeviceObj *gfxDevice;
            TextRender              *textRenderer;            

            // test
            std::vector<Vert_pct> gdeb2_vertices;// todo: remove
            std::vector<u16> gdeb2_indices;// todo: remove

            GfxDevice::VertexIndexBufferWrapper *vibuffer;

            // data
            //vec4 cursorPos{ 0, 0, -5.f };
            //mat3 cursorOri{ unit };
        };
    }//namespace Render
}//namespace murkyFramework