#pragma once
//------------------------------------------------------------------------------
// 2015 J. Coelho.
// Platform: C++11
namespace murkyFramework {}//namespace murkyFramework
class SystemSpecific
{
private:      
    int a = 0;
public:
    SystemSpecific();
    virtual bool createWindow(std::wstring title, int width, int height) = 0;    
};