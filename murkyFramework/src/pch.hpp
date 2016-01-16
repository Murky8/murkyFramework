//------------------------------------------------------------------------------
// 2015 J. Coelho

#pragma message("Compiling precompiled headers.\n")
//#include <sal.h>    // special

#include <combaseapi.h>
#include <stdlib.h>
#include <windows.h>
#include <wchar.h>
#include <winuser.h>
#include <wrl.h>

#include <array>
#include <chrono>
#include <cmath>
#include <codecvt>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <limits>
#include <list>
#include <map>
#include <memory>
#include <mutex>
#include <ratio>
#include <regex> 
#include <string>
#include <thread>
#include <vector>

// third party
#include <ThirdParty/boost/circular_buffer.hpp>
#include <ThirdParty/boost/multi_array.hpp>
#include <ThirdParty/lodepng.h>
//#include <ThirdParty/fmod/fmod.hpp>
#include "hidusage.h"
// third party ^


#include <murkyFramework/src/version.hpp>
#include <murkyFramework/src/types.hpp>
#include <murkyFramework/src/common.hpp>

#include <murkyFramework/src/appFramework.hpp>

#include <murkyFramework/src/SystemSpecific/systemSpecific.hpp>
#include <murkyFramework/src/SystemSpecific/Windows/WindowsSpecific.hpp>

#include <murkyFramework/src/collectionNamed.hpp>
#include <murkyFramework/src/debugUtils.hpp>
#include <murkyFramework/src/fileUtils.hpp>
#include <murkyFramework/src/loadSaveFile.hpp>
#include <murkyFramework/src/vectorMatrix.hpp>
#include <murkyFramework/src/vectorMatrix_rotation.hpp>
#include <murkyFramework/src/randomNumbers.hpp>
#include <murkyFramework/src/inputDevices.hpp>
#include <murkyFramework/src/Audio/Audio.hpp>
#include <murkyFramework/src/formula.hpp>

#include <murkyFramework/src/Game/EntityBase.hpp>
#include <murkyFramework/src/Game/Game.hpp>
//#include <murkyFramework/src/octree.hpp>

// gfxDevice
#include <murkyFramework/src/GfxDevice/version_gfxDevice.hpp>
#include <murkyFramework/src/GfxDevice/GfxDeviceCommon.hpp>
#include <murkyFramework/src/GfxDevice/GfxDeviceObjCommon.hpp>
#include <murkyFramework/src/GfxDevice/gfxLowLevel.hpp>
#include <murkyFramework/src/GfxDevice/gfxPrimativeTypes.hpp>
#include <murkyFramework/src/GfxDevice/shaders.hpp>
#include <murkyFramework/src/GfxDevice/textureHelpers.hpp>
#include <murkyFramework/src/GfxDevice/vertexBufferHelpers.hpp>

#ifdef USE_OPENGL //todo: separate pch?
    #define GLM_FORCE_RADIANS
    #include <ThirdParty/glew/include/GL/glew.h> 
    #include <ThirdParty/glew/include/GL/wglew.h>
    #include <ThirdParty/glm/glm.hpp>
    #include <ThirdParty/glm/vec2.hpp>
    #include <ThirdParty/glm/vec3.hpp>
    #include <ThirdParty/glm/vec4.hpp>
    #include <ThirdParty/glm/mat4x4.hpp>
    #include <ThirdParty/glm/gtc/matrix_transform.hpp>    

    #include <murkyFramework/src/GfxDevice/openGL4/shaderWrapper.hpp>
    #include <murkyFramework/src/GfxDevice/openGL4/textureWrapper.hpp>
    #include <murkyFramework/src/GfxDevice/openGL4/vertexBufferWrapper.hpp>
    #include <murkyFramework/src/GfxDevice/openGL4/GfxDeviceObj.hpp>
#endif

#ifdef USE_DIRECT3D12 //todo: separate pch?
    #include <d3d12.h>
    #include <d2d1_3.h>
    #include <d3d11on12.h>
    #include <D3Dcompiler.h>
    #include <directxcolors.h>
    #include <DirectXMath.h>    
    #include <dxgi1_4.h>
    #include <dwrite.h>    
    #include <ThirdParty/d3d12/d3dx12.h>
                
    #include <murkyFramework/src/GfxDevice/d3d12/shaderWrapper.hpp>
    #include <murkyFramework/src/GfxDevice/d3d12/textureWrapper.hpp>
    #include <murkyFramework/src/GfxDevice/d3d12/vertexBufferWrapper.hpp>
    #include <murkyFramework/src/GfxDevice/d3d12/gfxLowLevel_d3d12.hpp>
    #include <murkyFramework/src/GfxDevice/d3d12/shaders_d3d12.hpp>
    #include <murkyFramework/src/GfxDevice/d3d12/texture_d3d12.hpp>
    //#include <murkyFramework/src/GfxDevice/d3d12/vertexBuffer_d3d12.hpp>
    #include <murkyFramework/src/GfxDevice/d3d12/GfxDeviceObj.hpp>
#endif

    #include <murkyFramework/src/GfxDevice/public/textureWrappers.hpp>
    #include <murkyFramework/src/GfxDevice/TexturesManager.hpp>

#ifdef USE_OPENGL //todo: separate pch?
    #include <murkyFramework/src/GfxDevice/openGL4/TexturesManager_ogl4.hpp>
#endif

#ifdef USE_DIRECT3D12 //todo: separate pch?
#include <murkyFramework/src/GfxDevice/d3d12/TexturesManager_d3d12.hpp>
#endif

    #include <murkyFramework/src/GfxDevice/public/shaderWrappers.hpp>
    #include <murkyFramework/src/GfxDevice/public/vertexBufferWrappers.hpp>
    #include <murkyFramework/src/GfxDevice/texture.hpp>
// gfxDevice

#include <murkyFramework/src/readFBX.hpp>
#include <murkyFramework/src/stringHelpers.hpp>

// render
#include <murkyFramework/src/Render/linesShapes.hpp>
#include <murkyFramework/src/Render/projectionMat.hpp>
#include <murkyFramework/src/Render/model.hpp>
#include <murkyFramework/src/Render/render.hpp>
#include <murkyFramework/src/Render/textRender.hpp>
#include <murkyFramework/src/Render/RenderObj.hpp>
// render



