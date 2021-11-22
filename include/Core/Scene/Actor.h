#ifndef YYLB_ACTOR_H
#define YYLB_ACTOR_H
#include "Math/Matrix.h"
namespace YYLB
{
    class Actor
    {

    public:
        YYLB::Vec3f position_world;
        Actor(Vec3f &list) : position_world(list) {}
        Actor(const float x = 0, const float y = 0, const float z = 0) : position_world{x, y, z} {}
        inline const Vec3f &getPos() const { return position_world; }
        virtual void setPos(float x, float y, float z)
        {
            position_world.x() = x;
            position_world.y() = y;
            position_world.z() = z;
        }
    };
}
#endif