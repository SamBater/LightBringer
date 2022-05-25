#include "Renderer.h"
#include "GLFW/glfw3.h"
#include "imgui.h"
#include "ModelLoader.h"
namespace ylb {
	void Renderer::ProcessInput(double&& delta_time) {
		int dx = glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS ? -1 : glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS ? 1 :
			0;
		int dz = glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS ? 1 : glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS ? -1 :
			0;
		int dy = glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS ? 1 : 0;
		dy = glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS ? -1 : dy;

		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			std::exit(0);

		if (dz == 1)
			cam->ProcessKeyboard(Camera_Movement::FORWARD, delta_time);
		if (dz == -1)
			cam->ProcessKeyboard(Camera_Movement::BACKWARD, delta_time);
		if (dx == 1)
			cam->ProcessKeyboard(Camera_Movement::RIGHT, delta_time);
		if (dx == -1)
			cam->ProcessKeyboard(Camera_Movement::LEFT, delta_time);
		if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
			cam->ProcessKeyboard(Camera_Movement::Up, delta_time);
		if(glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
			cam->ProcessKeyboard(Camera_Movement::Down, delta_time);
	}

	void Renderer::Framebuffer_Size_Callback(GLFWwindow* window, int width, int height) {
		auto& instance = Instance();
		instance.SetViewPort(width, height);
	}

	
	void Renderer::Mouse_Move_Callback(GLFWwindow* window, double xposIn,
		double yposIn) {
		static bool firstMouse = true;
        static float lastX, lastY;
		auto instance = Instance();
		float xpos = static_cast<float>(xposIn);
		float ypos = static_cast<float>(yposIn);
		if (firstMouse) {
			lastX = xpos;
			lastY = ypos;
			firstMouse = false;
		}

		float xoffset = xpos - lastX;
		float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

		lastX = xpos;
		lastY = ypos;

		instance.cam->ProcessMouseMovement(xoffset, yoffset);
	}

	void Renderer::Render(std::vector<ylb::Model*>& models) {
		statistic.InitTriangleCnt();

		auto const view = std::make_shared<glm::mat4>(cam->GetViewMatrix());
		auto const project = std::make_shared<glm::mat4>(cam->GetProjectionMatrix());
		glm::vec3 world_pos[3];
		for (int i = 0; i < models.size(); i++) {
			auto& model = models[i];
			for (int i = 0; i < model->faces->size(); i++) {

				auto t = model->Triangle(i);

				VertexShaderContext vertexShaderContext;
				vertexShaderContext.model = &model->transform.ModelMatrix();
				 for (int i = 0; i < 3; i++)
				 	world_pos[i] = *vertexShaderContext.model * glm::vec4(t.vts[i].model_coords, 0);
				 if (pipelineSetting->back_face_culling && BackFaceCulling(world_pos))
				 	continue;

				vertexShaderContext.view = view.get();
				vertexShaderContext.project = project.get();
				vertexShaderContext.camPos = &cam->transform.WorldPosition();
				vertexShaderContext.l = lights[0];
				ProcessGeometry(t, model->shader, vertexShaderContext);
			}
		}
    }

    void Renderer::GenerateShadowMap() {
        pipelineSetting->open_frame_buffer_write = false;
        auto mode = cam->mode;
        auto origin = cam->transform.WorldPosition();
        auto dir = cam->Front;
        cam->mode = PROJECTION_MODE::ORTHOGONAL;
        cam->transform.SetPosition(lights[0]->transform.WorldPosition());
        cam->Front = -glm::normalize(lights[0]->LightDir(dir));

		//对于平行光,使用正交投影渲染场景
        auto vp = cam->GetProjectionMatrix() * cam->GetViewMatrix();
        frame_buffer->Clear();
		Render(models);
		//将shadowMap保存
		lights[0]->SetShadowMap(vp, frame_buffer->depth_buffer, w, h);

		//DEBUG DEPTH BUFFER
        frame_buffer->SaveDepthMap("depth.bmp",cam->mode == PROJECTION_MODE::PERSPECTIVE);
    
		
		//恢复原来的状态
		cam->transform.SetPosition(origin);
       	cam->Front = dir;
       	cam->mode = mode;
        pipelineSetting->open_frame_buffer_write = true;
    }

    Model* Renderer::GenerateSkybox() {
        Model *cube = LoadModel("Model/cube.obj");
        std::vector<std::string> faces_path{
            "Texture/skybox/right.jpg",
            "Texture/skybox/left.jpg",
            "Texture/skybox/top.jpg",
            "Texture/skybox/bottom.jpg",
            "Texture/skybox/front.jpg",
            "Texture/skybox/back.jpg",
        };
        CubeMap *cube_map = new CubeMap(faces_path);
        cube->shader = new SkyBoxShader(cube_map);
        return cube;
    }

	void Renderer::Rasterization(ylb::Triangle& t, Shader* shader, Light* light) {
		statistic.IncreaseTriangleCnt();
		const ylb::BoundingBox* bb = t.bounding_box();
        glm::vec3 color;
		FragmentShaderContext fragmentShaderContext(&cam->transform.WorldPosition(), lights[0]);
		for (int y = bb->bot; y < bb->top; y++)
			for (int x = bb->left; x < bb->right ; x++) {
				int pixel = y * w + x;
				//三角形测试
				if (ylb::Triangle::Inside(x + 0.5f, y + 0.5f, t,cam->mode == PROJECTION_MODE::PERSPECTIVE)) {
                    float depth_buffer = t.interpolated_depth();
					//深度测试
					if (frame_buffer->DepthTest(x, y, depth_buffer)) {
						//深度写入
						if (pipelineSetting->open_depth_buffer_write) {
							frame_buffer->SetDepth(x, y, depth_buffer);
						}
						//帧缓存写入
						if (pipelineSetting->open_frame_buffer_write) {
							color = shader->FragmentShading(t, fragmentShaderContext);
							frame_buffer->set_color(x, y, color);
						}
					}
				}
			}
	}

	void Renderer::InitOpenGL() {
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
		const char* glsl_version = "#version 100";
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
		glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(__APPLE__)
	// GL 3.2 + GLSL 150
		const char* glsl_version = "#version 150";
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // 3.2+ only
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);           // Required on Mac
#else
	// GL 3.0 + GLSL 130
		const char* glsl_version = "#version 130";
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
		glfwSetFramebufferSizeCallback(window, &Renderer::Framebuffer_Size_Callback);
		//glfwSetCursorPosCallback(window, &Renderer::Mouse_Move_Callback);
		//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		glfwSwapInterval(1); // Enable vsync
							 // Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		(void)io;
		// Setup Dear ImGui style
		//ImGui::StyleColorsDark();
		ImGui::StyleColorsClassic();

		// Setup Platform/Renderer backends
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init(glsl_version);
		// io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable
		// Keyboard Controls io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad; //
		// Enable Gamepad Controls
	}

	void Renderer::LoadScene(const char* scene_file_path)
	{
		auto scene = SceneLoader::Instance().LoadScene(scene_file_path);
		cam = std::move(scene->cam);
		SetViewPort(w, h);
		for (auto& obj : *scene->models) {
			models.push_back(std::move(obj));
		}

		for (auto& lite : *scene->lights) {
			lights.push_back(std::move(lite));
		}
	}

	void Renderer::SetViewPort(int width, int height) {
		delete frame_buffer;
		w = width;
		h = height;
		frame_buffer = new FrameBuffer(width, height);
        cam->aspect_ratio = w * 1.0f / h;
        cam->UpdateCameraInfo();
		view_port = glm::mat4(1);
		view_port[0][0] = static_cast<double>(w) / 2.0; view_port[3][0] = static_cast<double>(w - 1) / 2.0;
		view_port[1][1] = static_cast<double>(h) / 2.0; view_port[3][1] = static_cast<double>(h - 1) / 2.0;
	}

	bool Renderer::BackFaceCulling(const glm::vec3 world_pos[]) {
		auto e0 = world_pos[1] - world_pos[0];
		auto e1 = world_pos[2] - world_pos[0];
		auto n = glm::cross(e0, e1);
		auto angle = glm::dot(n, cam->Front);
		return angle > 0.01f;
	}

	void Renderer::Start() {
		using ylb::Triangle;
		using ylb::Vertex;

		InitOpenGL();
		LoadScene("Scene/sample.json");
        Model *skybox = GenerateSkybox();
        std::vector<Model *> skybox_world = {skybox};
        GenerateShadowMap();
		while (!glfwWindowShouldClose(window)) {
			ProcessInput(ImGui::GetIO().Framerate / 1000);
			glClear(GL_COLOR_BUFFER_BIT);

			frame_buffer->Clear();

			pipelineSetting->open_depth_buffer_write = true;
			Render(models);
            //pipelineSetting->open_depth_buffer_write = false;
            //Render(skybox_world);

			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();
			
			statistic.Render();
			//Light Setting UI
			{
				ImGui::Begin("LightSetting");
				ImGui::SliderFloat("shadow_bias", &lights[0]->shadow_bias, 0.0f, 0.2f, "%.5f");
				auto sun = static_cast<ParalleLight *>(lights[0]);
				float dir[3];
				for (int i = 0; i < 3; i++)
					dir[i] = sun->dir[i];
				ImGui::SliderFloat3("light_dir", dir, -1.0f, 1.0f);
				for (int i = 0; i < 3; i++)
					sun->dir[i] = dir[i];

				float offset[3];
				for (int i = 0; i < 3; i++)
					offset[i] = sun->offset[i];
				ImGui::SliderFloat3("shadow_offset", offset, -1.0f, 1.0f);
				for (int i = 0; i < 3; i++)
					sun->offset[i] = offset[i];

				glm::normalize(sun->dir);
				if (ImGui::Button("CreateShdowMap"))
				{
					GenerateShadowMap();
				}
				if (ImGui::Button("Switch Projection Mode"))
					cam->mode = cam->mode == PROJECTION_MODE::ORTHOGONAL ?
									PROJECTION_MODE::PERSPECTIVE :
									PROJECTION_MODE::ORTHOGONAL;
				ImGui::End();
			}
			

			glDrawPixels(w, h, GL_RGB, GL_UNSIGNED_BYTE, frame_buffer->color_buffer);

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
	}

	
	void Renderer::ProcessGeometry(Triangle& t, Shader*& shader, VertexShaderContext& vertexShaderContext) {
		for (int i = 0; i < 3; i++) {
			auto& vt = t.vts[i];
			vertexShaderContext.nthVertex = i;
			vt.ccv = shader->VertexShading(vt, vertexShaderContext);
		}

		//裁剪

		std::vector<Vertex> vts{ t.vts[0], t.vts[1], t.vts[2] };

		 auto clipped_x = Clipper::ClipPolygon(Plane::POSITIVE_X, vts);
		 auto clipped_nx = Clipper::ClipPolygon(Plane::NEGATIVE_X, clipped_x);
		 auto clipped_y = Clipper::ClipPolygon(Plane::POSITIVE_Y, clipped_nx);
		 auto clipped_ny = Clipper::ClipPolygon(Plane::NEGATIVE_Y, clipped_y);
		 auto clipped_z = Clipper::ClipPolygon(Plane::POSITIVE_Z, clipped_ny);
		 auto clipped_nz = Clipper::ClipPolygon(Plane::NEGATIVE_Z, clipped_z);
		 auto clipped_vt = Clipper::ClipPolygon(Plane::POSITIVE_W, clipped_nz);

		 if (clipped_vt.size() < 3)
		    return;

		//auto clipped_vt = vts;

		for (int i = 0; i < clipped_vt.size(); i++) {
			auto& vt = clipped_vt[i];
			auto inv_w = 1.0f / vt.ccv.w;
			if (cam->mode == PROJECTION_MODE::PERSPECTIVE) {
                //透视除法
                vt.ccv *= inv_w;
            }

			//裁剪
            //if (vt.ccv.x >= vt.ccv.w)
            //    return;
            //if (vt.ccv.x <= -vt.ccv.w)
            //    return;
            //if (vt.ccv.y >= vt.ccv.w)
            //    return;
            //if (vt.ccv.y <= -vt.ccv.w)
            //    return;
            //if (vt.ccv.z <= -vt.ccv.w)
            //    return;
            //if (vt.ccv.z >= vt.ccv.w)
            //    return;


			if (cam->mode == PROJECTION_MODE::PERSPECTIVE) {
                vt.texture_coords = vt.texture_coords * inv_w;
                vt.normal = vt.normal * inv_w;
                vt.world_coords = (*vertexShaderContext.model) * glm::vec4(vt.model_coords,0) * inv_w;
			}

			vt.screen_coords = view_port * vt.ccv;
			if(cam->mode == PROJECTION_MODE::PERSPECTIVE)
			{
				vt.sz() *= inv_w;
                vt.screen_coords[3] = inv_w;
			}
		}

		int step = clipped_vt.size() > 3 ? 1 : 3;
		int triangle_num = clipped_vt.size();
		for (int i = 0; i < triangle_num; i += step) {
			Triangle t(clipped_vt[i], clipped_vt[(i + 1) % clipped_vt.size()],
				clipped_vt[(i + 2) % clipped_vt.size()]);
			//设置三角形,准备光栅化
			t.ComputeBoundingBox();
			Rasterization(t, shader, nullptr);
		}
	}

} // namespace ylb