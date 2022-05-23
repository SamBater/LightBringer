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
#include "Model.h"
#include <vector>
#include <chrono>
#include "Texture.h"
#include "Vertex.h"
#include "ParallelLight.h"
#include "PhongShader.h"
#include "PointLight.h"
#include "FrameBuffer.h"
#include "glm/fwd.hpp"
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
        bool back_face_culling = true;
    };

    class Renderer
    {
    public:
        static Renderer& Instance() {
            static Renderer instance(600,600);
            return instance;
        }
        void Start();
    
    private:
        Renderer(int _w = 600, int _h = 600);
        int w;
        int h;
        const char *title = "Light Bringer - Made By YBT";
        GLFWwindow *window = nullptr;
        Camera *cam = nullptr;
        glm::mat4 view_port;
        FrameBuffer *frame_buffer = nullptr;
        RenderTargetSetting* renderTargetSetting = new RenderTargetSetting();
        std::vector<Model> models;
        std::vector<Light *> lights;
        Statistic statistic;
        
        void InitOpenGL();
        void ProcessInput(double &&delta_time);
        static void Framebuffer_Size_Callback(GLFWwindow *window, int width, int height);
        static void Mouse_Move_Callback(GLFWwindow* window, double xposIn, double yposIn);

        void LoadScene(const char* scene_file_path);
        void SetViewPort(int width,int height);
        bool BackFaceCulling(const glm::vec3 world_pos[]);
        void ProcessGeometry(ylb::Triangle &t, Shader*& shader, VertexShaderContext& vertexShaderContext);
        void Rasterization(ylb::Triangle &t, ylb::Shader* shader, Light* light);
        void Render(std::vector<ylb::Model> &ts);
        void GenerateShadowMap();
    };
}
#endif