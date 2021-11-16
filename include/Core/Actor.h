#ifndef YYLB_ACTOR_H
#define YYLB_ACTOR_H
#include <math/Matrix.h>
namespace YYLB
{
    class Actor
    {
    protected:
        YYLB::Vec3f world_pos;

    public:
        Actor(Vec3f &list) : world_pos(list) {}
        Actor(const float x = 0, const float y = 0, const float z = 0) : world_pos{x, y, z} {}
        inline const Vec3f &getPos() const { return world_pos; }
    };
}
#endif