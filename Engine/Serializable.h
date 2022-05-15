#ifndef __SERIALIZABLE_H__
#define __SERIALIZABLE_H__

namespace ylb {
#include <string>
#include "json11.hpp"
#include "glm/glm.hpp"

inline glm::vec3 LoadVec3(const json11::Json &jsonObj) {
    glm::vec3 result;
    size_t i = 0;
    for (auto &e : jsonObj.array_items())
        result[i++] = std::stof(e.dump()));
    return result;
}
} // namespace ylb

#endif // __SERIALIZABLE_H__