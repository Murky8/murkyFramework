//------------------------------------------------------------------------------
// 2015 J. Coelho.
// Platform: C++11

#include <murkyFramework/include/version.hpp>
#include <murkyFramework/include/loadSaveFile.hpp>
#include <murkyFramework/include/stringHelpers.hpp>
#include <murkyFramework/include/debugUtils.hpp>   
#include <regex>

bool loadFBX(const std::wstring &dirName, const std::wstring &fileName, const std::wstring &extensionName)
    {
        std::unique_ptr<qdev::BinaryFileLoader> pBinaryFile(new qdev::BinaryFileLoader(dirName, fileName, extensionName));
        
        if (pBinaryFile->pdata == 0)
        {// unsuccessful
            return false;
        }
                
        std::string temp(pBinaryFile->pdata, pBinaryFile->getDataLength());
        std::wstring text = s2ws(temp);

        // parse text
        std::size_t foundAtIndex = text.find(L"Vertices");
        if (foundAtIndex != std::string::npos)
        {         
            //debugLog << (int)found << L"\n";
        }
        else
        {
            debugLog << L"loadFBX error" << L"\n";
            return false;
        }
        
        std::wregex regx_nVerts(L"\\*[0-9]+ ");
        std::wsmatch match;
                
        auto it = text.cbegin() + foundAtIndex;
        int nVerts;
        
        if (std::regex_search(it, text.cend(), match, regx_nVerts))
        {
            for (auto& m : match)
            {
                std::wstring substr(m.first +1, m.second -1);
                nVerts = _wtoi(substr.c_str());
                debugLog << m;
                it = m.second;
            }
            match.empty();

            std::wregex regx_vertex(L"[-+]?[0-9]*\\.?[0-9]+");
            for (int i = 0; i < nVerts; i++)
            {
                if (std::regex_search(it, text.cend(), match, regx_vertex))
                {
                    for (auto& m : match)
                    {
                        std::wstring substr(m.first, m.second);
                        float vert = _wtof(substr.c_str());
                        debugLog << vert << L"\n";
                        it = m.second;
                    }
                }
            }
            //match.
            //std::cout << "Match\n";

            //for (auto m : match)
            //  std::cout << "  submatch " << m << '\n';

            //std::cout << "match[1] = " << match[1] << '\n';
        }

        // parse text
        return true;
    }