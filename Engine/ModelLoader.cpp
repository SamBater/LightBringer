#include "ModelLoader.h"
#include "glm/glm.hpp"
#include <iostream>
#include <regex>
#include "YLBFileSystem.h"
std::vector<ylb::Triangle>* ylb::LoadObj(const char *modelPath) {
    std::string s_modelPath = ylb::YLBFileSystem::GetInstance().GetAssetsPath(modelPath);
    FILE *fp = fopen(s_modelPath.c_str(), "r");
    if (fp == NULL) {
        std::cerr << "Can't open " << modelPath << " !\n";
        return nullptr;
    }

    std::vector<glm::vec3> vts;
    std::vector<glm::vec2> uvs;
    std::vector<glm::vec3> ns;
    std::vector<ylb::Triangle>* ts = new std::vector<ylb::Triangle>();
    int f_style = -1, matches = -1;

    int vertexIndex[3], uvIndex[3], normalIndex[3];
    char lineHeader[128] = "";
    char lineData[256] = "";
    glm::vec3 n;
    float u = 0, v = 0;

    while (1) {
        int res = fscanf(fp, "%s", lineHeader);
        if (res == EOF)
            break;
        if (strcmp(lineHeader, "v") == 0) {
            float x, y, z;
            matches = fscanf(fp, "%f %f %f", &x, &y, &z);
            if (matches < 3) {
                std::cerr << "error when load pos on file:" << modelPath << std::endl;
            }
            vts.push_back(glm::vec3{x, y, z});
        } else if (strcmp(lineHeader, "vt") == 0) {
            matches = fscanf(fp, "%f %f", &u, &v);
            if (matches < 2) {
                std::cerr << "error when load vt on file:" << modelPath << std::endl;
            }
            uvs.push_back(glm::vec2{u, v});
        } else if (strcmp(lineHeader, "vn") == 0) {
            matches = fscanf(fp, "%f %f %f", &n.x, &n.y, &n.z);
            if (matches < 3) {
                std::cerr << "error when load normal on file:" << modelPath
                          << std::endl;
            }
            ns.push_back(n);
        } else if (strcmp(lineHeader, "f") == 0) {
            fgets(lineData, 255, fp);

            if (f_style == -1) {
                std::regex r[3];
                r[0] = " [[:digit:]]*//[[:digit:]]* [[:digit:]]*//[[:digit:]]* "
                       "[[:digit:]]*//[[:digit:]]*\n";
                r[1] = " [[:digit:]]*/[[:digit:]]* [[:digit:]]*/[[:digit:]]* "
                       "[[:digit:]]*/[[:digit:]]*\n";
                r[2] = " [[:digit:]]*/[[:digit:]]*/[[:digit:]]* "
                       "[[:digit:]]*/[[:digit:]]*/[[:digit:]]* "
                       "[[:digit:]]*/[[:digit:]]*/[[:digit:]]*\n";
                for (int i = 0; i < 3; i++) {
                    if (std::regex_match(lineData, r[i])) {
                        f_style = i;
                        break;
                    }
                }
                if (f_style == -1) {
                    std::cerr << "cant't parse file:" << modelPath << std::endl;
                    return nullptr;
                }
            }

            if (f_style == 0) {
                int matches = sscanf(lineData, "%d//%d %d//%d %d//%d\n",
                                     &vertexIndex[0], &normalIndex[0], &vertexIndex[1],
                                     &normalIndex[1], &vertexIndex[2], &normalIndex[2]);
                if (matches < 6) {
                    std::cerr << "fscanf matches error.\n";
                    return ts;
                }
            }

            if (f_style == 2) {
                int matches = sscanf(lineData, "%d/%d/%d %d/%d/%d %d/%d/%d\n",
                                     &vertexIndex[0], &uvIndex[0], &normalIndex[0],
                                     &vertexIndex[1], &uvIndex[1], &normalIndex[1],
                                     &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
                if (matches < 9) {
                    std::cerr << "fscanf matches error.\n";
                    return ts;
                }
            }

            // int matches = fscanf(fp,"%d %d
            // %d\n",&vertexIndex[0],&vertexIndex[1],&vertexIndex[2]);

            auto in3 = [](std::vector<glm::vec3> &vec, size_t i) -> glm::vec3 & {
                return vec[i - 1];
            };
            auto in2 = [](std::vector<glm::vec2> &vec, size_t i) -> glm::vec2 & {
                return vec[i - 1];
            };

            glm::vec2 defaultvec2 = {0.5f, 0.5f};
            glm::vec3 defaultvec3 = {0.33f, 0.33f, 0.33f};
            std::vector<ylb::Vertex> vtv;
            for (size_t i = 0; i < 3; i++) {
                glm::vec3 &pos = in3(vts, vertexIndex[i]);
                glm::vec2 &uv = uvs.size() > 0 ? in2(uvs, uvIndex[i]) : defaultvec2;
                glm::vec3 &n = ns.size() > 0 ? in3(ns, normalIndex[i]) : defaultvec3;
                vtv.push_back(ylb::Vertex(pos, n, uv));
            }
            ts->push_back(ylb::Triangle(vtv[0], vtv[1], vtv[2]));
        }
    }
    return ts;
}