#pragma once

#include "Camera.h"
#include "Actor.h"
#include "glm/glm.hpp"
#include "Texture.h"
namespace ylb
{
        class GouraudShader;
        class Light : public Actor
        {
        private:
            friend class Render;
        protected:
                float light_intense = 1.0f;
                glm::vec3 light_color = {1.f, 1.f, 1.f};
                friend class Shader;

        public:
                Texture *shadow_map;
                glm::mat4 vp;
                Light(float intence, glm::vec3 light_color) : light_intense(intence), light_color(light_color) {}
                virtual float attenuation(glm::vec3 &pos) = 0;
                virtual glm::vec3 LightDir(glm::vec3 &pos) = 0;
        };
}