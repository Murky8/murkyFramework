//------------------------------------------------------------------------------
// 2015 J. Coelho.
#include <murkyFramework/include/version.hpp>
#include <murkyFramework/include/gfxLowLevel/texture.hpp>

#include <vector>
#include <murkyFramework/include/types.hpp>
#include <map>
#include <murkyFramework/include/debugUtils.hpp>

namespace GfxLowLevel
{
    // forward declarations

    void TextureManager::insert(const std::wstring &name, TextureId texID)
    {
        textures.insert(std::pair<std::wstring, TextureId>(name, std::move(texID)));
    }

    TextureId &TextureManager::getTextureByName(const std::wstring &name)
    {
        auto it = textures.find(name);
        if (it != textures.end())
        {
            return it->second;
        }
        else
        {
            triggerBreakpoint();
            return it->second;
        }
    }
}