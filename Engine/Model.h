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
        auto coord = vid >= 0 ? verties->at(vid) : glm::vec3(1,1,1);
        auto normal = nid >= 0 ? normals->at(nid) : glm::vec3(1,1,1);
        auto uv = uid >= 0 ? uvs->at(uid) : glm::vec2(0,0.5);
        Vertex v(coord, normal, uv);
        v.tangent = vid >= 0 ? tangents->at(vid) : glm::vec3(1,0,0);
        return v;
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
    std::vector<glm::vec3>* tangents = new std::vector<glm::vec3>();
    std::vector<Face>* faces = new std::vector<Face>();
    ylb::Shader *shader = new Shader();
};
} // namespace ylb
#endif