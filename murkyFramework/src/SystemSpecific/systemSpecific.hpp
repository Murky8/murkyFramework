#pragma once
//------------------------------------------------------------------------------
// 2015 J. Coelho.
// Platform: C++11
namespace murkyFramework {
class SystemSpecific
{    
public:
    SystemSpecific();
    virtual ~SystemSpecific(){}
    virtual bool createWindow(std::wstring title, int width, int height) = 0;    
    virtual f64 readTimeSecondsSinceAppStart() = 0;
};
}//namespace murkyFramework