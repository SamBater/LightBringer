#include "ModelLoader.h"
#include <iostream>
#define TINYOBJLOADER_IMPLEMENTATION
// #define TINYOBJLOADER_USE_MAPBOX_EARCUT
#include "tiny_obj_loader.h"

ylb::Model* ylb::LoadModel(const char* modelPath)
{
    ylb::Model* model = new ylb::Model();
    std::string inputfile = ylb::YLBFileSystem::GetInstance().GetAssetsPath(modelPath);
    tinyobj::ObjReaderConfig reader_config;
    reader_config.mtl_search_path = ylb::YLBFileSystem::GetInstance().GetAssetsPath(""); // Path to material files

    tinyobj::ObjReader reader;

    if (!reader.ParseFromFile(inputfile, reader_config)) {
        if (!reader.Error().empty()) {
            std::cerr << "TinyObjReader: " << reader.Error();
        }
        exit(1);
    }

    if (!reader.Warning().empty()) {
        std::cout << "TinyObjReader: " << reader.Warning();
    }

    auto& attrib = reader.GetAttrib();
    auto& shapes = reader.GetShapes();
    auto& materials = reader.GetMaterials();

    // Loop over shapes
    for (size_t inv_w = 0; inv_w < shapes.size(); inv_w++) {
        for (size_t v = 0; v < attrib.vertices.size(); v += 3)
        {
            auto vx = attrib.vertices[v];
            auto vy = attrib.vertices[v + 1];
            auto vz = attrib.vertices[v + 2];
            model->verties->push_back(glm::vec3(vx, vy, vz));
            model->tangents->push_back(glm::vec3(0, 0, 0));
        }

        for (size_t n = 0; n < attrib.normals.size(); n += 3)
        {
            auto nx = attrib.normals[n];
            auto ny = attrib.normals[n + 1];
            auto nz = attrib.normals[n + 2];
            model->normals->push_back(glm::vec3(nx, ny, nz));
        }

        for (size_t uv = 0; uv < attrib.texcoords.size(); uv += 2) {
            auto u = attrib.texcoords[uv];
            auto v = attrib.texcoords[uv + 1];
            model->uvs->push_back(glm::vec2(u, v));
        }


        for (int i = 0; i < shapes[inv_w].mesh.indices.size() / 3; i++) {
            Face face;
            for (int j = 0; j < 3; j++) {
                face.vid[j] = shapes[inv_w].mesh.indices[i * 3 + j].vertex_index;
                face.uid[j] = shapes[inv_w].mesh.indices[i * 3 + j].texcoord_index;
                face.nid[j] = shapes[inv_w].mesh.indices[i * 3 + j].normal_index;
            }
            model->faces->push_back(face);
        }
    }
    CalcTangents(model);
    return model;
}

void CalcTangents(ylb::Model* model)
{
    auto& faces = model->faces;
    auto& vts = model->verties;
    auto& uvs = model->uvs;
    auto& tangents = model->tangents;
    for (unsigned int i = 0; i < faces->size(); i ++) {

        auto& face = faces->at(i);
        int vid0 = face.vid[0];
        int vid1 = face.vid[1];
        int vid2 = face.vid[2];
        int uid0 = face.uid[0];
        int uid1 = face.uid[1];
        int uid2 = face.uid[2];

        auto& v0 = vts->at(vid0);
        auto& v1 = vts->at(vid1);
        auto& v2 = vts->at(vid2);
        
        auto& uv0 = uvs->at(uid0);
        auto& uv1 = uvs->at(uid1);
        auto& uv2 = uvs->at(uid2);

        glm::vec3 edge1 = v1 - v0;
        glm::vec3 edge2 = v2 - v0;
        glm::vec2 deltaUV1 = uv1 - uv0;
        glm::vec2 deltaUV2 = uv2 - uv0;

        glm::vec3 tangent;
        //glm::vec3 bitangent;

        float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

        tangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
        tangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
        tangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);

        //bitangent.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
        //bitangent.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
        //bitangent.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);

        //model->bitangents->at(vid0) += bitangent;
        //model->bitangents->at(vid1) += bitangent;
        //model->bitangents->at(vid2) += bitangent;

        tangents->at(vid0) += tangent;
        tangents->at(vid1) += tangent;
        tangents->at(vid2) += tangent;
    }

    for (unsigned int i = 0; i < vts->size(); i++) {
        auto& t = tangents->at(i);
        t = glm::normalize(t);
        //auto& bt = model->bitangents->at(i);
        //bt = glm::normalize(bt);
    }
}
