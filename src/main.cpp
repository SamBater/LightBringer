#include <Core/Render.h>
int main(int argc, const char **argv)
{
    YYLB::Render render(1440, 1024);
    render.start();
    // auto tf = new YYLB::Transformer();
    // YYLB::Vec3f pos{15, 10, 22};
    // YYLB::Vec3f wp{0, 1, -2};
    // tf->set_model_to_world(wp);
    // int w = 1000, h = 500;
    // YYLB::Camera *cam = new YYLB::Camera(0, 1, 0, YYLB::PI / 2.f, w * 1.f / h, 2.f, 400.f);
    // tf->set_world_to_view(cam->getPos());
    // tf->set_view_to_project(cam);
    // tf->set_projection_to_screen(w, h);

    //std::cout << tf->vertex_output(pos, wp);

    return 0;
}
