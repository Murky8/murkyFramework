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
#include <map>
#include <memory>
#include <mutex>
#include <ratio>
#include <regex>
#include <string>
#include <thread>
#include <vector>

#include <external/boost/circular_buffer.hpp>
#include <external/boost/multi_array.hpp>
#include <external/lodepng.h>
#include "hidusage.h"

#include <murkyFramework/include/version.hpp>
#include <murkyFramework/include/types.hpp>
#include <murkyFramework/include/common.hpp>

#include <murkyFramework/include/collectionNamed.hpp>
#include <murkyFramework/include/debugUtils.hpp>
#include <murkyFramework/include/fileUtils.hpp>
#include <murkyFramework/include/loadSaveFile.hpp>
#include <murkyFramework/include/readFBX.hpp>
#include <murkyFramework/include/stringHelpers.hpp>
#include <murkyFramework/include/system.hpp>
#include <murkyFramework/include/vectorMatrix.hpp>
#include <murkyFramework/include/vectorMatrix_rotation.hpp>

#include <murkyFramework/src/private/systemSpecific.hpp>
#include <murkyFramework/src/private/windows/WindowsSpecific.hpp>

#include <murkyFramework/include/inputDevices.hpp>
#include <murkyFramework/src/game/public/game.hpp>

// gfxDevice
#include <murkyFramework/include/GfxDevice/version_gfxDevice.hpp>
#include <murkyFramework/src/GfxDevice/public/gfxDevice.hpp>
#include <murkyFramework/include/GfxDevice/gfxPrimativeTypes.hpp>   
#include <murkyFramework/include/GfxDevice/vertexBuffer.hpp>
#include <murkyFramework/src/GfxDevice/private/vertexBufferHelpers.hpp>
#include <murkyFramework/include/GfxDevice/gfxLowLevel.hpp>
#include <murkyFramework/include/GfxDevice/texture.hpp>
#include <murkyFramework/src/GfxDevice/private/textureHelpers.hpp>
#include <murkyFramework/include/GfxDevice/shaders.hpp>

#ifdef USE_OPENGL //todo: fix
    #include <external/glew/include/GL/wglew.h>
    #define GLM_FORCE_RADIANS
    #include <external/glm/glm.hpp>
    #include <external/glm/vec2.hpp>
    #include <external/glm/vec3.hpp>
    #include <external/glm/vec4.hpp>
    #include <external/glm/mat4x4.hpp>
    #include <external/glm/gtc/matrix_transform.hpp>
    #include <external/glew/include/GL/glew.h>     
    #include <murkyFramework/src/GfxDevice/private/openGL/GfxDeviceObj.hpp>
#endif

#ifdef USE_DIRECT3D11 //todo: fix

    #include <d3d11_1.h>
    #include <d3dcompiler.h>
    #include <DirectXColors.h>
    #include <directxcolors.h>
    #include <directxmath.h>
    #include <murkyFramework/src/GfxDevice/private/d3d11/GfxDeviceObj.hpp>

#endif

#ifdef USE_DIRECT3D12 //todo: fix
    #include <d3d12.h>
    #include <d2d1_3.h>
    #include <d3d11on12.h>
    #include <D3Dcompiler.h>
    #include <directxcolors.h>
    #include <DirectXMath.h>    
    #include <dxgi1_4.h>
    #include <dwrite.h>    
    #include <external/d3d12/d3dx12.h>
    #include <murkyFramework/src/GfxDevice/private/d3d12/gfxDevice.h>
    #include <murkyFramework/include/GfxDevice/d3d12/shaders_d3d12.hpp>
    #include <murkyFramework/src/GfxDevice/private/d3d12/GfxDeviceObj.hpp>
#endif
// gfxDevice

// render
#include <murkyFramework/include/Render/linesShapes.hpp>
#include <murkyFramework/include/Render/projectionMat.hpp>
#include <murkyFramework/include/Render/render.hpp>
#include <murkyFramework/include/Render/textRender.hpp>
#include <murkyFramework/src/Render/public/RenderObj.hpp>
// render

#include <murkyFramework/include/appFramework.hpp>
