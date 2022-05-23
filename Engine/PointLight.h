#pragma once
#include "Light.h"
#include "Common.h"
namespace ylb {
class PointLight : public Light {
public:

    PointLight(glm::vec3 light_color = glm::vec3(1, 1, 1)) :
        Light(light_color) {
    }

    virtual float attenuation(const glm::vec3 &pos) const override {
        return 1;
        float r = glm::distance(pos, transform.WorldPosition());
        float result = light_color.length() / (r * r);
        return result;
    }

    virtual glm::vec3 LightDir(const glm::vec3 &pos) const override {
        return (transform.WorldPosition() - pos);
    }

    virtual void DeSerilization(const json11::Json &json) override {
        this->light_color = DeSerilizationVec3(json["intensity"]);
        this->transform.SetPosition(DeSerilizationVec3(json["Transform"]["position"]));
    }
};
} // namespace ylb