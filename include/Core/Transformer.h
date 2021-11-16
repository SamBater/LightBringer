#ifndef YYLB_TRANSFORMER_H
#define YYLB_TRANSFORMER_H
#include <math/Matrix.h>
#include <Core/Camera.h>
namespace YYLB
{
    //负责坐标转换,并输出相应
    struct Transformer
    {
        Matrix4f m_m2w;
        Matrix4f m_w2v;
        Matrix4f m_v2p;
        Matrix4f m_p2s;

        //物体移动时重新计算
        void set_model_to_world(Vec3f &world_pos);

        //相机位置改变时重新计算
        void set_world_to_view(const Vec3f &cam_pos);

        //相机变化时重新计算
        void set_view_to_project(Camera *cam);

        //屏幕尺寸改变时重新计算
        void set_projection_to_screen(int &w, int &h);

        //转换输出
        Vec4f vertex_output(Vec3f &local_pos, Vec3f &world_pos);
    };

}
#endif