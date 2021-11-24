#include "Core/Pipeline/Render.h"
namespace YYLB
{

    void Render::processInput(double &&delta_time)
    {
        double move_speed = 5.0 * delta_time;
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
        auto pos = cam->getPos();
        if (ty)
        {
            world[0].rotate(rot_speed * ty);
        }
        if(tx)
        {
            world[0].rotate(rot_speed * tx);
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
                        need_clip = !transformer->vertex_output(t.vts[i], world_pos, vertex_ss[i]);
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

        using YYLB::Triangle;
        using YYLB::Vertex;       
        YYLB::ParalleLight *sun = new ParalleLight(1.5f, Vec3f{1, 1, 1}, Vec3f{0.44f, -0.35f, -0.33f});
        //YYLB::ParalleLight* sun = new ParalleLight(1.1f, Vec3f{ 1, 1, 1 }, Vec3f{ -0.13f, -0.77f, -0.23f });
        lights.push_back(sun);

        YYLB::PointLight *point_light = new YYLB::PointLight(5.1f, Vec3f{1, 0, 1});
        point_light->setPos(2.f,6.5f, -6.f);
         lights.push_back(point_light);

        Vertex v1{ {5,-1,3} ,{0,1,0},{0,0} }, v2{ {5,-1,-10} ,{0,1,0},{0,0.5} }, 
                v3{ {-5,-1,3},{0,1,0},{0.5,0} }, v4{ {-5,-1,-10},{0,1,0},{0.5,0.5 } };
        
        std::vector<Triangle> ts2 = {
            YYLB::Triangle(v1, v2, v3),
            YYLB::Triangle(v4, v2, v3)
        };

        Texture* t = new Texture("assets/uv.jpg");
        Texture* checkBoard = new Checkboard();
        Texture* noise = new NoiseMap();
        YYLB::Mesh cube(-7.f,4.0f,-7.f,LoadObj("assets/cube.obj"));
        cube.shader = new PhongShader(noise);
        world.push_back(cube);

        //YYLB::Mesh m1(-10.f, 5.0f, -7.f, LoadObj("assets/monkey.obj"));
        //m1.shader = new PhongShader();
        //world.push_back(std::move(m1));

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

        //设置渲染状态
        Vec3f camPos = {-10.f, 5.f, 1.f};
        cam = new Camera(camPos.x(), camPos.y(), camPos.z(), PI / 3, w * 1.f / h, 0.8f, 1000.f);
        Shader::camPos = &cam->position_world;
        transformer = new Transformer();
        transformer->set_world_to_view(cam);
        transformer->set_view_to_project(cam);
        transformer->set_projection_to_screen(w, h);
    }
}