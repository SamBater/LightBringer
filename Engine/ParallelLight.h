#pragma once
#include "Light.h"
namespace ylb
{
    class ParalleLight : public Light
    {
    private:
    public:
        glm::vec3 dir;
        ParalleLight(float intence, glm::vec3 color, glm::vec3 dir) : Light(intence, color), dir(dir) {}
        virtual float attenuation(glm::vec3 &pos) { return 1.0f; }
        virtual glm::vec3 LightDir(glm::vec3 &pos)
        {
            return dir * -1.f;
        }
    };
}