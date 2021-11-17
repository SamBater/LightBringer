#include <Core/Render.h>

namespace YYLB
{
    void Render::processInput()
    {
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        {
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

                Vec4f vertex_ss[3] =
                    {
                        transformer->vertex_output(t.v3d_local[0], world_pos),
                        transformer->vertex_output(t.v3d_local[1], world_pos),
                        transformer->vertex_output(t.v3d_local[2], world_pos)};

                //设置三角形,准备光栅化
                t.ready_to_raser(vertex_ss);
                render(t);
            }
        }
    }

    template <typename T>
    T &clamp(T val, const T min, const T max)
    {
        if (val > max)
            val = max;
        if (val < min)
            val = min;
        return val;
    }

    void Render::render(YYLB::Triangle &t)
    {
        //包围盒AABB
        const YYLB::BoundingBox *aabb = t.bounding_box();
        int topY = aabb->top;
        int botY = aabb->bot;
        int rightX = aabb->right;
        int leftX = aabb->left;

        Vec3f cof;
        // #pragma omp parallel for num_threads(2)
        for (int y = botY; y < topY; y++)
            for (int x = leftX; x < rightX; x++)
            {
                int pixel = y * w + x;
                if (YYLB::Triangle::inside(x + 0.5f, y + 0.5f, t, cof))
                {
                    float depth = t.v2d[0].z() * cof.x() + t.v2d[1].z() * cof.y() + t.v2d[2].z() * cof.z();
                    unsigned char r = cof.x() * 255, g = cof.y() * 255, b = cof.z() * 255;
                    if (depth - frame_buffer->depth[pixel] > YYLB::eps)
                    {
                        frame_buffer->set_depth(x, y, depth);
                        frame_buffer->set_color(x, y, r, g, b);
                    }
                }
            }
    }

    void Render::start()
    {
        if (window == nullptr)
            return;

        //准备物体

        std::vector<YYLB::Triangle> ts{
            YYLB::Triangle({-1, 1, 1}, {-1, -1, 1}, {1, -1, 1}, {0xff, 0, 0}),
            YYLB::Triangle({-1, 1, 1}, {1, 1, 1}, {1, -1, 1}, {0xff, 0, 0}),
            YYLB::Triangle({-1, -1, -1}, {-1, -1, 1}, {1, -1, 1}, {0xcc, 0xff, 0xff}),
            YYLB::Triangle({-1, -1, -1}, {1, -1, 1}, {1, -1, -1}, {0xcc, 0xff, 0xff}),
            YYLB::Triangle({-1, 1, 1}, {-1, -1, 1}, {-1, -1, -1}, {0, 0, 0xff}),
            YYLB::Triangle({-1, 1, 1}, {-1, 1, -1}, {-1, -1, -1}, {0, 0, 0xff}),
            YYLB::Triangle({1, 1, 1}, {1, -1, 1}, {1, -1, -1}, {0xff, 0xff, 0}),
            YYLB::Triangle({1, 1, 1}, {1, 1, -1}, {1, -1, -1}, {0xff, 0xff, 0}),
            YYLB::Triangle({-1, 1, -1}, {-1, 1, 1}, {1, 1, 1}, {0xff, 0, 0xff}),
            YYLB::Triangle({-1, 1, -1}, {1, 1, -1}, {1, 1, 1}, {0xff, 0, 0xff}),
            YYLB::Triangle({-1, 1, -1}, {-1, -1, -1}, {1, -1, -1}, {0xff, 0xff, 0xff}),
            YYLB::Triangle({-1, 1, -1}, {1, 1, -1}, {1, -1, -1}, {0xff, 0xff, 0xff})};
        YYLB::Mesh m1(1, 5, -10, std::move(ts));
        std::vector<Mesh> world;
        world.push_back(std::move(m1));

        auto start = std::chrono::high_resolution_clock::now();
        int cnt = 0;

        Vec4f pos{0, 0, -15};
        // auto rot = rotation_y_matrix4f(PI / 6);
        // pos = rot * pos;
        while (!glfwWindowShouldClose(window) && cnt < 120)
        {
            world[0].setPos(pos.x(), pos.y(), pos.z());

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            frame_buffer->clear();
            //processInput();
            render(world);
            glDrawPixels(w, h, GL_RGB, GL_UNSIGNED_BYTE, frame_buffer->pixels);

            glfwSwapBuffers(window);
            cnt++;
        }

        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duration = end - start;
        printf("fps:%lf\n", std::chrono::duration<double>(cnt) / duration);
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
        cam = new Camera(1, 5, 1, PI / 6, w * 1.f / h, 2.f, 400.f);
        transformer = new Transformer();
        transformer->set_world_to_view(cam);
        transformer->set_view_to_project(cam);
        transformer->set_projection_to_screen(w, h);
    }
}