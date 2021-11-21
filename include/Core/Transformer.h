#ifndef YYLB_TRANSFORMER_H
#define YYLB_TRANSFORMER_H
#include <math/Matrix.h>
#include <Core/Camera.h>
#include <math/Vertex.h>
namespace YYLB
{
    //负责坐标转换,并输出相应
    struct Transformer
    {
        Matrix4f world; //模型变化矩阵
        Matrix4f view; //视图变换矩阵
        Matrix4f projection; //透视投影变换矩阵
        Matrix4f view_port; //视口变换矩阵

        //物体移动时重新计算
        void set_matrix_world(Vec3f &world_pos);

        //相机位置改变时重新计算
        void set_world_to_view(Camera *cam);

        //相机变化时重新计算
        void set_view_to_project(Camera *cam);

        //屏幕尺寸改变时重新计算
        void set_projection_to_screen(int &w, int &h);

        //转换输出
        //return : 是否需要裁剪
        bool vertex_output(Vertex &vt, Vec3f &world_pos, Vec4f &out_ss_pos);
    };

}
#endif