#include "Core/Pipeline/Render.h"
#include "glm/ext/matrix_transform.hpp"
int main(int argc, const char **argv)
{
    YYLB::Render render(1280, 1024);
    render.start();

    glm::mat4 mat(1);
    glm::vec3 vec(1.5,4.2,-40);
    mat = glm::translate(mat,vec);
    auto inv = glm::inverse(mat);

    return 0;
}