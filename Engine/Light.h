#pragma once

#include "Camera.h"
#include "Actor.h"
#include "glm/glm.hpp"
#include "Texture.h"
#include "YLBSerializable.h"
namespace ylb {
class Light : public Actor , public YLBSerializable {
public:
    Light() = default;

    Light(const glm::vec3& light_color) :
        light_color(light_color) {
    }

    glm::vec3 LightIntensity() const {
        return light_color;
    }

    virtual float attenuation(const glm::vec3 &pos) const = 0;

    virtual glm::vec3 LightDir(const glm::vec3 &pos) const = 0;

    void SetLightIntensity(const glm::vec3& light_intensity) {
        light_color = light_intensity;
    }

    virtual void DeSerilization(const json11::Json &json) override {

    }

protected:
    glm::vec3 light_color = {1.f, 1.f, 1.f};
    friend class Shader;

private:
    friend class Render;
};
} // namespace ylb