#include "Core/Pipeline/Render.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "Image/stb_image_write.h"
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

        }
        if(tx)
        {

        }
        if (dx || dz || dy)
        {
            Vec3f vx = Vec3f {1,0,0} * dx * move_speed;
            Vec3f vz = Vec3f {0,0,1} * dz * move_speed;
            Vec3f vy = Vec3f {0,1,0} * dy * move_speed;
            cam->position_world = cam->position_world + vx + vz + vy;
            transformer->set_world_to_view(cam);
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

                for (auto& t : triangles)
                {

                    Vec4f vertex_ss[3];
                    bool need_clip = false;
                    for (int i = 0; i < 3 && !need_clip; i++)
                    {
                        need_clip = !transformer->vertex_output(cam,t.vts[i], world_pos, vertex_ss[i]);
                        t.vts[i].color = mesh.shader->shading(t.vts[i], light);
                    }

                    if (need_clip)
                        continue;

                    //设置三角形,准备光栅化
                    t.ready_to_raser(vertex_ss);
                    render(t, mesh.shader);
                }
            }
        }
    }

    void Render::render(YYLB::Triangle &t,Shader *&shader)
    {
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
                        color = shader->fragment_shading(t, lights[0]);
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

        //设置渲染状态
        Vec3f camPos = {0.f, 0.f, -7.f};
        cam = new Camera(camPos.x(), camPos.y(), camPos.z(), PI / 6, w * 1.f / h, 1, -10.f);
        transformer->set_world_to_view(cam);
        transformer->set_view_to_project(cam,PROJECTION_MODE::PERSPECTIVE);
//        transformer->set_view_to_project(cam,PROJECTION_MODE::ORTHOGONAL);
        transformer->set_projection_to_screen(w, h);
        Shader::camPos = &cam->position_world;

        using YYLB::Triangle;
        using YYLB::Vertex;       
        YYLB::ParalleLight *sun = new ParalleLight(1.5f, Vec3f{1, 1, 1}, Vec3f{0.33f, -0.35f, 0.33f});
        lights.push_back(sun);

        Vertex v1{ {5,-1,3} ,{0,1,0},{0,0} }, v2{ {5,-1,-10} ,{0,1,0},{0,0.5} }, 
                v3{ {-5,-1,3},{0,1,0},{0.5,0} }, v4{ {-5,-1,-10},{0,1,0},{0.5,0.5 } };
        
        std::vector<Triangle> ts2 = {
            YYLB::Triangle(v1, v2, v3),
            YYLB::Triangle(v4, v2, v3)
        };

        Mesh tm(0.5,0.0,1.f,ts2);
        tm.shader = new PhongShader();
        world.push_back(tm);

        Texture* noise = new NoiseMap();
        YYLB::Mesh cube(0.f,0.0f,5.f,LoadObj("assets/cube.obj"));
        cube.shader = new PhongShader();
        world.push_back(cube);

        YYLB::Mesh m1(0.f, 1.0f, -1.f, LoadObj("assets/monkey.obj"));
        m1.shader = new PhongShader();
        m1.rotate(YYLB::PI);
        world.push_back(std::move(m1));



        //生成shadowMap
        Vec3f origin_look_at = cam->look_at;
        cam->look_at = sun->dir; cam->look_at.z() *= -1.f;
        transformer->set_view_to_project(cam,PROJECTION_MODE::ORTHOGONAL);
        transformer->set_world_to_view(cam);
        Vec2i shadow_size{w ,h};
        unsigned char *buffer = new unsigned char[shadow_size.x() * shadow_size.y() * 3];
        frame_buffer->clear();
        for(auto& mesh : world)
        {
            auto world_pos = mesh.getPos();
            for(auto& t : mesh.triangles)
            {
                Vec4f vertex_ss[3];
                bool need_clip = false;
                for (int i = 0; i < 3 && !need_clip; i++)
                {
                    need_clip = !transformer->vertex_output(cam,t.vts[i], world_pos, vertex_ss[i]);
                }

                if (need_clip)
                    continue;

                //设置三角形,准备光栅化
                t.ready_to_raser(vertex_ss);
                const YYLB::BoundingBox *bb = t.bounding_box();
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
                                frame_buffer->set_depth(x, y, depth);
                            }
                        }
                    }
            }
        }

        for(int y = 0 ; y < shadow_size.y() ; y++)
        {
            for(int x = 0 ; x < shadow_size.x() ; x++)
            {
                int i = y * shadow_size.x() * 3 + x * 3;
                float depth = frame_buffer->depth[y*w+x];
                depth -= 1;
                unsigned char color = 0;
                if(depth > -10)color = (1+depth/11) * 255;
                buffer[i] = color;
                buffer[i+1] = color;
                buffer[i+2] = color;
            }
        }

        stbi__flip_vertically_on_write = 1;
        stbi_write_bmp("shadowMap.bmp",shadow_size.x(),shadow_size.y(),3,buffer);

        cam->look_at = origin_look_at;
        transformer->set_view_to_project(cam,PROJECTION_MODE::PERSPECTIVE);
//        transformer->set_view_to_project(cam,PROJECTION_MODE::ORTHOGONAL);
        transformer->set_world_to_view(cam);

        char* str = new char[256];
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

//            Vec2i shadow_size{w ,h };
//            unsigned char *buffer = new unsigned char[shadow_size.x() * shadow_size.y() * 3];
//            float max = -9999;
//            float min = 1000;
//            for(int y = 0 ; y < shadow_size.y() ; y++)
//            {
//                for(int x = 0 ; x < shadow_size.x() ; x++)
//                {
//                    int i = y * shadow_size.x() * 3 + x * 3;
//                    float depth = frame_buffer->depth[y*w+x];
//                    depth -= 1;
//                    max = std::max(max,depth);
//                    if(depth > - 1000 )min = std::min(min,depth);
//                    unsigned char color = 0;
//                    if(depth > -10)color = (1+depth/11) * 255;
//                    buffer[i] = color;
//                    buffer[i+1] = color;
//                    buffer[i+2] = color;
//                }
//            }
//
//            stbi__flip_vertically_on_write = 1;
//            stbi_write_bmp("shadowMap.bmp",shadow_size.x(),shadow_size.y(),3,buffer);
//            stbi_write_bmp("pixel.bmp",w,h,3,frame_buffer->pixels);
//            exit(0);


            glfwSwapBuffers(window);
            glfwPollEvents();


            end = std::chrono::high_resolution_clock::now();
            delta_time = end - start;
            start = std::chrono::high_resolution_clock::now();
            sprintf(str, "%s delta_time:%.3lf", title, delta_time.count());
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

        window = glfwCreateWindow(w, h, title, NULL, NULL);
        if (!window)
        {
            printf("Couldn't open window\n");
            return;
        }
        glfwMakeContextCurrent(window);
        frame_buffer = new FrameBuffer(w, h);
        transformer = new Transformer();

    }
}