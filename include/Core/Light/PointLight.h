#pragma once
#include "Light.h"
#include "Math/Common.h"
namespace YYLB
{
    class PointLight : public Light
    {
    public:
        PointLight(float intence, Vec3f light_color) : Light(intence, light_color) {}
        virtual float attenuation(Vec3f &pos)
        {
            float r = std::sqrt((position_world - pos).scalar());
            float result = light_intense / (r * r);
            return result;
        }
        virtual Vec3f LightDir(Vec3f &pos)
        {
            return (position_world - pos);
        }
    };
}