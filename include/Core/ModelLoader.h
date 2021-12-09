#pragma once
#include "Math/Triangle.h"
#include "Math/Vertex.h"
#include <cwchar>
#include <vector>
#include <cstring>
namespace ylb
{
    std::vector<ylb::Triangle> LoadObj(const char* modelPath);
}