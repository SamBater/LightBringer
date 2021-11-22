#pragma once
#include "Math/Matrix.h"
#include "Math/Triangle.h"
#include "Math/Vertex.h"
#include <cwchar>
#include <vector>
#include <cstring>
namespace YYLB
{
    std::vector<YYLB::Triangle> LoadObj(const char* modelPath);
}