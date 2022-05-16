#include "Vertex.h"

void ylb::Vertex::DeSerilization(const json11::Json &json) {
    position = DeSerilizationVec3(json["position"]);
    normal = DeSerilizationVec3(json["normal"]);
    tex_coord = DeSerilizationVec3(json["uv"]);
}
