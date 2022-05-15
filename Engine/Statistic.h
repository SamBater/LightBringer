#ifndef __STATISTIC_H__
#define __STATISTIC_H__

#include "imgui.h"
#include <basetsd.h>


namespace ylb{

class Statistic{
public:
    void Render(){
        ImGui::Begin("Statisc");
        ImGui::Text("FPS:%.1f\n"
        "CPU:%.1fms\n"
        "Triangles:%d\n", ImGui::GetIO().Framerate,1000.0f / ImGui::GetIO().Framerate,triangle_cnt);
        ImGui::End();
    }
    
    void InitTriangleCnt(){
        triangle_cnt = 0;
    }
    
    void IncreaseTriangleCnt(UINT64 cnt = 1){
        triangle_cnt += cnt;
    }
private:
    UINT64 triangle_cnt;
};

}

#endif // __STATISTIC_H__