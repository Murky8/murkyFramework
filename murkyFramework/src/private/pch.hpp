//------------------------------------------------------------------------------
// 2015 J. Coelho

#pragma message("Compiling precompiled headers.\n")

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
#include "hidusage.h"

#include <murkyFramework/include/version.hpp>
#include <murkyFramework/include/GfxDevice/version_gfxDevice.hpp>

#include <murkyFramework/include/types.hpp>

#include <murkyFramework/include/collectionNamed.hpp>
#include <murkyFramework/include/common.hpp>
#include <murkyFramework/include/debugUtils.hpp>
#include <murkyFramework/include/fileUtils.hpp>
#include <murkyFramework/include/loadSaveFile.hpp>
#include <murkyFramework/include/readFBX.hpp>
#include <murkyFramework/include/stringHelpers.hpp>
#include <murkyFramework/include/system.hpp>
#include <murkyFramework/include/vectorMatrix.hpp>
#include <murkyFramework/include/vectorMatrix_rotation.hpp>

#include <murkyFramework/include/GfxDevice/gfxPrimativeTypes.hpp>   
#include <murkyFramework/include/GfxDevice/vertexBuffer.hpp>
#include <murkyFramework/include/GfxDevice/gfxLowLevel.hpp>
#include <murkyFramework/include/GfxDevice/texture.hpp>
#include <murkyFramework/include/GfxDevice/shaders.hpp>

#include <murkyFramework/include/Render/linesShapes.hpp>
#include <murkyFramework/include/Render/projectionMat.hpp>
#include <murkyFramework/include/Render/render.hpp>
#include <murkyFramework/include/Render/textRender.hpp>

#include <murkyFramework/src/GfxDevice/public/gfxDevice.hpp>
#include <murkyFramework/src/GfxDevice/public/GfxDeviceObj.hpp>
#include <murkyFramework/src/Render/public/RenderObj.hpp>

#include <murkyFramework/src/private/systemSpecific.hpp>
#include <murkyFramework/src/private/windows/WindowsSpecific.hpp>
#include <murkyFramework/include/inputDevices.hpp>
#include <murkyFramework/src/game/public/game.hpp>
#include <murkyFramework/include/appFramework.hpp>

///