#pragma once
//------------------------------------------------------------------------------
// 2015 J. Coelho.
// Platform: C++11
namespace murkyFramework {
namespace systemSpecific {
class WindowsSpecific : public SystemSpecific
{
private:
    HDC			hDC;		 
    HINSTANCE	hInstance;	
    HWND		hWnd;	    

    f64     performanceCounterFrequency;
    f64     readTimeSeconds();
    f64     readTimeSecondsSinceAppStart();
    f64     getPerformanceCounterFrequency();
    const f64 appStartTime;
public:
    WindowsSpecific();
    void windowsLoopIteration();
    virtual bool createWindow(std::wstring title, int width, int height);    
    HGLRC		hRC;		
    
    HDC gethDC() const { return hDC; }
    HGLRC gethRC() const { return hRC; }
    HINSTANCE gethInstance() const { return hInstance; }
    HWND gethWnd()  const { return hWnd; }

protected:
    ~WindowsSpecific();
};
}//namespace systemSpecific
}//namespace murkyFramework
