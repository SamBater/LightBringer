#include "Mesh.h"

void ylb::Mesh::DeSerilization(const json11::Json &json) {
    triangles = LoadObj(json["path"].string_value().c_str());
    position_world = DeSerilizationVec3(json["Transform"]["position"]);
}
