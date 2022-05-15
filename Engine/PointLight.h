#pragma once
#include "Light.h"
#include "Common.h"
namespace ylb
{
    class PointLight : public Light
    {
    public:
        PointLight(float intence, glm::vec3 light_color) : Light(intence, light_color) {}
        virtual float attenuation(glm::vec3 &pos)
        {
            float r = glm::distance(pos,position_world);
            float result = light_intense / (r * r);
            return result;
        }
        virtual glm::vec3 LightDir(glm::vec3 &pos)
        {
            return (position_world - pos);
        }
    };
}