#include "Core/Pipeline/Transformer.h"
namespace YYLB
{
    void Transformer::set_matrix_world(Vec3f &world_pos)
    {
        set_identyti(world);
        translate(world, world_pos);
    }

    void Transformer::set_world_to_view(Camera *cam)
    {
        set_identyti(view);

        auto g = cam->look_at;
        g.normalized();
        auto& t = cam->up;

        auto w = g * -1.0f;w.normalized();
        auto u = cross_product_3d(t,w);u.normalized();
        auto v = cross_product_3d(w,u);

        view[0][0] = u.x();view[0][1] = u.y();view[0][2] = u.z();
        view[1][0] = v.x();view[1][1] = v.y();view[1][2] = v.z();
        view[2][0] = w.x();view[2][1] = w.y();view[2][2] = w.z();
        view[0][3] = cam->position_world.x() * -1;
        view[1][3] = cam->position_world.y() * -1;
        view[2][3] = cam->position_world.z() * -1;
    }

    void Transformer::set_view_to_project(Camera *cam,PROJECTION_MODE mode)
    {
        cam->mode = mode;
        if(mode == PROJECTION_MODE::PERSPECTIVE)
            cam->set_perspective_matrix(projection);
        if(mode == PROJECTION_MODE::ORTHOGONAL)
            cam->set_orthogonal_matrix(projection);
    }

    void Transformer::set_projection_to_screen(int &w, int &h)
    {
        set_identyti(view_port);
        view_port[0][0] = w * 1.f / 2;view_port[0][3] = (w-1) / 2;
        view_port[1][1] = h * 1.f / 2;view_port[1][3] = (h-1) / 2;
    }

    bool Transformer::vertex_output(Vertex &vt, Vec3f &world_pos, Vec4f &out_ss_pos,PROJECTION_MODE project_mode)
    {
        auto local_pos = vt.position;
        set_matrix_world(world_pos);
        Matrix4f mvp = projection * view * world;
        Vec4f local_pos_h{local_pos.x(), local_pos.y(), local_pos.z(), 1};
        Vec4f ccv_pos = mvp * local_pos_h;
        //裁剪
        if (ccv_pos.x() >= ccv_pos.w() || ccv_pos.x() <= -ccv_pos.w())
            return false;
        if (ccv_pos.y() >= ccv_pos.w() || ccv_pos.y() <= -ccv_pos.w())
            return false;
        if(ccv_pos.z() >= ccv_pos.w() || ccv_pos.z() <= -ccv_pos.w())
            return false;

        //透视除法
        vt.inv = 1.0f / ccv_pos.w();
        ccv_pos *= vt.inv;

        if(project_mode == PROJECTION_MODE::PERSPECTIVE)
        {
            vt.set_uv(vt.u() * vt.inv, vt.v() * vt.inv);
            vt.normal = vt.normal * vt.inv;
            vt.position_world = vt.position_world * vt.inv;
        }

        out_ss_pos = view_port * ccv_pos;
        return true;
    }

    Matrix4f Transformer::calc_mvp(Camera *cam, const Vec3f &model_world_pos) {
        YYLB::Matrix4f model;
        YYLB::set_identyti(model);
        translate(model,model_world_pos * -1.0);
        set_world_to_view(cam);
        set_view_to_project(cam,cam->mode);
        return projection * view * model;
    }


}