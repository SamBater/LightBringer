#include "SceneLoader.h"
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


	//TODO:Load Shader From JSON
	Texture* texture = new Texture("uv.jpg");
	Shader* phong = new PhongShader(texture);
	for (auto& model_json : json["Models"].array_items()) {
		Mesh* mesh = new Mesh();
		mesh->SetShader(phong);
		mesh->DeSerilization(model_json);
		scene->meshs->push_back(mesh);
	}

	for (auto& light_json : json["Lights"].array_items()) {
		Light* lite = nullptr;
		auto lite_type = light_json["type"].string_value();
		if (lite_type._Equal("Paralle"))
			lite = new ParalleLight();
		else if (lite_type._Equal("Point"))
			lite = new PointLight();
		lite->DeSerilization(light_json);
		scene->lights->push_back(lite);
	}

	return scene;
}
