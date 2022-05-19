#ifndef YYLB_MESH
#define YYLB_MESH
#include "glm/glm.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "Actor.h"
#include "Triangle.h"
#include "Shader.h"
#include <vector>
#include "YLBSerializable.h"
namespace ylb {
class Model : public Actor , public YLBSerializable {
public:
    Model() = default;

    void SetModel(Model* model) {
        verties = model->verties;
        normals = model->normals;
        uvs = model->uvs;
        faces = model->faces;
    }

    void SetShader(Shader* shader) {
        this->shader = shader;
    }

    virtual void DeSerilization(const json11::Json& json) override {
        transform.DeSerilization(json["Transform"]);
    }

    std::vector<glm::vec3>* verties = new std::vector<glm::vec3>();
    std::vector<glm::vec3>* normals = new std::vector<glm::vec3>();
    std::vector<glm::vec2>* uvs = new std::vector<glm::vec2>();
    std::vector<std::vector<int>*>* faces = new std::vector<std::vector<int>*>();
    Transform transform;
    Shader* shader = nullptr;
};
} // namespace ylb
#endif