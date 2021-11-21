#include "Core/Light.h"
#include "Core/ParallelLight.h"
#include "Core/Texture.h"
#include "GLFW/glfw3.h"
#include "math/Common.h"
#include "math/Matrix.h"
#include "math/Vertex.h"
#include <Core/Render.h>
namespace YYLB
{

    void Render::processInput(double&& delta_time)
    {
        double move_speed = 10.0 * delta_time;
        double rot_speed = YYLB::PI / 6 * delta_time;
        int dx = glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS    ? -1
                 : glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS ? 1
                                                                    : 0;
        int dz = glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS     ? 1
                 : glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS ? -1
                                                                   : 0;
        int dy = glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS ? 1 : 0;
        dy = glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS ? -1 : dy;

        
        int ty = glfwGetKey(window,GLFW_KEY_LEFT) == GLFW_PRESS ? 1 : 0;
        ty = glfwGetKey(window,GLFW_KEY_RIGHT) ? -1 : ty;
        auto pos = cam->getPos();
        if(ty)
        {   
            world[0].rotate(rot_speed * ty);
        }    
        if (dx || dz || dy)
        {
            cam->setPos(pos.x() + dx * move_speed, pos.y() + dy * move_speed, pos.z() + dz * move_speed);
            transformer->set_world_to_view(cam);
            transformer->set_view_to_project(cam);
        }
    }

    void Render::render(std::vector<YYLB::Mesh> &meshs)
    {
        for (auto &mesh : meshs)
        {
            auto world_pos = mesh.getPos();
            auto triangles = mesh.get_triangles();

            for (auto &t : triangles)
            {

                Vec4f vertex_ss[3];
                bool need_clip = false;
                for (int i = 0; i < 3 && !need_clip; i++)
                {
                    need_clip = !transformer->vertex_output(t.vts[i], world_pos, vertex_ss[i]);
                }

                if (need_clip)
                    continue;

                //设置三角形,准备光栅化
                t.ready_to_raser(vertex_ss);
                render(t);
            }
        }
    }

    void Render::render(YYLB::Triangle &t)
    {
        auto shader = world[0].shader;
        const YYLB::BoundingBox *bb = t.bounding_box();
        Vec3f color = {0.5, 0.5, 0.5};
        for (int y = bb->bot; y < bb->top; y++)
            for (int x = bb->left; x < bb->right; x++)
            {
                int pixel = y * w + x;
                //三角形测试
                if (YYLB::Triangle::inside(x + 0.5f, y + 0.5f, t))
                {
                    float depth = t.interpolated_depth();
                    //深度测试
                    if (depth - frame_buffer->depth[pixel] > YYLB::eps)
                    {
                        color = shader->fragment_shading(t,&lights[0]);
                        frame_buffer->set_depth(x, y, depth);
                        frame_buffer->set_color(x, y, color);
                    }
                }
            }
    }

    void Render::start()
    {
        if (window == nullptr)
            return;

        //准备物体

        using YYLB::Triangle;
        using YYLB::Vertex;
        std::vector<Triangle> ts;

        //底面
        ts.push_back(Triangle(Vertex({-1,-1,-1},{0.f,-1,0.f},{0.f,0.f}),
                              Vertex({-1,-1,1},{0.f,-1,0},{0,0.5f}),
                              Vertex({1,-1,1},{0,-1,0},{0.5f,0.5f})  ));

        ts.push_back(Triangle(Vertex({-1,-1,-1},{0.f,-1,0.f},{0.f,0.f}),
        Vertex({1,-1,-1},{0.f,-1,0},{0.5f,0.f}),
        Vertex({1,-1,1},{0,-1,0},{0.5f,0.5f})  ));

        //顶面
                ts.push_back(Triangle(Vertex({-1,1,-1},{0.f,1,0.f},{0.f,0.f}),
                              Vertex({-1,1,1},{0.f,1,0},{0,0.5f}),
                              Vertex({1,1,1},{0,1,0},{0.5f,0.5f})  ));

        ts.push_back(Triangle(Vertex({-1,1,-1},{0.f,1,0.f},{0.f,0.f}),
        Vertex({1,1,-1},{0.f,1,0},{0.5f,0.f}),
        Vertex({1,1,1},{0,1,0},{0.5f,0.5f})  ));

        //右面
        ts.push_back(Triangle(Vertex({1,1,1},{1.f,0.f,0.f},{0.f,0.f}),
        Vertex({1,1,-1},{1.f,0,0},{0.5f,0.f}),
        Vertex({1,-1,1},{1.f,0,0},{0.f,0.5f})  ));

        ts.push_back(Triangle(Vertex({1,-1,-1},{1.f,0.f,0.f},{0.5f,0.5f}),
        Vertex({1,1,-1},{1.f,0,0},{0.5f,0.f}),
        Vertex({1,-1,1},{1,0,0},{0.f,0.5f})  ));

        //左面
        ts.push_back(Triangle(Vertex({-1,1,1},{-1.f,0.f,0.f},{0.f,0.f}),
        Vertex({-1,1,-1},{-1.f,0,0},{0.5f,0.f}),
        Vertex({-1,-1,1},{-1.f,0,0},{0.f,0.5f})  ));

        ts.push_back(Triangle(Vertex({-1,-1,-1},{-1.f,0.f,0.f},{0.5f,0.5f}),
        Vertex({-1,1,-1},{-1.f,0,0},{0.5f,0.f}),
        Vertex({-1,-1,1},{-1,0,0},{0.f,0.5f})  ));


        //背面
                    ts.push_back(Triangle(Vertex({-1,1,-1},{0.f,0.f,-1.f},{0.f,0.f}),
        Vertex({1,1,-1},{0.f,0,-1.f},{0.5f,0.f}),
        Vertex({-1,-1,-1},{0,0.f,-1.f},{0.f,0.5f})  ));

        ts.push_back(Triangle(Vertex({1,-1,-1},{0.f,0.f,-1.f},{0.5f,0.5f}),
        Vertex({1,1,-1},{0.f,0,-1.f},{0.5f,0.f}),
        Vertex({-1,-1,-1},{0,0.f,-1.f},{0.f,0.5f})  ));

        ts.push_back(Triangle(Vertex({-1,1,1},{0.f,0.f,1.f},{0.f,0.f}),
        Vertex({1,1,1},{0.f,0,1.f},{0.5f,0.f}),
        Vertex({-1,-1,1},{0,0.f,1.f},{0.f,0.5f})  ));

        ts.push_back(Triangle(Vertex({1,-1,1},{0.f,0.f,1.f},{0.5f,0.5f}),
        Vertex({1,1,1},{0.f,0,1.f},{0.5f,0.f}),
        Vertex({-1,-1,1},{0,0.f,1.f},{0.f,0.5f})  ));
        

        YYLB::ParalleLight sun;
        sun.setPos(10.5f,1.f,10.5f);
        sun.dir = Vec3f{-0.15f,0.25f,0};
        lights.push_back(sun);

        YYLB::Mesh m1(2.5f, 1.0f, -4.f, std::move(ts));
        auto t = new Texture("Img/uv.jpg");
        m1.shader = new SimpleShader(t);
        char *str = new char[256];

        world.push_back(std::move(m1));
        auto start = std::chrono::high_resolution_clock::now();
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> delta_time = end - start;
        while (!glfwWindowShouldClose(window))
        {
            processInput(delta_time.count());
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            frame_buffer->clear();

            render(world);
            glDrawPixels(w, h, GL_RGB, GL_UNSIGNED_BYTE, frame_buffer->pixels);

            glfwSwapBuffers(window);
            glfwPollEvents();
            
            end = std::chrono::high_resolution_clock::now(); 
            delta_time = end - start;
            start = std::chrono::high_resolution_clock::now();
            sprintf(str, "%s delta_time:%.3lf",title,delta_time.count());
            glfwSetWindowTitle(window, str);
            
        }
    }

    Render::Render(int _w, int _h) : w(_w), h(_h)
    {
        //设置opengl
        if (!glfwInit())
        {
            printf("Couldn't init GLFW\n");
            return;
        }

        window = glfwCreateWindow(w, h,title, NULL, NULL);
        if (!window)
        {
            printf("Couldn't open window\n");
            return;
        }
        glfwMakeContextCurrent(window);
        int length = w * h;

        frame_buffer = new FrameBuffer(w, h);

        //设置渲染状态
        Vec3f camPos = {2.5f,1.f,1.f};
        cam = new Camera(camPos.x(), camPos.y(),camPos.z(), PI / 2, w * 1.f / h, 2.f, -400.f);
        transformer = new Transformer();
        transformer->set_world_to_view(cam);
        transformer->set_view_to_project(cam);
        transformer->set_projection_to_screen(w, h);
    }
}