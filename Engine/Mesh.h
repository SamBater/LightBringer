#ifndef YYLB_MESH
#define YYLB_MESH
#include "glm/glm.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "Actor.h"
#include "Triangle.h"
#include "Shader.h"
#include <vector>
#include "YLBSerializable.h"
#include "ModelLoader.h"
namespace ylb {
class Renderer;
class Mesh : public Actor , public YLBSerializable {
public:
    Mesh() = default;
    
    Mesh(glm::vec3 pos, std::vector<Triangle> *trs, Shader *shader) :
        Actor(pos), triangles(trs), shader(shader) {
    }

    std::vector<Triangle> *Triangles() {
        return triangles;
    }

    void SetShader(Shader* shader) {
        this->shader = shader;
    }

    virtual void DeSerilization(const json11::Json& json) override {
        triangles = LoadObj(json["path"].string_value().c_str());
        transform.DeSerilization(json["Transform"]);
    }

private:
    friend Renderer;
    std::vector<Triangle> *triangles = new std::vector<Triangle>();
    ylb::Shader *shader = new Shader();
};
} // namespace ylb
#endif