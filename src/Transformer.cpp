#include <Core/Transformer.h>
#include <math/Common.h>
namespace YYLB
{
    void Transformer::set_model_to_world(Vec3f &world_pos)
    {
        set_identyti(m_m2w);
        translate(m_m2w, world_pos);
    }

    void Transformer::set_world_to_view(Camera *cam)
    {
        set_identyti(m_w2v);
        auto &cam_pos = cam->getPos();
        for (int i = 0; i < 3; i++)
            m_w2v[i][3] = -cam_pos[i][0];

        auto &g = cam->look_at = {0, 0, -1};
        auto &t = cam->up = {0, 1, 0};
        auto gxt = cross_product_3d(g, t);
        auto ng = g * 1.0f;
        Matrix4f m_rv;
        set_zero(m_rv);
        m_rv[0][0] = gxt.x();
        m_rv[0][1] = gxt.y();
        m_rv[0][2] = gxt.z();
        m_rv[1][0] = t.x();
        m_rv[1][1] = t.y();
        m_rv[1][2] = t.z();
        m_rv[2][0] = ng.x();
        m_rv[2][1] = ng.y();
        m_rv[2][2] = ng.z();

        m_w2v *= m_rv;
    }

    void Transformer::set_view_to_project(Camera *cam)
    {
        cam->set_perspective_matrix(m_v2p);
    }

    void Transformer::set_projection_to_screen(int &w, int &h)
    {
        set_identyti(m_p2s);
        m_p2s[0][0] = m_p2s[0][3] = w / 2;
        m_p2s[1][1] = m_p2s[1][3] = h / 2;
    }

    Vec4f Transformer::vertex_output(Vertex &vt, Vec3f &world_pos)
    {
        auto local_pos = vt.position;
        set_model_to_world(world_pos);
        auto mvp = m_v2p * m_w2v * m_m2w;
        Vec4f local_pos_h{local_pos.x(), local_pos.y(), local_pos.z(), 1};
        auto pos_project = mvp * local_pos_h;
        pos_project /= pos_project.w();
        pos_project = m_p2s * pos_project;
        //std::cout << pos_project;
        return pos_project;
    }
}