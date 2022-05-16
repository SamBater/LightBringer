#pragma once
#include "Triangle.h"
#include "Vertex.h"
#include <cwchar>
#include <vector>
#include <cstring>
namespace ylb
{
    std::vector<ylb::Triangle>* LoadObj(const char* modelPath);
}