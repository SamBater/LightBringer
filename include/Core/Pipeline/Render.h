#ifndef YYLB_RENDER_H
#define YYLB_RENDER_H
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <GLFW/glfw3.h>
#include "Math/Triangle.h"
#include <Math/Common.h>
#include "Core/Scene/Actor.h"
#include "Core/Scene/Camera.h"
#include <Math/Mesh.h>
#include <vector>
#include <chrono>
#include "Transformer.h"
#include "Core/Texture/Texture.h"
#include <Math/Vertex.h>
#include "Core/Light/ParallelLight.h"
#include "Core/Shader/PhongShader.h"
#include "Core/Light/PointLight.h"
#include "FrameBuffer.h"
#include "Core/ModelLoader.h"
#include "glm/glm.hpp"
#include "Core/Texture/CubeMap.h"
#include <iostream>
#include <Core/Shader/SkyBoxShader.h>
namespace ylb
{
    struct RenderTargetSetting
    {
        bool open_z_buffer_write = true;
        bool open_frame_buffer_write = true;
    };
    class Render
    {
    private:
        int w;
        int h;
        const char *title = "Light Bringer - Made By YBT";
        GLFWwindow *window;
        Camera *cam;
        Transformer *transformer;
        FrameBuffer *frame_buffer;
        RenderTargetSetting* renderTargetSetting;
        std::vector<Mesh> world;
        std::vector<Light *> lights;
        void processInput(double &&delta_time);
        void geometryProcessing(ylb::Triangle &t, Shader*& shader, Light*& light);
        void rasterization(ylb::Triangle &t, ylb::Shader*& shader, Light*& light);
        void render(std::vector<ylb::Mesh> &ts);
        void generate_shadow_map(Light* light);

        //返回值:true->剔除
        bool back_face_culling(Vertex& vt);
    public:
        Render(int _w = 800, int _h = 600);
        void start();
    };
}
#endif