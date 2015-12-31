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

            void loadAllModels();
            void loadResources();

            // objects/components
            mat4 cameraPosOri = { unit };
            GfxDevice::GfxDeviceObj *gfxDevice;
            TextRender              *textRenderer;                        
            CollectionNamed<Model> modelManager;

            std::vector<Line_pc> defaultLines;
            // test
            //std::vector<Vert_pct> gdeb2_vertices;// todo: remove
            //std::vector<u16> gdeb2_indices;// todo: remove
            GfxDevice::VertexIndexBufferWrapper *vibuffer; //use collection map
            
        };
    }//namespace Render
}//namespace murkyFramework