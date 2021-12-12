//
// Created by a on 2021/12/10.
//

#ifndef LIGHTBRINGER_CLIPPER_H
#define LIGHTBRINGER_CLIPPER_H
#include "math/Triangle.h"
#include <vector>
namespace ylb{

    enum Plane{
        POSITIVE_W,
        POSITIVE_X,
        NEGATIVE_X,
        POSITIVE_Y,
        NEGATIVE_Y,
        POSITIVE_Z,
        NEGATIVE_Z
    };

    class Clipper {
    private:
        static Vertex Intersect(ylb::Vertex &previousVertex, ylb::Vertex &currentVertex,float intersectionFactor);
        static int is_inside_plane(glm::vec4 coord, Plane plane);
        static float get_intersect_ratio(glm::vec4 prev, glm::vec4 curr, Plane plane);

    public:
        static std::vector<ylb::Vertex> ClipPolygon(Plane plane,std::vector<Vertex>& in_coords);
        static bool need_to_clip(ylb::Vertex vt[],int in_num);
    };
}


#endif //LIGHTBRINGER_CLIPPER_H
