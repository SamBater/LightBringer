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
#include "glm/glm.hpp"
#include "CubeMap.h"
#include <iostream>
#include "SkyBoxShader.h"
#include "Clipper.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "Statistic.h"
#include "SceneLoader.h"
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
        static Renderer& Instance() {
            static Renderer instance(1024, 768);
            return instance;
        }
        void Start();
    
    private:
        Renderer(int _w = 1024, int _h = 768);
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
        void InitOpenGL();
        void LoadScene(const char* scene_file_path);
        void SetMVPMatrix(Camera* cam,PROJECTION_MODE mode);
        void ProcessInput(double &&delta_time);
        static void Framebuffer_Size_Callback(GLFWwindow *window, int width, int height);
        void ProcessGeometry(ylb::Triangle &t, Shader*& shader, const VertexShaderContext& vertexShaderContext);
        void Rasterization(ylb::Triangle &t, ylb::Shader* shader, Light* light);
        void Render(std::vector<ylb::Mesh> &ts);

        //返回值:true->剔除
        bool Backface_culling(Vertex& vt);
    };
}
#endif