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

    virtual float Attenuation(const glm::vec3 &pos) const = 0;

    void SetShadowMap(const glm::mat4& light_matrix,Texture* depth_buffer, int w , int h) {
        shadow_map = new Texture(*depth_buffer);
        this->view_project_matrix = light_matrix;
    }

    float VisibleInLightSpace(glm::vec4& shadowCoord) {
        if (!shadow_map)
            return 1.0f;
        
        shadowCoord /= shadowCoord.w;
        shadowCoord = (shadowCoord + glm::vec4(1, 1, 1, 0)) * 0.5f;
        float x = shadowCoord.x + offset.x;
        float y = shadowCoord.y + offset.y;
        float map_depth = (shadow_map->tex2d(x, y).x * 0.5f) + 0.5f;
        return map_depth + shadow_bias < (shadowCoord.z)  ? 0.5f : 1.0f;
         //float shadow = .3+.7*(map_depth + shadow_bias < (shadowCoord.z)); 
        // return shadow;
    }

    virtual glm::vec3 LightDir(const glm::vec3 &pos) const = 0;

    void SetLightIntensity(const glm::vec3& light_intensity) {
        light_color = light_intensity;
    }

    virtual void DeSerilization(const json11::Json &json) override {

    }

    glm::mat4& GetLightMatrix(){
		
        return view_project_matrix;
    }

    //static void SetOffSet();


    glm::mat4 view_project_matrix;

    glm::vec3 offset;

protected:
    glm::vec3 light_color = {1.f, 1.f, 1.f};
    friend class Shader;
    friend class Renderer;
    Texture* shadow_map;
    float shadow_bias = 0.005f;


private:
    friend class Render;
};
} // namespace ylb