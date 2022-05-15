#ifndef YYLB_RENDER_H
#define YYLB_RENDER_H
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <GLFW/glfw3.h>
#include "Triangle.h"
#include "Common.h"
#include "Actor.h"
#include "Camera.h"
#include "Mesh.h"
#include <vector>
#include <chrono>
#include "Transformer.h"
#include "Texture.h"
#include "Vertex.h"
#include "ParallelLight.h"
#include "PhongShader.h"
#include "PointLight.h"
#include "FrameBuffer.h"
#include "ModelLoader.h"
#include "glm/glm.hpp"
#include "CubeMap.h"
#include <iostream>
#include "SkyBoxShader.h"
#include "Clipper.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "Statistic.h"
namespace ylb
{
    struct RenderTargetSetting
    {
        bool open_depth_buffer_write = true;
        bool open_frame_buffer_write = true;
    };

    class Renderer
    {
    public:
        Renderer(int _w = 800, int _h = 600);
        void Start();
    
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
        Statistic statistic;
        void ProcessInput(double &&delta_time);
        void ProcessGeometry(ylb::Triangle &t, Shader*& shader, Light*& light);
        void Rasterization(ylb::Triangle &t, ylb::Shader*& shader, Light*& light);
        void Render(std::vector<ylb::Mesh> &ts);
        void Generate_ShadowMap(Light* light);

        //返回值:true->剔除
        bool Backface_culling(Vertex& vt);
    };
}
#endif