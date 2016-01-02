#pragma once
//------------------------------------------------------------------------------
// 2015 J. Coelho.
namespace murkyFramework 
{
    class Game
    {
    public:
        mat4 cursorPosOri = {unit};
        Game()
        {
            cursorPosOri.set_t(vec4(0.f, 0.f, -1.f));
        }
        virtual ~Game() {}
        std::list<class EntityBase*> entities;


        //vec4 cursorPos{ 0, 60, -150.f };
        //mat3 cursorOri{ unit };
    };
}//namespace murkyFramework