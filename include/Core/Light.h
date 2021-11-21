#pragma once
#include "Core/Actor.h"
#include "math/Vertex.h"
#include "math/Matrix.h"
namespace YYLB{
    class SimpleShader;
    class Light : public Actor
    {
    protected:
            float light_intense = 1.0f;
            Vec3f light_color;
            friend class Shader;
            friend class SimpleShader;
    public:

    };
}