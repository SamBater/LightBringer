#pragma once
#include "Light.h"
namespace ylb
{
    class ParalleLight : public Light
    {
    public:
        
        ParalleLight() :
            Light() {
        }
        
        ParalleLight(const glm::vec3& color, const glm::vec3& dir) : Light(color), dir(dir) {
        }
        
        virtual float attenuation(const glm::vec3 &pos) const override {
            return 1;
        }
        
        virtual glm::vec3 LightDir(const glm::vec3 &pos) const override
        {
            return dir * -1.f;
        }

        void SetDir(const glm::vec3& dir) {
            this->dir = glm::normalize(dir);
        }

        virtual void DeSerilization(const json11::Json &json) override {
            this->SetDir(DeSerilizationVec3(json["dir"]));
            this->SetLightIntensity(DeSerilizationVec3(json["intensity"]));
        }
        glm::vec3 dir = glm::vec3(0.33, 0.33, -0.33);
    protected:

    };
}