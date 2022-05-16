#ifndef YYLB_TRANSFORMER_H
#define YYLB_TRANSFORMER_H
#include "Camera.h"
#include "Vertex.h"
#include "glm/glm.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "Common.h"
namespace ylb
{
    //负责坐标转换,并输出相应
    struct Transformer
    {
        glm::mat4 view;         //视图变换矩阵
        glm::mat4 projection;   //透视投影变换矩阵
        glm::mat4 view_port;    //视口变换矩阵

        //物体移动时重新计算
        glm::mat4 calc_matrix_world(glm::vec3 &world_pos);

        //相机位置改变时重新计算
        void set_world_to_view(Camera *cam);

        //相机变化时重新计算
        void set_view_to_project(Camera *cam,PROJECTION_MODE mode);

        //屏幕尺寸改变时重新计算
        void set_projection_to_screen(int &w, int &h);
    };



}
#endif