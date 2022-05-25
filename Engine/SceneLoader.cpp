#include "SceneLoader.h"
#include "ModelLoader.h"
using namespace ylb;
std::unique_ptr<Scene> SceneLoader::LoadScene(const char* scene_path)
{
	using namespace std;
	using namespace json11;
	auto abs_path = ylb::YLBFileSystem::GetInstance().GetScenePath(scene_path);
	ifstream sp_ftream(abs_path);
	string scene_string((istreambuf_iterator<char>(sp_ftream)), istreambuf_iterator<char>());
	string err;
	auto json = Json::parse(scene_string, err);

	if (!err.empty()) {
		cerr << "error ocurse when load "
			<< scene_path << '\n';
		exit(-1);
	}

	auto scene = make_unique<Scene>();
	scene->cam->DeSerilization(json["Camera"]);

	for (auto& model_json : json["Models"].array_items()) {
		Model* model = LoadModel(model_json["path"].string_value().c_str());

		Shader *shader = nullptr;
		auto shader_string = model_json["Shader"]["type"].string_value();
		auto diffuse_map = model_json["Shader"]["diffuse"].string_value();
		auto specular_map = model_json["Shader"]["specular"].string_value();
		auto normal_map = model_json["Shader"]["normal"].string_value();

		Texture* diffuse = nullptr;
		Texture* specular = nullptr;
		Texture* normal = nullptr;

		//Load Textures
		if(diffuse_map.size())
			diffuse = new Texture(diffuse_map.c_str());
		if(specular_map.size())
			specular = new Texture(specular_map.c_str());
		if (normal_map.size())
			normal = new Texture(normal_map.c_str());

		if (shader_string == "Groud")
			shader = new GouraudShader(diffuse,normal);
		else
			shader = new PhongShader(diffuse,specular,normal);
		model->SetShader(shader);
		model->DeSerilization(model_json);
		scene->models->push_back(model);
	}

	const std::string paralle = "Paralle";
	const std::string point = "Point";
	for (auto& light_json : json["Lights"].array_items()) {
		Light* lite = nullptr;
		auto lite_type = light_json["type"].string_value();
		if (lite_type == paralle)
			lite = new ParalleLight();
		else if (lite_type == point)
			lite = new PointLight();
		lite->DeSerilization(light_json);
		scene->lights->push_back(lite);
	}

	return scene;
}
