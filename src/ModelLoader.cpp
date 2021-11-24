#include "Core/ModelLoader.h"
#include "Math/Matrix.h"
#include <regex>
std::vector<YYLB::Triangle> YYLB::LoadObj(const char *modelPath)
{
    FILE* fp = fopen(modelPath, "r");
    if (fp == NULL)
    {
        std::cerr << "Can't open " << modelPath << " !\n";
        return std::vector<YYLB::Triangle>();
    }

    std::vector<YYLB::Vec3f> vts;
    std::vector<YYLB::Vec2f> uvs;
    std::vector<YYLB::Vec3f> ns;
    std::vector<YYLB::Triangle> ts;
    int f_style = -1 , matches = -1;


    int vertexIndex[3],uvIndex[3],normalIndex[3];
    char lineHeader[128] = "";
    char lineData[256] = "";
    Vec3f n;
    float u=0, v=0;
    

    while(1){

        int res = fscanf(fp,"%s",lineHeader);
        if(res == EOF) break;
        if(strcmp(lineHeader,"v") == 0){
            float x,y,z;
            matches = fscanf(fp,"%f %f %f",&x,&y,&z);
            if (matches < 3)
            {
                std::cerr << "error when load pos on file:" << modelPath << std::endl;
            }
            vts.push_back(Vec3f{x,y,z});
        }
        else if(strcmp(lineHeader,"vt") == 0)
        {
            matches = fscanf(fp,"%f %f",&u,&v);
            if (matches < 2)
            {
                std::cerr << "error when load vt on file:" << modelPath << std::endl;
            }
            uvs.push_back(Vec2f{u,v});
        }
        else if(strcmp(lineHeader,"vn") == 0)
        {
            
            matches = fscanf(fp,"%f %f %f",&n.x(),&n.y(),&n.z());
            if (matches < 3)
            {
                std::cerr << "error when load normal on file:" << modelPath << std::endl;
            }
            ns.push_back(n);
        }
        else if(strcmp(lineHeader,"f") == 0){
           
            fgets(lineData, 255, fp);

            if (f_style == -1)
            {
                std::regex r[3];
                r[0] = " [[:digit:]]*//[[:digit:]]* [[:digit:]]*//[[:digit:]]* [[:digit:]]*//[[:digit:]]*\n";
                r[1] = " [[:digit:]]*/[[:digit:]]* [[:digit:]]*/[[:digit:]]* [[:digit:]]*/[[:digit:]]*\n";
                r[2] = " [[:digit:]]*/[[:digit:]]*/[[:digit:]]* [[:digit:]]*/[[:digit:]]*/[[:digit:]]* [[:digit:]]*/[[:digit:]]*/[[:digit:]]*\n";
                for (int i = 0; i < 3; i++)
                {
                    if (std::regex_match(lineData, r[i]))
                    {
                        f_style = i;
                        break;
                    }
                }
                if (f_style == -1)
                {
                    std::cerr << "cant't parse file:" << modelPath << std::endl;
                    return std::vector<YYLB::Triangle>();
                }
            }

            if (f_style == 0)
            {
                int matches = sscanf(lineData,"%d//%d %d//%d %d//%d\n",&vertexIndex[0],&normalIndex[0],&vertexIndex[1],&normalIndex[1],&vertexIndex[2],&normalIndex[2]);
                if (matches < 6)
                {
                    std::cerr << "fscanf matches error.\n";
                    return ts;
                }
            }

            if (f_style == 2)
            {
                int matches = sscanf(lineData, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
                if (matches < 9)
                {
                    std::cerr << "fscanf matches error.\n";
                    return ts;
                }
            }

            // int matches = fscanf(fp,"%d %d %d\n",&vertexIndex[0],&vertexIndex[1],&vertexIndex[2]);

            auto in3 = [](std::vector<YYLB::Vec3f>&vec,size_t i)->YYLB::Vec3f&{return vec[i-1];};
            auto in2 = [](std::vector<YYLB::Vec2f>&vec,size_t i)->YYLB::Vec2f&{return vec[i-1];};
            
            Vec2f defaultVec2f = { 0.5f,0.5f };
            Vec3f defaultVec3f = { 0.33f,0.33f,0.33f };
            std::vector<YYLB::Vertex> vtv;
            for(size_t i = 0 ; i < 3 ;i++)
            {
                Vec3f& pos = in3(vts,vertexIndex[i]);
                Vec2f& uv = uvs.size() > 0 ? in2(uvs, uvIndex[i]) : defaultVec2f;
                Vec3f& n = ns.size() > 0 ? in3(ns, normalIndex[i]) : defaultVec3f;
                vtv.push_back(YYLB::Vertex(pos,n,uv));
            }
            ts.push_back(YYLB::Triangle(vtv[0],vtv[1],vtv[2]));
        }
    }
    return ts;
}