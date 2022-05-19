//
// Created by a on 2021/12/10.
//

#include <iostream>
#include "Clipper.h"

#define W_CLIPPING_PLANE 1e-5

std::vector<ylb::Vertex> ylb::Clipper::ClipPolygon(Plane plane,std::vector<Vertex>& in_coords) {
    std::vector<Vertex> out_coords;

    if(in_coords.size() < 3) return out_coords;

    for (int i = 0; i < in_coords.size(); i++) {
        int prev_index = (i - 1 + in_coords.size()) % in_coords.size();
        int curr_index = i;
        Vertex prev_coord = in_coords[prev_index];
        Vertex curr_coord = in_coords[curr_index];
        int prev_inside = is_inside_plane(prev_coord.ccv, plane);
        int curr_inside = is_inside_plane(curr_coord.ccv, plane);

        if (prev_inside != curr_inside) {
            float ratio = get_intersect_ratio(prev_coord.ccv, curr_coord.ccv, plane);
            assert(ratio <= 1 && ratio >= 0);
            Vertex dest_coord = Intersect(prev_coord,curr_coord,ratio);
            out_coords.push_back(dest_coord);
        }

        if (curr_inside) {
            out_coords.push_back(curr_coord);
        }
    }
    return out_coords;
}

ylb::Vertex ylb::Clipper::Intersect(ylb::Vertex &previousVertex, ylb::Vertex &currentVertex,float intersectionFactor) {
    glm::vec3 position = ylb::lerp(previousVertex.position,currentVertex.position,intersectionFactor);
    glm::vec3 normal = ylb::lerp(previousVertex.normal,currentVertex.normal,intersectionFactor);
    //glm::vec3 tex_coord = ylb::lerp(previousVertex.tex_coord,currentVertex.tex_coord,intersectionFactor);
    auto tex_coord = glm::vec2(1, 0);
    Vertex vt = Vertex(position,normal,tex_coord);

    vt.ccv = ylb::lerp(previousVertex.ccv,currentVertex.ccv,intersectionFactor);
    //vt.l_pos = ylb::lerp(previousVertex.l_pos,currentVertex.l_pos,intersectionFactor);
    return vt;
}

int ylb::Clipper::is_inside_plane(glm::vec4 coord, Plane plane) {
    switch (plane) {
        case POSITIVE_W:
            return coord.w >= 1e-5;
        case POSITIVE_X:
            return coord.x <= +coord.w;
        case NEGATIVE_X:
            return coord.x >= -coord.w;
        case POSITIVE_Y:
            return coord.y <= +coord.w;
        case NEGATIVE_Y:
            return coord.y >= -coord.w;
        case POSITIVE_Z:
            return coord.z <= +coord.w;
        case NEGATIVE_Z:
            return coord.z >= -coord.w;
        default:
            assert(0);
            return 0;
    }
}

float ylb::Clipper::get_intersect_ratio(glm::vec4 prev, glm::vec4 curr, ylb::Plane plane) {
    switch (plane) {
        case POSITIVE_W:
            return (prev.w - 1e-5) / (prev.w - curr.w);
        case POSITIVE_X:
            return (prev.w - prev.x) / ((prev.w - prev.x) - (curr.w - curr.x));
        case NEGATIVE_X:
            return (prev.w + prev.x) / ((prev.w + prev.x) - (curr.w + curr.x));
        case POSITIVE_Y:
            return (prev.w - prev.y) / ((prev.w - prev.y) - (curr.w - curr.y));
        case NEGATIVE_Y:
            return (prev.w + prev.y) / ((prev.w + prev.y) - (curr.w + curr.y));
        case POSITIVE_Z:
            return (prev.w - prev.z) / ((prev.w - prev.z) - (curr.w - curr.z));
        case NEGATIVE_Z:
            return (prev.w + prev.z) / ((prev.w + prev.z) - (curr.w + curr.z));
        default:
            assert(0);
            return 0;
    }
}

bool ylb::Clipper::need_to_clip(ylb::Vertex *vt,int in_num) {
    for(int i = 0 ; i < in_num ; i++)
    {
        auto& ccv = vt[i].ccv;
        if(ccv.x >= ccv.w || ccv.x <= -ccv.w || ccv.y >= ccv.w || ccv.y <= -ccv.w || ccv.z >= ccv.w || ccv.z <= -ccv.w)
            return false;
    }
    return true;
}

