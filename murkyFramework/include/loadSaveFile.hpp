//------------------------------------------------------------------------------
// 2014 J. Coelho
// Platform: C++03. Thread safe. Windows. 32/64

#pragma once
#include <memory>
#include <string>

namespace qdev
{    
    void    setCurrentDirectoryToAppRoot();
    
    class BinaryFileLoader
    {
    public:                
        // Constructors 
        BinaryFileLoader( const std::wstring &fileName );
        BinaryFileLoader( const wchar_t *const fileName );

        // Destructors
        ~BinaryFileLoader();

        // Methods
        char    *data();
        int     getDataLength()const;        
        
    private:
        // Constructors 
        BinaryFileLoader() = delete;

        // Data
        char        *pdata;        
        int         dataLength;
    };    
}
