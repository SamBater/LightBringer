#ifndef __SERLIZABLE_H__
#define __SERLIZABLE_H__

#include "glm/fwd.hpp"
#include "json11.hpp"
#include <string>

namespace ylb {
class YLBSerializable {
public:
    virtual void DeSerilization(const json11::Json &json) = 0;

protected:
    inline glm::vec3 DeSerilizationVec3(const json11::Json &json) {
        glm::vec3 v{1,1,1};
        glm::vec3::length_type i = 0;
        for (auto &e : json.array_items())
            v[i++] = std::stof(e.dump());
        return v;
    }

    inline glm::vec2 DeSerilizationVec2(const json11::Json &json) {
        glm::vec2 v(0,0);
        glm::vec2::length_type i = 0;
        for (auto &e : json.array_items())
            v[i++] = std::stof(e.dump());
        return v;
    }
};
} // namespace ylb

#endif // __SERLIZABLE_H__