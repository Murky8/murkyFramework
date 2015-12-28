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
            GfxDevice::GfxDeviceObj *gfxDevice;
            TextRender              *textRenderer;                        
            CollectionNamed<Model> modelManager;

            // test
            std::vector<Vert_pct> gdeb2_vertices;// todo: remove
            std::vector<u16> gdeb2_indices;// todo: remove
            GfxDevice::VertexIndexBufferWrapper *vibuffer;// todo: remove
            
        };
    }//namespace Render
}//namespace murkyFramework