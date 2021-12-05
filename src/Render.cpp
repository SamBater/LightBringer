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
            sun->dir.y() += delta_time * 0.1f * ty;
        }
        if(tx)
        {
            auto sun = static_cast<ParalleLight*>(lights[0]);
            sun->dir.x() += delta_time * 0.1f * tx;
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
                Matrix4f model;
                set_identyti(model);
                translate(model,mesh.position_world * -1.0f);
                for (auto& t : triangles)
                {

                    Vec4f vertex_ss[3];
                    bool need_clip = false;
                    for (int i = 0; i < 3 && !need_clip; i++)
                    {
                        need_clip = !transformer->vertex_output(t.vts[i], world_pos, vertex_ss[i]);
//                        if(renderTargetSetting->open_frame_buffer_write)
//                        {
//                            vertex_ss[i] = mesh.shader->vertex_shading(t.vts[i], light);
//                            need_clip = vertex_ss[i].z() <= 0;
//                        }
                    }

                    if (need_clip)
                        continue;


                    //光源空间变换
                    if(renderTargetSetting->open_frame_buffer_write)
                    {
                        for(int i = 0 ; i < 3 ; i++)
                        {
                            Vec4f pos_h {t.vts[i].x(),t.vts[i].y(),t.vts[i].z(),1};
                            t.vts[i].l_pos =  transformer->view_port * light->lvp * model * pos_h;
                            t.vts[i].l_pos = t.vts->l_pos * t.vts[i].inv;
                        }
                    }

                    //NDC->SCREEN
//                    for(int i = 0 ; i < 3 ; i++)
//                    {
//                        vertex_ss[i] = transformer->view_port * vertex_ss[i];
//                    }

                    //设置三角形,准备光栅化
                    t.ready_to_raser(vertex_ss);
                    render(t, mesh.shader,light);
                }
            }
        }
    }

    void Render::render(YYLB::Triangle &t,Shader *&shader,Light*& light)
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
                        //深度写入
                        if(renderTargetSetting->open_z_buffer_write)
                        {
                            frame_buffer->set_depth(x, y, depth);
                        }
                        //帧缓存写入
                        if(renderTargetSetting->open_frame_buffer_write)
                        {
                            float visibility = 1, depth_light_pos,depth_shadow_map;

//                            auto light_pos = t.interpolated_world_position();
//                            Vec4f light_pos_h{light_pos.x(),light_pos.y(),light_pos.z(),1};

                            float s = 1 / (t.cof.x() * t.vts[0].inv + t.cof.y() * t.vts[1].inv + t.cof.z() * t.vts[2].inv);
                            //透视矫正 获取像素在
                            Vec4f v_light_pos ;//=transformer->view_port * light->lvp * light_pos_h;
                            v_light_pos.x() = t.vts[0].l_pos.x() * t.cof.x() + t.vts[1].l_pos.x() * t.cof.y() + t.vts[2].l_pos.x() * t.cof.z();
                            v_light_pos.y() = t.vts[0].l_pos.y() * t.cof.x() + t.vts[1].l_pos.y() * t.cof.y() + t.vts[2].l_pos.y() * t.cof.z();
                            v_light_pos.z() = t.vts[0].l_pos.z() * t.cof.x() + t.vts[1].l_pos.z() * t.cof.y() + t.vts[2].l_pos.z() * t.cof.z();
                            v_light_pos = v_light_pos * s;
                            v_light_pos.x() /= w;
                            v_light_pos.y() /= y;

                            int si = v_light_pos.y() * w + v_light_pos.x();
                            float shadow_bias = 0.05f;
                            if(si >= 0 && si < w * h)
                            {
                                depth_shadow_map = light->shadow_map->tex2d(v_light_pos.x(),v_light_pos.y()).z();
                                if(depth_shadow_map > depth_light_pos + shadow_bias)
                                    visibility = 0.5f;
                            }

                            color = shader->fragment_shading(t, light) * visibility;
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
        Vec3f camPos = {-1.5f, 6.5f, -80.f};
        cam = new Camera(camPos.x(), camPos.y(), camPos.z(), PI / 6, w * 1.f / h, 1.2, 1000);
        transformer->set_world_to_view(cam);
        transformer->set_view_to_project(cam,PROJECTION_MODE::PERSPECTIVE);
        transformer->set_projection_to_screen(w, h);
        Shader::camPos = &cam->position_world;

        using YYLB::Triangle;
        using YYLB::Vertex;
//        YYLB::ParalleLight *sun = new ParalleLight(1.5f, Vec3f{1, 1, 1}, Vec3f{0.5, -2,-2});
        YYLB::ParalleLight *sun = new ParalleLight(1.5f, Vec3f{1, 1, 1}, Vec3f{0.5, -2,-2});
        lights.push_back(sun);

        Vertex v1{ {5,-1,3} ,{0,1,0},{0,0} }, v2{ {5,-1,-10} ,{0,1,0},{0,0.5} }, 
                v3{ {-5,-1,3},{0,1,0},{0.5,0} }, v4{ {-5,-1,-10},{0,1,0},{0.5,0.5 } };

        std::vector<Triangle> ts2 = {
            YYLB::Triangle(v1, v2, v3),
            YYLB::Triangle(v4, v2, v3)
        };

        Texture* cb = new Texture("assets/cb.jpg");
        YYLB::Mesh tm(0,0,2.f,ts2);

        YYLB::Mesh wall(0,0,8,ts2);
        wall.shader = new PhongShader();
        wall.rotate(Vec3f{1,0,0},YYLB::PI/2);
        wall.scale_transform(2,1.5,2);
        world.push_back(wall);


        PhongShader *shader = new PhongShader();
        YYLB::Matrix4f m_i;
        YYLB::set_identyti(m_i);

        tm.shader = shader;
        tm.scale_transform(2,1,2);
        world.push_back(tm);

        Texture* t = new Texture("assets/uv.jpg");
        YYLB::Mesh cube(-2.f,0.0f,-4.f,LoadObj("assets/cube.obj"));
        cube.shader = new PhongShader(t);
        world.push_back(cube);

        YYLB::Mesh m1(4.f, 0.0f, -5.f, LoadObj("assets/monkey.obj"));
        m1.shader = new PhongShader();
        m1.rotate(Vec3f{0,1,0},YYLB::PI);
        world.push_back(std::move(m1));

        generate_shadow_map(sun);

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
//            frame_buffer->save("z_buffer.bmp", true);
//            exit(0);
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
        cam = new Camera(0,8,-60,YYLB::PI / 2,16/9.f,2,1000);
        cam->l = -20;cam->r = 20;
        cam->t = 20;cam->b = -20;
        cam->mode = PROJECTION_MODE::ORTHOGONAL;
        cam->look_at = light->LightDir(cam->position_world);
        transformer->set_view_to_project(cam,PROJECTION_MODE::ORTHOGONAL);
        transformer->set_world_to_view(cam);
        transformer->set_projection_to_screen(w,h);

        Vec2i size{256,256};
        unsigned char* shadow_map = new unsigned char[w*h*3];
        renderTargetSetting->open_frame_buffer_write = false;
        render(world);
        frame_buffer->save("shadowMap.bmp", false);
        for(int y = 0 ; y < h ; y++)
        {
            for(int x = 0 ; x < w ; x++)
            {
                int i = y*w*3+x*3;
                shadow_map[i] = (1 + frame_buffer->depth[y*w+x]) * 255;
                shadow_map[i+1] = (1 + frame_buffer->depth[y*w+x]) * 255;
                shadow_map[i+2] = (1 + frame_buffer->depth[y*w+x]) * 255;
            }
        }

        light->shadow_map = new Texture(shadow_map,w,h);
        light->lvp = transformer->projection * transformer->view;

        delete cam;
        cam = originCam;
        transformer->set_view_to_project(cam,mode);
        transformer->set_world_to_view(cam);
        renderTargetSetting->open_frame_buffer_write = true;
    }
}