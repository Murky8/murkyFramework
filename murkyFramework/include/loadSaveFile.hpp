#pragma once
//------------------------------------------------------------------------------
// 2015 J. Coelho.
#include <murkyFramework/include/gfxLowLevel/version_gfxDevice.hpp>

#include <memory>
#include <string>

namespace qdev
{    
    void    setCurrentDirectoryToAppRoot();
    
    class BinaryFileLoader
    {
    public:                
        // Constructors 
        BinaryFileLoader() = delete;
        BinaryFileLoader(const std::wstring &dirName, const std::wstring &fileName,
            const std::wstring &extensionName);
        BinaryFileLoader(const std::wstring &fileName);
        BinaryFileLoader( const wchar_t *const fileName );

        // Destructors
        ~BinaryFileLoader();

        // Methods
        char    *data();
        int     getDataLength()const;        
        char        *pdata;        
        
    private:
        // Constructors 

        // Data
        int         dataLength;
    };    
}
