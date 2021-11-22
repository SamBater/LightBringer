#pragma once
#include "Core/Scene/Actor.h"
#include "Math/Matrix.h"
namespace YYLB
{
        class SimpleShader;
        class Light : public Actor
        {
        protected:
                float light_intense = 1.0f;
                Vec3f light_color = {1.f, 1.f, 1.f};
                friend class Shader;
                friend class SimpleShader;

        public:
                Light(float intence, Vec3f light_color) : light_intense(intence), light_color(light_color) {}
                //pos为顶点坐标
                virtual float attenuation(Vec3f &pos) = 0;
                virtual Vec3f LightDir(Vec3f &pos) = 0;
        };
}