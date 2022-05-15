#ifndef __SCENELOADER_H__
#define __SCENELOADER_H__

#include <string.h>
#include <fstream>
#include <streambuf>
#include <json11.hpp>
#include "Camera.h"
#include <memory>
#include "YLBFileSystem.h"
namespace ylb {

struct Scene {
    Camera* cam = new Camera();
};

inline std::unique_ptr<Scene> LoadScene(const char* scene_path) {
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
    return scene;
}

}
#endif // __SCENELOADER_H__