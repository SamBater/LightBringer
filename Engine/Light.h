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

    void SetShadowMap(const glm::mat4& light_matrix,float *depth_buffer , int w , int h) {
        shadow_depth = new float[w*h];
        for(int i = 0 ; i < w * h ;i++)
            // shadow_depth[i] = depth_buffer[i];
            shadow_depth[i] = (depth_buffer[i] * 0.5f) +0.5f;
        shadow_map = new Texture(shadow_depth,w,h,1,1.0f);
        this->w = w;
        this->h = h;

        this->view_project_matrix = light_matrix;
    }

    float VisibleInLightSpace(glm::vec4& shadowCoord) {
        if (!shadow_map)
            return 1.0f;
        shadowCoord /= shadowCoord.w;
        int x = shadowCoord.x;
        int y = shadowCoord.y;
        int idx = y * w + x;
        // float map_depth = shadow_map->tex2d(x,y).x;
        float map_depth = shadow_depth[idx];
        return map_depth + shadow_bias < (shadowCoord.z * 0.5f + 0.5f)  ? 0.5f : 1.0f;
        // float shadow = .3+.7*(shadow_depth[idx] + shadow_bias < (shadowCoord[2])); 
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
    glm::mat4 view_project_matrix;
protected:
    glm::vec3 light_color = {1.f, 1.f, 1.f};
    friend class Shader;
    friend class Renderer;
    float* shadow_depth;
    int w,h;
    Texture* shadow_map;
    float shadow_bias = 0.005f;


private:
    friend class Render;
};
} // namespace ylb