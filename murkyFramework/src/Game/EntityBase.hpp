#pragma once
//------------------------------------------------------------------------------
// 2015 J. Coelho.
namespace murkyFramework {
    class EntityBase
    {
    public:

        EntityBase() {}
        EntityBase(const mat4 &transform) : transform(transform)
        {}

        EntityBase(const mat4 &posOri, vec4 vel, vec4 angularVel) : 
            transform(posOri), vel(vel), angularVel(angularVel)
        {}

        virtual void doFrame() {}

        virtual ~EntityBase(){}
        mat4 transform  = {unit};
        vec4 vel        = {zero};
        vec4 angularVel = {zero};
    };
}//namespace murkyFramework