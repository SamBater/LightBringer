#include "GroudShader.h"
#include "glm/fwd.hpp"
#include "glm/matrix.hpp"

glm::vec4 ylb::GroudShader::VertexShading(Vertex& vt,
	const VertexShaderContext& context) {
	glm::vec4 pos_h(vt.position, 1);

	// MVP
    auto MVP = (*context.project) * (*context.view) * (*context.model);
    glm::vec4 ccv_pos = MVP * pos_h;

	vt.light_coord = context.l->view_project_matrix * *context.model * pos_h;

	// float inv = vt.light_coord.w;
	// vt.light_coord *= inv;

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
		(normal_map->tex2d(u, v) * 2.0f - glm::vec3(1, 1, 1))
		: t.interpolated_world_normal();
	normal = t.interpolated_world_normal();
	float light_intensity = 0;
	for (int i = 0; i < 3; i++)
		light_intensity += varying_color[i] * t.cof[i];

	auto light_coord = 
	t.cof.x * t.vts[0].light_coord + 
	t.cof.y * t.vts[1].light_coord + 
	t.cof.z * t.vts[2].light_coord;
	light_coord *= t.s;

    float visibility = context.l->VisibleInLightSpace(light_coord);

	auto l_dir = context.l->LightDir(normal);
	l_dir = glm::normalize(l_dir);
	normal = glm::normalize(normal);
	light_intensity = std::max(0.0f, glm::dot(l_dir, normal));
	auto diffuse = diffuse_map ? diffuse_map->tex2d(u, v) : glm::vec3(1, 1, 1);

	static glm::vec3 ambient = {0.05,0.05,0.05};
	return diffuse * light_intensity * visibility + ambient;
}
