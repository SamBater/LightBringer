#pragma once
#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"
#include "YLBSerializable.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
namespace ylb {
	class Actor;
	class Transform : public YLBSerializable {
	public:
		Transform() {
			BuildModelMatrix();
		}

		Transform(const glm::vec3& model_coords, const glm::vec3& scale) : model_coords(model_coords), scale(scale) {
			BuildModelMatrix();
		}

		const glm::mat4& ModelMatrix() const {
			return modelMatrix;
		}

		const glm::vec3& WorldPosition() const {
			return model_coords;
		}

		void SetPosition(const glm::vec3& model_coords) {
			this->model_coords = model_coords;
		}

		virtual void DeSerilization(const json11::Json& json) override {
			model_coords = DeSerilizationVec3(json["position"]);
			scale = DeSerilizationVec3(json["scale"]);
			BuildModelMatrix();
		}
	private:
		friend Actor;

		void BuildModelMatrix() {
			ModifiedScaleParms();
			ModifiedTranslateParms();
		}

		void ModifiedTranslateParms() {
			modelMatrix[3][0] = model_coords.x;
			modelMatrix[3][1] = model_coords.y;
			modelMatrix[3][2] = model_coords.z;
            //modelMatrix = glm::translate(modelMatrix, model_coords / scale);
		}
		void ModifiedScaleParms() {
            modelMatrix = glm::scale(modelMatrix, scale);
		}
		glm::vec3 model_coords = glm::vec3(0, 0, 0);
		glm::vec3 scale = glm::vec3(1, 1, 1);
		glm::mat4 modelMatrix = glm::mat4(1);
	};
}