#ifndef YYLB_ACTOR_H
#define YYLB_ACTOR_H
#include "glm/glm.hpp"
namespace ylb
{
    class Actor
    {
    public:
        glm::vec3 position_world;
        Actor(glm::vec3 &list) : position_world(list) {}
        Actor(const float x = 0, const float y = 0, const float z = 0) : position_world{x, y, z} {}
        inline const glm::vec3 & getPos() const { return position_world; }
        virtual void setPos(float x, float y, float z)
        {
            position_world.x = x;
            position_world.y = y;
            position_world.z = z;
        }

    };


}
#endif