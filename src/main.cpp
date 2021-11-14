#include <Core/Render.h>
int main(int argc, const char **argv)
{
    YYLB::Render render(1024, 768);
    render.start();
    return 0;
}
