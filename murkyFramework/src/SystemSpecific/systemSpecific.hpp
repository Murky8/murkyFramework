#pragma once
//------------------------------------------------------------------------------
// 2015 J. Coelho.
// Platform: C++11
namespace murkyFramework {
    namespace systemSpecific {
        class SystemSpecificObj
        {
        public:
            SystemSpecificObj();
            virtual ~SystemSpecificObj() {}            
            virtual bool createWindow(std::wstring title, int width, int height) = 0;
            virtual f64 readTimeSecondsSinceAppStart() = 0;
        };

    }//namespace systemSpecific 

}//namespace murkyFramework