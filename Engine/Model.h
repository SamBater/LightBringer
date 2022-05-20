#ifndef YYLB_MODEL
#define YYLB_MODEL
#include "glm/glm.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "Actor.h"
#include "Triangle.h"
#include "Shader.h"
#include <vector>
#include "YLBSerializable.h"
namespace ylb {
struct Face {
    glm::vec<3,int> vid;
    glm::vec<3,int> uid;
    glm::vec<3,int> nid;
};

struct Model : public Actor , public YLBSerializable {
    Model() = default;

    Vertex Vert(int vid,int nid,int uid) {
        auto vert = verties->at(vid);
        auto normal = normals->at(nid);
        auto uv = uvs->at(uid);
        return Vertex(vert, normal, uv);
    }

    Triangle Triangle(int fid) {
        auto face = faces->at(fid);
        Vertex vts[3];
        for (int i = 0; i < 3; i++)
            vts[i] = Vert(face.vid[i], face.nid[i], face.uid[i]);
        return ylb::Triangle(vts[0], vts[1], vts[2]);
    }

    void SetShader(Shader* shader) {
        this->shader = shader;
    }

    virtual void DeSerilization(const json11::Json& json) override {
        //triangles = LoadObj(json["path"].string_value().c_str());
        transform.DeSerilization(json["Transform"]);
    }

    std::vector<glm::vec3>* verties = new std::vector<glm::vec3>();
    std::vector<glm::vec3>* normals = new std::vector<glm::vec3>();
    std::vector<glm::vec2>* uvs = new std::vector<glm::vec2>();
    std::vector<Face>* faces = new std::vector<Face>();
    ylb::Shader *shader = new Shader();
};
} // namespace ylb
#endif