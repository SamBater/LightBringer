#include <iostream>
#include "Core/Pipeline/Render.h"
namespace YYLB
{
    void Render::processInput(double &&delta_time)
    {
        double move_speed = 10.0 * delta_time;
        double rot_speed = YYLB::PI / 3 * delta_time;
        int dx = glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS   ? -1
                 : glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS ? 1
                                                                : 0;
        int dz = glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS   ? 1
                 : glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS ? -1
                                                                : 0;
        int dy = glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS ? 1 : 0;
        dy = glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS ? -1 : dy;

        int ty = glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS ? 1 : 0;
        ty = glfwGetKey(window, GLFW_KEY_RIGHT) ? -1 : ty;

        int tx = glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS ? 1 : 0;
        tx = glfwGetKey(window, GLFW_KEY_DOWN) ? -1 : tx;
        if (ty)
        {
            auto sun = static_cast<ParalleLight*>(lights[0]);
            sun->dir.y += delta_time * 0.1f * ty;
        }
        if(tx)
        {
            auto sun = static_cast<ParalleLight*>(lights[0]);
            sun->dir.x += delta_time * 0.1f * tx;
        }
        if (dx || dz || dy)
        {
            glm::vec3 vx = glm::vec3 {1,0,0} * static_cast<float>(dx * move_speed);
            glm::vec3 vz = glm::vec3 {0,0,1} * static_cast<float>(dz * move_speed);
            glm::vec3 vy = glm::vec3 {0,1,0} * static_cast<float>(dy * move_speed);
            cam->position_world = cam->position_world + vx + vz + vy;
            transformer->set_world_to_view(cam);
            std::cerr << "move\n";
        }
    }

    void Render::render(std::vector<YYLB::Mesh> &meshs)
    {
        for (auto& light : lights)
        {
            for (auto& mesh : meshs)
            {
                auto world_pos = mesh.getPos();
                auto triangles = mesh.get_triangles();
                glm::mat4 model(1);
                model[0][3] = world_pos.x;
                model[1][3] = world_pos.y;
                model[2][3] = world_pos.z;
                for (auto& t : triangles)
                {
                    glm::vec4 vertex_ss[3];
                    bool need_clip = false;
                    for (int i = 0; i < 3 && !need_clip; i++)
                    {
                        t.vts[i].position_world = t.vts[i].position + world_pos;
                        need_clip = !transformer->vertex_output(t.vts[i], world_pos, vertex_ss[i],cam->mode);
                    }

                    if (need_clip)
                        continue;

                    //光源空间变换
                    if(renderTargetSetting->open_frame_buffer_write)
                    {
                        for(int i = 0 ; i < 3 ; i++)
                        {
                            glm::vec4 pos = glm::vec4 (t.vts[i].position,1);
                            t.vts[i].l_pos =pos * model * light->vp;
                            t.vts[i].l_pos *= t.vts[i].inv;
                        }
                    }

                    //设置三角形,准备光栅化
                    t.ready_rasterization(vertex_ss);
                    render(t, mesh.shader,light);
                }
            }
        }
    }

    void Render::render(YYLB::Triangle &t,Shader *&shader,Light*& light)
    {

        const YYLB::BoundingBox *bb = t.bounding_box();
        glm::vec3 color = {0.5, 0.5, 0.5};
        float zero = 0;
        for (int y = YYLB::max(bb->bot,zero); y < bb->top; y++)
            for (int x = YYLB::max(bb->left,zero); x < bb->right; x++)
            {
                int pixel = y * w + x;
                //三角形测试
                if (YYLB::Triangle::inside(x + 0.5f, y + 0.5f, t))
                {
                    float depth = 0;

                    if(cam->mode == PROJECTION_MODE::PERSPECTIVE)
                        depth = t.interpolated_depth();
                    else
                        depth = t.vts[0].sz() * t.cof.x + t.vts[1].sz() * t.cof.y + t.vts[2].sz() * t.cof.z;
                    //深度测试
                    if (depth - frame_buffer->depth[pixel] > YYLB::eps)
                    {
                        //深度写入
                        if(renderTargetSetting->open_z_buffer_write)
                        {
                            frame_buffer->set_depth(x, y, depth);
                        }
                        //帧缓存写入
                        if(renderTargetSetting->open_frame_buffer_write)
                        {
                            color = shader->fragment_shading(t, light);
                            frame_buffer->set_color(x, y, color);
                        }
                    }
                }
            }
    }

    void Render::start()
    {
        if (window == nullptr)
            return;

        //准备物体

        //设置渲染状态
//        glm::vec3 camPos = {-1, 1, -70.f};
        glm::vec3 camPos = {0, 4, -70.f};
        cam = new Camera(camPos.x, camPos.y, camPos.z, PI / 4, w * 1.f / h, 0.2, 1000);
        cam->look_at = {-4,2,1};
//        cam->look_at = {0,0,-1};
        cam->look_at = glm::normalize(cam->look_at);
        transformer->set_world_to_view(cam);
        transformer->set_view_to_project(cam,PROJECTION_MODE::PERSPECTIVE);
        transformer->set_projection_to_screen(w, h);
        Shader::camPos = &cam->position_world;

        using YYLB::Triangle;
        using YYLB::Vertex;
        YYLB::ParalleLight *sun = new ParalleLight(1.5f, glm::vec3{1, 1, 1}, glm::vec3{0, -2,-2});
//        YYLB::ParalleLight *sun = new ParalleLight(1.5f, glm::vec3{1, 1, 1}, glm::vec3{-0.7, -1,0.63});
        sun->dir = glm::normalize(sun->dir);
        lights.push_back(sun);

        Vertex v1{ {5,-1,3} ,{0,1,0},{0,0} }, v2{ {5,-1,-10} ,{0,1,0},{0,0.5} }, 
                v3{ {-5,-1,3},{0,1,0},{0.5,0} }, v4{ {-5,-1,-10},{0,1,0},{0.5,0.5 } };

        std::vector<Triangle> ts2 = {
            YYLB::Triangle(v1, v2, v3),
            YYLB::Triangle(v4, v2, v3)
        };

        Texture* cb = new Texture("assets/cb.jpg");
        Texture* brick_wall = new Texture("assets/brick_wall.png");
        Texture* t = new Texture("assets/uv.jpg");


        YYLB::Mesh tm(0,0,2.f,ts2);
        PhongShader *shader = new PhongShader();
        tm.shader = shader;
        tm.scale_transform(3,1.5,3);
        world.push_back(tm);

        auto cubeTs = LoadObj("assets/cube.obj");

//        for(auto &t : cubeTs)
//        {
//            for(auto& v : t.vts)
//            {
//                v.set_uv(1-v.u(),v.v());
//            }
//        }

        YYLB::Mesh cube(-4.f,0,-16.f,cubeTs);
        cube.scale_transform(2,2,2);
        cube.shader = new PhongShader(t);
        world.push_back(cube);

        YYLB::Mesh m1(5.f, -0.5f, -16.f, LoadObj("assets/sphere.obj"));
        m1.shader = new PhongShader();
        world.push_back(std::move(m1));

        Mesh wall(0,6,13,cubeTs);
        wall.shader = new PhongShader();
        wall.scale_transform(16,8,2);
        world.push_back(wall);

        generate_shadow_map(sun);

        char str[256] = "";
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
            end = std::chrono::high_resolution_clock::now();
            delta_time = end - start;
            start = std::chrono::high_resolution_clock::now();
            sprintf(str, "%s delta_time:%.3lf", title, delta_time.count());
            glfwSetWindowTitle(window, str);

            glfwSwapBuffers(window);
            glfwPollEvents();
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

        window = glfwCreateWindow(w, h, title, NULL, NULL);
        if (!window)
        {
            printf("Couldn't open window\n");
            return;
        }
        glfwMakeContextCurrent(window);
        frame_buffer = new FrameBuffer(w, h);
        transformer = new Transformer();
        renderTargetSetting = new RenderTargetSetting();
    }

    void Render::generate_shadow_map(Light* light) {
        frame_buffer->clear();
        Camera *originCam = cam;
        auto mode = cam->mode;
        cam = new Camera(0,10,-25,YYLB::PI / 2 ,16/9.f,0,100);
        cam->l = -40;cam->r = 40;
        cam->t = 40;cam->b = -40;
        cam->mode = PROJECTION_MODE::ORTHOGONAL;
        cam->look_at = light->LightDir(cam->position_world);
        transformer->set_view_to_project(cam,PROJECTION_MODE::ORTHOGONAL);
        transformer->set_world_to_view(cam);
        transformer->set_projection_to_screen(w,h);

        glm::vec<2,int> size{256,256};
        unsigned char* shadow_map = new unsigned char[w*h*3];

        renderTargetSetting->open_frame_buffer_write = false;
        render(world);
        for(int y = 0 ; y < h ; y++)
        {
            for(int x = 0 ; x < w ; x++)
            {
                int i = y*w*3+x*3;
                shadow_map[i] = shadow_map[i+1] = shadow_map[i+2] = (1+frame_buffer->depth[y*w+x] ) * 127;
            }
        }

        frame_buffer->save_zbuffer("shadow_map.bmp", false);
        light->shadow_map = new Texture(shadow_map,w,h);
        light->vp = transformer->view * transformer->projection;

        delete cam;
        cam = originCam;
        transformer->set_view_to_project(cam,originCam->mode);
        transformer->set_world_to_view(cam);
        transformer->set_projection_to_screen(w,h);
        renderTargetSetting->open_frame_buffer_write = true;
    }

    bool Render::back_face_culling(Vertex &vt) {
        return false;
    }
}