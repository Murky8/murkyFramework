#pragma once
//------------------------------------------------------------------------------
// 2015 J. Coelho.
namespace murkyFramework 
{
    class Game
    {
    public:
        Game() {}
        virtual ~Game() {}
        std::list<class EntityBase*> entities;
        vec4 cursorPos{ 0, 60, -150.f };
        mat3 cursorOri{ unit };
    };
}//namespace murkyFramework