//------------------------------------------------------------------------------
// 2015 J. Coelho.
// Platform: C++11

#include <murkyFramework/include/version.hpp>
#include <murkyFramework/include/loadSaveFile.hpp>
#include <murkyFramework/include/stringHelpers.hpp>
#include <murkyFramework/include/vectorMatrix.hpp>   
#include <murkyFramework/include/debugUtils.hpp>   
#include <murkyFramework/include/GfxDevice/gfxPrimativeTypes.hpp>   
#include <regex>
//http://www.cheatography.com/davechild/cheat-sheets/regular-expressions/

int getNextPosInt_incIt(std::wstring::const_iterator &it, std::wstring::const_iterator  &end)
{
    static std::wregex regx_int(L"[0-9]+");

    int i;
    std::wsmatch match;

    if (std::regex_search(it, end, match, regx_int))
    {
        std::wstring substr(match[0].first, match[0].second);
        i = _wtoi(substr.c_str());

        it = match[0].second;
    }

    return i;
}

int getNextInt_incIt(std::wstring::const_iterator &it, std::wstring::const_iterator  &end)
{
    static std::wregex regx_int(L"[-\\+]?[0-9]+");

    int i;
    std::wsmatch match;

    if (std::regex_search(it, end, match, regx_int))
    {
        std::wstring substr(match[0].first, match[0].second);
        i = _wtoi(substr.c_str());

        it = match[0].second;
    }

    return i;
}

float getNextFloat_incIt(std::wstring::const_iterator &it, std::wstring::const_iterator  &end)
{
    // "-+[0-9]+\\.+[0-9]+"
    static std::wregex regx_float(L"[-\\+]?[0-9]*\\.?[0-9]+");

    float f;
    std::wsmatch match;

    if (std::regex_search(it, end, match, regx_float))
    {
        std::wstring substr(match[0].first, match[0].second);
        f = (float)_wtof(substr.c_str());

        it = match[0].second;
    }

    return f;
}

bool findNextText_incIt(std::wstring &textToSearch, wchar_t *textToFind, std::wstring::const_iterator &it )
{
    std::size_t foundAtIndex = textToSearch.find(textToFind);
    if (foundAtIndex == std::string::npos)
    {
        return false;
    }

    it = textToSearch.cbegin() + foundAtIndex;    
    return true;
}

std::vector<Vert_pct> gdeb_verts;
std::vector<Triangle_pct> gdeb_tris;

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

        // get num verts

        auto it = text.cbegin();
        if (findNextText_incIt(text, L"Vertices:", it)==false)
        {            triggerBreakpoint();        }
                 

        int nVerts;
        nVerts = getNextInt_incIt(it, text.end()) / 3;

        // read verts
        for (int i = 0; i < nVerts; i++)
        {
            float x = getNextFloat_incIt(it, text.end());
            float z = getNextFloat_incIt(it, text.end());
            float y = getNextFloat_incIt(it, text.end());
            gdeb_verts.push_back
                (
                Vert_pct{ vec3(x, y, z), vec3(1, 1, 1), vec2(1, 1) }
                );
            //debugLog << i << x << y << z << L"\n";
        }
        // get num faces
        if (findNextText_incIt(text, L"PolygonVertexIndex:", it) == false)
        {
            triggerBreakpoint();
        }
        int nFaces = getNextInt_incIt(it, text.end()) / 3;
        
        // get face indices
        for (int i = 0; i < nFaces; i++)
        {
            int i0 = getNextInt_incIt(it, text.end());
            int i1 = getNextInt_incIt(it, text.end());
            int i2 = ~getNextInt_incIt(it, text.end()); // note: FBX weirdness -1

            gdeb_tris.push_back(
                Triangle_pct
                (

                gdeb_verts[i0],
                gdeb_verts[i1],
                gdeb_verts[i2]

                ));
        }
        
        // parse text
        return true;
    }