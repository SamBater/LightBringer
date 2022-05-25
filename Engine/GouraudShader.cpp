#include "GouraudShader.h"
#include "glm/fwd.hpp"
#include "glm/matrix.hpp"

glm::vec4 ylb::GouraudShader::VertexShading(Vertex& vt,
	const VertexShaderContext& context) {
	glm::vec4 pos_h(vt.model_coords, 1);

	// MVP
    auto MVP = (*context.project) * (*context.view) * (*context.model);
    glm::vec4 ccv_pos = MVP * pos_h;

	vt.light_coords = context.l->view_project_matrix * *context.model * pos_h;

	// float inv = vt.light_coords.w;
	// vt.light_coords *= inv;

	const auto& model = (glm::mat3(*context.model));
	glm::vec3 T = glm::normalize(glm::vec3(model * glm::vec4(vt.tangent, 0.0)));
	glm::vec3 N = glm::normalize(glm::vec3(model * glm::vec4(vt.normal, 0.0)));
	auto B = glm::normalize(glm::cross(N, T));
	TBN = glm::mat3(T, B, N);
	TBN = glm::transpose(TBN);

	auto normal = normal_map ?
                      TBN * (normal_map->tex2d(vt.u(), vt.v()) * 2.0f - glm::vec3(1, 1, 1)) :
                      vt.normal;
    normal = glm::normalize(normal);
	
	auto l_dir = TBN * context.l->LightDir(normal);
    l_dir = glm::normalize(l_dir);
    normal = glm::normalize(normal);

	varying_color[context.nthVertex] = std::max(0.0f, glm::dot(l_dir, normal));
	return ccv_pos;
}

glm::vec3
ylb::GouraudShader::FragmentShading(Triangle& t,
	const FragmentShaderContext& context) {
	float u, v;
	t.interpolated_uv(u, v);
    float light_intensity = glm::dot(varying_color, t.bar);

	//auto light_coords = 
	//t.bar.x * t.vts[0].light_coords + 
	//t.bar.y * t.vts[1].light_coords + 
	//t.bar.z * t.vts[2].light_coords;
	//light_coords *= t.inv_w;
    //float visibility = context.l->VisibleInLightSpace(light_coords);
	float visibility = 1.0;

	auto diffuse = diffuse_map ? diffuse_map->tex2d(u, v) : glm::vec3(1, 1, 1);

	static glm::vec3 ambient = {0.0,0.0,0.0};
	return diffuse * light_intensity * visibility + ambient;
}
