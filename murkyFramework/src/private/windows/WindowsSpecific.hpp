#pragma once
//------------------------------------------------------------------------------
// 2015 J. Coelho.
// Platform: C++11

class WindowsSpecific : public SystemSpecific
{
private:
    HDC			hDC;		 
    HGLRC		hRC;		
    HINSTANCE	hInstance;	
    HWND		hWnd;	    

public:
    WindowsSpecific();
    ~WindowsSpecific();
    void windowsLoopIteration();
    virtual bool createWindow(std::wstring title, int width, int height);    
    
    HDC gethDC() const { return hDC; }
    HGLRC gethRC() const { return hRC; }
    HINSTANCE gethInstance() const { return hInstance; }
    HWND gethWnd()  const { return hWnd; }
};

