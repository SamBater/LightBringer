#include "Renderer.h"
#include "Camera.h"
namespace ylb {
void Renderer::ProcessInput(double &&delta_time) {
    double move_speed = 10.0 * delta_time;
    double rot_speed = ylb::PI / 3 * delta_time;
    int dx = glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS ? -1 : glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS ? 1 :
                                                                                                                0;
    int dz = glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS ? 1 : glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS ? -1 :
                                                                                                               0;
    int dy = glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS ? 1 : 0;
    dy = glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS ? -1 : dy;

    int ty = glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS ? 1 : 0;
    ty = glfwGetKey(window, GLFW_KEY_RIGHT) ? -1 : ty;

    int tx = glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS ? 1 : 0;
    tx = glfwGetKey(window, GLFW_KEY_DOWN) ? -1 : tx;
    if (ty) {
        auto sun = static_cast<ParalleLight *>(lights[0]);
        sun->dir.y += static_cast<float>(delta_time * 0.1f * ty);
    }
    if (tx) {
        auto sun = static_cast<ParalleLight *>(lights[0]);
        sun->dir.x += static_cast<float>(delta_time * 0.1f * tx);
    }
    if (dx || dz || dy) {
        glm::vec3 vx = glm::vec3{1, 0, 0} * static_cast<float>(dx * move_speed);
        glm::vec3 vz = glm::vec3{0, 0, 1} * static_cast<float>(dz * move_speed);
        glm::vec3 vy = glm::vec3{0, 1, 0} * static_cast<float>(dy * move_speed);
        cam->position_world = cam->position_world + vx + vz + vy;
        transformer->set_world_to_view(cam);
    }
}

void Renderer::Render(std::vector<ylb::Mesh> &meshs) {
    statistic.InitTriangleCnt();
    for (auto &light : lights) {
        for (auto &mesh : meshs) {
            auto world_pos = mesh.getPos();
            auto triangles = mesh.get_triangles();

            //默认固定mvp
            mesh.shader->model = transformer->calc_matrix_world(mesh.position_world);
            mesh.shader->view = transformer->view;
            mesh.shader->projection = transformer->projection;
            for (auto &t : triangles) {
                ProcessGeometry(t, mesh.shader, light);
            }
        }
    }
}

void Renderer::Rasterization(ylb::Triangle &t, Shader *&shader, Light *&light) {
    statistic.IncreaseTriangleCnt();
    const ylb::BoundingBox *bb = t.bounding_box();
    glm::vec3 color = {0.5, 0.5, 0.5};
    int min_left = static_cast<int>(ylb::max(bb->bot, 0.f));
    for (int y = min_left; y < bb->top; y++)
        for (int x = bb->left; x < bb->right; x++) {
            int pixel = y * w + x;
            //三角形测试
            if (ylb::Triangle::inside(x + 0.5f, y + 0.5f, t)) {
                float depth = 0;

                if (cam->mode == PROJECTION_MODE::PERSPECTIVE)
                    depth = t.interpolated_depth();
                else
                    depth = t.vts[0].sz() * t.cof.x + t.vts[1].sz() * t.cof.y + t.vts[2].sz() * t.cof.z;
                //深度测试
                if (depth - frame_buffer->depth[pixel] >= ylb::eps) {
                    //深度写入
                    if (renderTargetSetting->open_depth_buffer_write) {
                        frame_buffer->set_depth(x, y, depth);
                    }
                    //帧缓存写入
                    if (renderTargetSetting->open_frame_buffer_write) {
                        color = shader->fragment_shading(t, light);
                        frame_buffer->set_color(x, y, color);
                    }
                }
            }
        }
}

void Renderer::SetMVPMatrix(Camera* cam,PROJECTION_MODE mode) {
    transformer->set_world_to_view(cam);
    transformer->set_view_to_project(cam, mode);
    transformer->set_projection_to_screen(w, h);
}

void Renderer::Start() {
    if (window == nullptr)
        return;

    //准备物体
    auto scene = LoadScene("Scene/sample.json");
    cam = std::move(scene->cam);
    //设置渲染状态
    
    SetMVPMatrix(cam,PROJECTION_MODE::PERSPECTIVE);
    
    Shader::camPos = &cam->position_world;

    using ylb::Triangle;
    using ylb::Vertex;
    ylb::ParalleLight *sun =
        new ParalleLight(1.5f, glm::vec3{1, 1, 1}, glm::vec3{0, -1, 2});
    //        ylb::ParalleLight *sun = new ParalleLight(1.5f, glm::vec3{1, 1, 1},
    //        glm::vec3{-0.7, -1,0.63});
    sun->dir = glm::normalize(sun->dir);
    lights.push_back(sun);

    Vertex v1{{5, -1, 3}, {0, 1, 0}, {0, 0}},
        v2{{5, -1, -10}, {0, 1, 0}, {0, 0.5}},
        v3{{-5, -1, 3}, {0, 1, 0}, {0.5, 0}},
        v4{{-5, -1, -10}, {0, 1, 0}, {0.5, 0.5}};

    std::vector<Triangle> ts2 = {ylb::Triangle(v1, v2, v3),
                                 ylb::Triangle(v4, v2, v3)};

    Texture *cb = new Texture("assets/cb.jpg");
    Texture *brick_wall = new Texture("assets/brick_wall.png");
    Texture *t = new Texture("assets/uv.jpg");
    Shader *shader = new PhongShader();
    Shader *cube_shader = new PhongShader(t);

    shader->view = transformer->view;
    shader->projection = transformer->projection;
    cube_shader->view = transformer->view;
    cube_shader->projection = transformer->projection;

    std::vector<std::string> cube_maps = {
        "assets/skybox/right.jpg", "assets/skybox/left.jpg",
        "assets/skybox/top.jpg", "assets/skybox/bottom.jpg",
        "assets/skybox/front.jpg", "assets/skybox/back.jpg"};
    CubeMap *skybox = new CubeMap(cube_maps);
    float skyboxVertices[] = {
        // positions
        -1.0f, 1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f, 1.0f, -1.0f,

        -1.0f, -1.0f, 1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, -1.0f,
        -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f, 1.0f,

        1.0f, -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f, 1.0f, -1.0f, 1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f,

        -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f};

    std::vector<ylb::Triangle> skybox_ts;
    int index = 0;
    auto getVal = [&]() { return skyboxVertices[index++]; };
    for (int i = 0; i < 12; i++) {
        ylb::Triangle triangle;
        for (int j = 0; j < 3; j++) {
            triangle.vts[j].position = glm::vec3(getVal(), getVal(), getVal());
        }
        skybox_ts.push_back(triangle);
    }

    ylb::Mesh skybox_mesh(0, 0, 0, skybox_ts);
    Shader *skybox_shader = new SkyBoxShader(skybox);
    skybox_mesh.shader = skybox_shader;
    //        world.push_back(skybox_mesh);
    std::vector<ylb::Mesh> world_only_sky;
    world_only_sky.push_back(skybox_mesh);

    ylb::Mesh tm(0, 0, 2.f, ts2);
    tm.shader = shader;
    tm.scale_transform(3, 1.5, 3);
    world.push_back(tm);

    auto cubeTs = LoadObj("assets/cube.obj");

    ylb::Mesh cube(-4.f, 0.6, -16.f, cubeTs);
    cube.scale_transform(2, 2, 2);
    cube.shader = cube_shader;
    world.push_back(cube);

    ylb::Mesh m1(5.f, -0.5f, -16.f, LoadObj("assets/sphere.obj"));
    m1.shader = shader;
    world.push_back(std::move(m1));

    Mesh wall(0, 6, 13, cubeTs);
    wall.shader = shader;
    wall.scale_transform(16, 8, 2);
    world.push_back(wall);

    char str[256] = "";
    auto start = std::chrono::high_resolution_clock::now();
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> delta_time = end - start;

    while (!glfwWindowShouldClose(window)) {
        ProcessInput(ImGui::GetIO().Framerate / 1000);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        frame_buffer->clear();

        // renderTargetSetting->open_z_buffer_write = false;
        // render(world_only_sky);
        renderTargetSetting->open_depth_buffer_write = true;
        Render(world);
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        statistic.Render();
        glDrawPixels(w, h, GL_RGB, GL_UNSIGNED_BYTE, frame_buffer->pixels);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
}

Renderer::Renderer(int _w, int _h) :
    w(_w), h(_h) {
    //设置opengl
    if (!glfwInit()) {
        printf("Couldn't init GLFW\n");
        return;
    }

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // Decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
    // GL ES 2.0 + GLSL 100
    const char *glsl_version = "#version 100";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(__APPLE__)
    // GL 3.2 + GLSL 150
    const char *glsl_version = "#version 150";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);           // Required on Mac
#else
    // GL 3.0 + GLSL 130
    const char *glsl_version = "#version 130";
    // glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    // glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+
    // only glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // 3.0+ only
#endif

    window = glfwCreateWindow(w, h, title, NULL, NULL);
    if (!window) {
        printf("Couldn't open window\n");
        return;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync
                         // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    // ImGui::StyleColorsClassic();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);
    // io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable
    // Keyboard Controls io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad; //
    // Enable Gamepad Controls
    frame_buffer = new FrameBuffer(w, h);
    transformer = new Transformer();
    renderTargetSetting = new RenderTargetSetting();
}

bool Renderer::Backface_culling(Vertex &vt) {
    return false;
}

void Renderer::ProcessGeometry(Triangle &t, Shader *&shader, Light *&light) {
    for (int i = 0; i < 3; i++) {
        auto &vt = t.vts[i];
        vt.ccv = shader->vertex_shading(vt, light);
    }

    //裁剪

    std::vector<Vertex> vts{t.vts[0], t.vts[1], t.vts[2]};

    auto clipped_x = Clipper::ClipPolygon(Plane::POSITIVE_X, vts);
    auto clipped_nx = Clipper::ClipPolygon(Plane::NEGATIVE_X, clipped_x);
    auto clipped_y = Clipper::ClipPolygon(Plane::POSITIVE_Y, clipped_nx);
    auto clipped_ny = Clipper::ClipPolygon(Plane::NEGATIVE_Y, clipped_y);
    auto clipped_z = Clipper::ClipPolygon(Plane::POSITIVE_Z, clipped_ny);
    auto clipped_nz = Clipper::ClipPolygon(Plane::NEGATIVE_Z, clipped_z);
    auto clipped_vt = Clipper::ClipPolygon(Plane::POSITIVE_W, clipped_nz);

    if (clipped_vt.size() < 3)
        return;

    for (int i = 0; i < clipped_vt.size(); i++) {
        auto &vt = clipped_vt[i];
        auto &ccv_pos = vt.ccv;
        //透视除法
        vt.inv = 1.0f / ccv_pos.w;
        ccv_pos *= vt.inv;

        if (cam->mode == PROJECTION_MODE::PERSPECTIVE) {
            vt.tex_coord *= vt.inv;
            vt.normal = vt.normal * vt.inv;
            vt.position_world = vt.position_world * vt.inv;
        }

        if (renderTargetSetting->open_frame_buffer_write) {
            glm::vec4 pos = glm::vec4(vt.position, 1);
            vt.l_pos = pos * shader->model * light->vp;
            vt.l_pos *= vt.inv;
        }

        vt.sv_pos = ccv_pos * transformer->view_port;
    }

    int step = clipped_vt.size() > 3 ? 1 : 3;
    int triangle_num = clipped_vt.size();
    for (int i = 0; i < triangle_num; i += step) {
        Triangle t(clipped_vt[i], clipped_vt[(i + 1) % clipped_vt.size()],
                   clipped_vt[(i + 2) % clipped_vt.size()]);
        //设置三角形,准备光栅化
        t.ready_rasterization();
        Rasterization(t, shader, light);
    }
}

} // namespace ylb