//------------------------------------------------------------------------------
// 2015 J. Coelho

#pragma message("Compiling precompiled headers.\n")

#include <combaseapi.h>
#include <stdlib.h>
#include <windows.h>
#include <wchar.h>
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

#include <murkyFramework/include/appFramework.hpp>
#include <murkyFramework/include/collectionNamed.hpp>
#include <murkyFramework/include/common.hpp>
#include <murkyFramework/include/debugUtils.hpp>
#include <murkyFramework/include/fileUtils.hpp>
#include <murkyFramework/include/inputDevices.hpp>
#include <murkyFramework/include/loadSaveFile.hpp>
#include <murkyFramework/include/stringHelpers.hpp>
#include <murkyFramework/include/state.hpp>
#include <murkyFramework/include/system.hpp>
#include <murkyFramework/include/types.hpp>
#include <murkyFramework/include/vectorMatrix.hpp>

//#include <murkyFramework/include/GfxDevice/version_gfxDevice.hpp>
#include <murkyFramework/include/GfxDevice/gfxPrimativeTypes.hpp>   