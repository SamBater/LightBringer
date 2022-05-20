#include "ModelLoader.h"
#include <iostream>
#define TINYOBJLOADER_IMPLEMENTATION
// #define TINYOBJLOADER_USE_MAPBOX_EARCUT
#include "tiny_obj_loader.h"

ylb::Mesh* ylb::LoadModel(const char* modelPath)
{
    ylb::Mesh* mesh = new ylb::Mesh();
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
    for (size_t s = 0; s < shapes.size(); s++) {
        for (size_t v = 0; v < attrib.vertices.size(); v += 3)
        {
            auto vx = attrib.vertices[v];
            auto vy = attrib.vertices[v + 1];
            auto vz = attrib.vertices[v + 2];
            mesh->verties->push_back(glm::vec3(vx, vy, vz));
        }

        for (size_t n = 0; n < attrib.normals.size(); n += 3)
        {
            auto nx = attrib.normals[n];
            auto ny = attrib.normals[n + 1];
            auto nz = attrib.normals[n + 2];
            mesh->normals->push_back(glm::vec3(nx, ny, nz));
        }

        for (size_t uv = 0; uv < attrib.texcoords.size(); uv += 2) {
            auto u = attrib.texcoords[uv];
            auto v = attrib.texcoords[uv + 1];
            mesh->uvs->push_back(glm::vec2(u, v));
        }


        for (int i = 0; i < shapes[s].mesh.indices.size() / 3; i++) {
            Face face;
            for (int j = 0; j < 3; j++) {
                face.vid[j] = shapes[s].mesh.indices[i * 3 + j].vertex_index;
                face.uid[j] = shapes[s].mesh.indices[i * 3 + j].texcoord_index;
                face.nid[j] = shapes[s].mesh.indices[i * 3 + j].normal_index;
            }
            mesh->faces->push_back(face);
        }
    }
    return mesh;
}
