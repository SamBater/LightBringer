#pragma once
#include "Light.h"
namespace YYLB
{
    class ParalleLight : public Light
    {
    private:
    public:
        Vec3f dir;
        ParalleLight(float intence, Vec3f color, Vec3f dir) : Light(intence, color), dir(dir) {}
        virtual float attenuation(Vec3f &pos) { return 1.0f; }
        virtual Vec3f LightDir(Vec3f &pos)
        {
            return dir * -1.f;
        }
    };
}