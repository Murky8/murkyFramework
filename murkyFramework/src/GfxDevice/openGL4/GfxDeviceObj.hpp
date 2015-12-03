#pragma once
//------------------------------------------------------------------------------
// 2015 J. Coelho.
// Platform: C++11. openGL4
namespace murkyFramework {
namespace GfxDevice
{
    void onGfxDeviceErrorTriggerBreakpoint();
}

struct GfxDeviceObj_initStruct
{
    u32 screenWidth;
    u32 screenHeight;
    WindowsSpecific* windowsSpecific;
};

class GfxDeviceObj
{
public:
    GfxDeviceObj() = delete;

    GfxDeviceObj(GfxDeviceObj_initStruct  *const initStruct);
    ~GfxDeviceObj();

    CollectionNamed<GfxDevice::ShaderWrapper>		shaderManager;
    CollectionNamed<GfxDevice::TextureWrapper>		textureManager;
    CollectionNamed<GfxDevice::VertexBufferWrapper>	vertexBufferManager;

    mat4    projectionMat{ unit };
    void setUniform_projectionMatrix(const float *pMat);

    HDC const hDC;

    void drawBegin();
    void drawEnd();
};
}//namespace murkyFramework