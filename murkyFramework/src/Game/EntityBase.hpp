#pragma once
//------------------------------------------------------------------------------
// 2015 J. Coelho.
namespace murkyFramework {
    class EntityBase
    {
    public:

        EntityBase() {}
        virtual ~EntityBase(){}
        mat4 transform = {unit};
        vec4 velocity = {zero};
        vec4 angularVel = {zero};
    };
}//namespace murkyFramework