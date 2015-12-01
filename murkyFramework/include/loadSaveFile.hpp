#pragma once
//------------------------------------------------------------------------------
// 2015 J. Coelho.

namespace qdev
{    
    void    setCurrentDirectoryToAppRoot();
    
    class BinaryFileLoader
    {
    public:                
        // constructors 
        BinaryFileLoader() = delete;
        BinaryFileLoader(const std::wstring &dirName, const std::wstring &fileName,
            const std::wstring &extensionName);
        BinaryFileLoader(const std::wstring &fileName);
        BinaryFileLoader( const wchar_t *const fileName );

        // destructor
        ~BinaryFileLoader();

        // methods
        char    *data();
        int     getDataLength()const;        
        char       *pdata;        
        
    private:        
        // data
        int         dataLength;
    };    
}
