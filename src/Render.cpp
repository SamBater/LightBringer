#include <Core/Render.h>
namespace YYLB
{

    void Render::processInput()
    {
        static float move_speed = 0.1f;
        int dx = glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS    ? -1
                 : glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS ? 1
                                                                    : 0;
        int dz = glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS     ? 1
                 : glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS ? -1
                                                                   : 0;
        int dy = glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS ? 1 : 0;
        dy = glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS ? -1 : dy;
        if (dx || dz || dy)
        {
            auto pos = cam->getPos();
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

    YYLB::Texture tex("img/uv.jpg");
    void Render::render(YYLB::Triangle &t)
    {

        const YYLB::BoundingBox *bb = t.bounding_box();
        Vec3f cof;
        RGB color = {255, 255, 255};
        unsigned char i = 255;
        // #pragma omp parallel for num_threads(2)
        for (int y = bb->bot; y < bb->top; y++)
            for (int x = bb->left; x < bb->right; x++)
            {
                int pixel = y * w + x;
                //三角形测试
                if (YYLB::Triangle::inside(x + 0.5f, y + 0.5f, t, cof))
                {
                    float depth = t.interpolated_depth(cof);
                    //深度测试
                    if (depth - frame_buffer->depth[pixel] > YYLB::eps)
                    {
                        //unsigned char r = cof.x() * 255, g = cof.y() * 255, b = cof.z() * 255;
                        float u = 1, v = 1;

                        //纹理采样
                        t.interpolated_uv(cof, u, v);
                        tex.tex2d(u, v, color);
                        //color = {255.f * cof.x(), 255.f * cof.y(), 255.f * cof.z()};

                        //TODO:扩展为片段着色器
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

        float p[] = {1, -1};
        std::vector<YYLB::Vertex> vts;
        for (int x = 0; x < 2; x++)
        {
            for (int y = 0; y < 2; y++)
            {
                for (int z = 0; z < 2; z++)
                {
                    vts.push_back(YYLB::Vertex({p[x], p[y], p[z]}, {0.f, 0.f}));
                }
            }
        }

        std::vector<YYLB::Triangle> ts;

        auto foo = [&](int &&v1, int &&v2, int &&v3)
        {
            ts.push_back(YYLB::Triangle(vts[v1], vts[v2], vts[v3]));
        };
        vts[4].set_uv(0.f, 0.f);
        vts[6].set_uv(0.f, 0.125f);
        vts[2].set_uv(0.125f, 0.125f);
        vts[0].set_uv(0.125f, 0.f);
        vts[1].set_uv(0.25f, 0.0f);
        vts[3].set_uv(0.25f, 0.125f);
        vts[5].set_uv(0.375f, 0.0f);
        vts[7].set_uv(0.375f, 0.125f);
        foo(4, 6, 2);
        foo(4, 0, 2);
        foo(0, 2, 3);
        foo(0, 1, 3);
        foo(5, 7, 3);
        foo(5, 1, 3);
        foo(4, 6, 7);
        foo(4, 5, 7);
        foo(5, 4, 0);
        foo(5, 1, 0);
        foo(7, 6, 2);
        foo(7, 3, 2);

        YYLB::Mesh m1(2.5, 1, -7, std::move(ts));
        char *str = new char[256];
        int cnt = 0;
        world.push_back(std::move(m1));
        auto start = std::chrono::high_resolution_clock::now();
        auto end = std::chrono::high_resolution_clock::now();

        while (!glfwWindowShouldClose(window))
        {
            processInput();
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            frame_buffer->clear();

            render(world);
            glDrawPixels(w, h, GL_RGB, GL_UNSIGNED_BYTE, frame_buffer->pixels);

            glfwSwapBuffers(window);
            glfwPollEvents();

            cnt++;
            if (cnt % 10 == 0)
            {
                end = std::chrono::high_resolution_clock::now();
                std::chrono::duration<double> duration = end - start;
                start = std::chrono::high_resolution_clock::now();
                sprintf(str, "fps:%.1lf", 10.0 / duration.count());
                glfwSetWindowTitle(window, str);
            }
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

        window = glfwCreateWindow(w, h, "Light Bringer - Made By YBT", NULL, NULL);
        if (!window)
        {
            printf("Couldn't open window\n");
            return;
        }
        glfwMakeContextCurrent(window);
        int length = w * h;

        frame_buffer = new FrameBuffer(w, h);

        //设置渲染状态
        cam = new Camera(1, 1, 2.f, PI / 2, w * 1.f / h, 2.f, -400.f);
        transformer = new Transformer();
        transformer->set_world_to_view(cam);
        transformer->set_view_to_project(cam);
        transformer->set_projection_to_screen(w, h);
    }
}