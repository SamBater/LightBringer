#include "Core/Pipeline/Render.h"
int main(int argc, const char **argv)
{
    YYLB::Render render(1280, 1024);
    render.start();
    return 0;
}