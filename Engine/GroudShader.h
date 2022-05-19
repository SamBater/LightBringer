#ifndef __GROUDSHADER_H__
#define __GROUDSHADER_H__

#include "Shader.h"
#include "glm/fwd.hpp"

namespace ylb {
	class GroudShader : public Shader {
	public:
		GroudShader() = default;
		GroudShader(Texture* diffuse_map,Texture* normal_map = nullptr) : diffuse_map(diffuse_map) , normal_map(normal_map) {}
		virtual glm::vec4 VertexShading(Vertex& v, const VertexShaderContext& context);
		virtual glm::vec3 FragmentShading(Triangle& t, const FragmentShaderContext& context);
	private:
		Texture* diffuse_map;
		Texture* normal_map;
		glm::vec3 varying_color;
		glm::mat3 TBN;
	};
}

#endif // __GROUDSHADER_H__