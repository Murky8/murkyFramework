#pragma once
//------------------------------------------------------------------------------
// 2015 J. Coelho
// Platform: Windows 32/64
// note: partially depreciated as defines are now stored in project settings.

#ifndef USE_OPENGL
#ifndef USE_VULKAN
#ifndef USE_DIRECT3D12

//stop compilation: at least one of these must be defined
#error Gfx device version needs to be specified.

#endif
#endif
#endif

