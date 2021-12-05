#pragma once

#include <Core/Scene/Camera.h>
#include "Core/Scene/Actor.h"
#include "Math/Matrix.h"
#include "Core/Texture/Texture.h"
namespace YYLB
{
        class GouraudShader;
        class Light : public Actor
        {
        private:
            friend class Render;
        protected:
                float light_intense = 1.0f;
                Vec3f light_color = {1.f, 1.f, 1.f};
                friend class Shader;
                friend class GouraudShader;

        public:
                Texture *shadow_map;
                Matrix4f lvp;
                Light(float intence, Vec3f light_color) : light_intense(intence), light_color(light_color) {}
                virtual float attenuation(Vec3f &pos) = 0;
                virtual Vec3f LightDir(Vec3f &pos) = 0;
        };
}