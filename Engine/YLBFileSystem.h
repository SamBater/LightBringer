#pragma once

namespace ylb {
#include "Predefined.h"
#include <string>
class YLBFileSystem {
public:
    static YLBFileSystem &GetInstance() {
        static YLBFileSystem instance;
        return instance;
    }

    std::string GetAssetsPath(const char *path) {
        return assets_dir + path;
    }

    std::string GetScenePath(const char *path) {
        return assets_dir + path;
    }

private:
    YLBFileSystem() {
        assets_dir = ASSETS_DIR;
        scene_dir = assets_dir + "Scene/";
    }
    std::string assets_dir;
    std::string scene_dir;
};

}