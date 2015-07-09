#pragma once
//------------------------------------------------------------------------------
// 2015 J. Coelho.
#include <murkyFramework/include/version.hpp>

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
