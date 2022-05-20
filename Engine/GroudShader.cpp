#include "GroudShader.h"
#include "glm/fwd.hpp"
#include "glm/matrix.hpp"

glm::vec4 ylb::GroudShader::VertexShading(Vertex& vt,
	const VertexShaderContext& context) {
	glm::vec4 pos_h(vt.position, 1);

	// MVP
	auto MVP = (*context.model) * (*context.view) * (*context.project);
	glm::vec4 ccv_pos = pos_h * MVP;

	const auto& model = (glm::mat3(*context.model));
	glm::vec3 T = glm::normalize(glm::vec3(model * glm::vec4(vt.tangent, 0.0)));
	glm::vec3 N = glm::normalize(glm::vec3(model * glm::vec4(vt.normal, 0.0)));
	auto B = glm::normalize(glm::cross(T, N));
	TBN = glm::mat3(T, B, N);
	TBN = glm::transpose(TBN);
	//varying_color[context.nthVertex] = std::max(0.0f, glm::dot(l, normal));
	return ccv_pos;
}

glm::vec3
ylb::GroudShader::FragmentShading(Triangle& t,
	const FragmentShaderContext& context) {

	float u, v;
	t.interpolated_uv(u, v);

	auto normal = normal_map ?
		(normal_map->tex2d(u, v) * 2.0f - glm::vec3(1, 1, 1)) * TBN
		: t.interpolated_world_normal();
	//normal = t.interpolated_world_normal();
	float light_intensity = 0;
	for (int i = 0; i < 3; i++)
		light_intensity += varying_color[i] * t.cof[i];

	auto l_dir = context.l->LightDir(normal) * TBN;
	l_dir = glm::normalize(l_dir);
	normal = glm::normalize(normal);
	light_intensity = std::max(0.0f, glm::dot(l_dir, normal));
	auto diffuse = diffuse_map ? diffuse_map->tex2d(u, v) : glm::vec3(1, 1, 1);
	return diffuse * light_intensity;
}
