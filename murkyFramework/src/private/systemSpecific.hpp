#pragma once
//------------------------------------------------------------------------------
// 2015 J. Coelho.
// Platform: C++11

class SystemSpecific
{
private:      
public:
    virtual bool createWindow(std::wstring title, int width, int height) = 0;    
};