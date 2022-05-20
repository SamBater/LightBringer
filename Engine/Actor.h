#ifndef YYLB_ACTOR_H
#define YYLB_ACTOR_H
#include "glm/glm.hpp"
#include "YLBSerializable.h"
#include "Transform.h"
namespace ylb
{
    class Actor
    {
    public:
        Actor(){}
        Actor(const glm::vec3& position, const glm::vec3& scale = glm::vec3(1, 1, 1)) : transform(position,scale) {

        }

        Transform transform;
    };


}
#endif