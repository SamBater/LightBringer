#include "Core/ModelLoader.h"
std::vector<YYLB::Triangle> YYLB::LoadObj(const char *modelPath)
{
    std::vector< unsigned int > vertexIndices, uvIndices, normalIndices;
    std::vector<YYLB::Vec3f> vts;
    std::vector<YYLB::Vec2f> uvs;
    std::vector<YYLB::Vec3f> ns;
    std::vector<YYLB::Triangle> ts;
    FILE* fp = fopen(modelPath,"r");
    if(fp == NULL)
    {
        std::cerr << "Can't open " << modelPath << " !\n";
        return std::vector<YYLB::Triangle>();
    }
    while(1){
        char lineHeader[256];
        int res = fscanf(fp,"%s",lineHeader);
        if(res == EOF) break;
        if(strcmp(lineHeader,"v") == 0){
            float x,y,z;
            fscanf(fp,"%f %f %f",&x,&y,&z);
            vts.push_back(Vec3f{x,y,z});
        }
        else if(strcmp(lineHeader,"vt") == 0)
        {
            float u,v;
            fscanf(fp,"%f %f",&u,&v);
            uvs.push_back(Vec2f{u,v});
        }
        else if(strcmp(lineHeader,"vn") == 0)
        {
            Vec3f n;
            fscanf(fp,"%f %f %f",&n.x(),&n.y(),&n.z());
            ns.push_back(n);
        }
        else if(strcmp(lineHeader,"f") == 0){
            std::string vertex1, vertex2, vertex3;
            int vertexIndex[3], uvIndex[3], normalIndex[3];
            int matches = fscanf(fp, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2] );
            if (matches != 9){
                std::cerr << "File can't be read by our simple parser : ( Try exporting with other options\n";
                return std::vector<YYLB::Triangle>();
            }
            auto in3 = [](std::vector<YYLB::Vec3f>&vec,int& i)->YYLB::Vec3f&{return vec[i-1];};
            auto in2 = [](std::vector<YYLB::Vec2f>&vec,int& i)->YYLB::Vec2f&{return vec[i-1];};
            
            std::vector<YYLB::Vertex> vtv;
            for(int i = 0 ; i < 3 ;i++)
            {
                Vec3f& pos = in3(vts,vertexIndex[i]);
                Vec2f& uv = in2(uvs,uvIndex[i]);
                Vec3f& n = in3(ns,normalIndex[i]);
                vtv.push_back(YYLB::Vertex(pos,n,uv));
            }
            ts.push_back(YYLB::Triangle(vtv[0],vtv[1],vtv[2]));
        }
    }
    return ts;
}