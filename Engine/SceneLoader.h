#ifndef __SCENELOADER_H__
#define __SCENELOADER_H__

#include <string.h>
#include <fstream>
#include <streambuf>
#include "json11.hpp"
#include "Camera.h"
#include <memory>
#include <vector>
#include "Triangle.h"
#include "YLBFileSystem.h"
#include "PhongShader.h"
#include "ParallelLight.h"
#include "Model.h"
#include "PointLight.h"
#include "GroudShader.h"
namespace ylb {

	struct Scene {
		Camera* cam = new Camera();
		std::vector<Model*>* models = new std::vector<Model*>();
		std::vector<Light*>* lights = new std::vector<Light*>();
	};

	class SceneLoader {
	public:

		static SceneLoader& Instance() {
			static SceneLoader scene_loader;
			return scene_loader;
		}

		std::unique_ptr<Scene> LoadScene(const char* scene_path);

	private:
		SceneLoader() = default;
	};
}
#endif // __SCENELOADER_H__