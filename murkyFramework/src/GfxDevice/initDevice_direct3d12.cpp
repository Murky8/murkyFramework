//------------------------------------------------------------------------------
// 2015 J. Coelho.
// Platform: C++11
#include <murkyFramework/include/version.hpp>
#include <murkyFramework/include/GfxDevice/version_gfxDevice.hpp>
#ifdef USE_DIRECT3D12

#include <windows.h>
#include <d3d12.h>
//#include "d3dx12.h"
#include <murkyFramework/include/common.hpp>
#include <murkyFramework/include/debugUtils.hpp>


//#include <d2d1_3.h>
//#include <dwrite.h>
//#include <d3d11on12.h>
//#include <dxgi1_4.h>
//#include <D3Dcompiler.h>
//#include <DirectXMath.h>

#include <string>
#include <wrl.h>

//using namespace DirectX;
using namespace Microsoft::WRL;

namespace GfxDevice
{        
    // forward declarations    
    extern  HDC     hDC;
    extern  HGLRC   hRC;
    extern  HWND    hWnd;

    extern  HINSTANCE	g_hInst;
    
	_Check_return_
    bool initialise_device24(HDC &hDC, HGLRC &hRC, HWND &hWnd)
    {
        //-------------------------------------------------------------------------------------- 
        // Create Direct3D device and swap chain 
        
            GfxDevice::hDC = hDC;//hDC = GetDC(hWnd); // Get the device context for our window
            GfxDevice::hRC = hRC;
            GfxDevice::hWnd = hWnd;

#ifdef _DEBUG
			// Enable the D3D12 debug layer.
			{
				ComPtr<ID3D12Debug> debugController;
				if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
				{
					debugController->EnableDebugLayer();
				}
			}
#endif

			return true;
    }


    bool deinitialise_device24()
    {       
        return true;
    }
} // namespace GfxDevice
#endif // USE_DIRECT3D12
