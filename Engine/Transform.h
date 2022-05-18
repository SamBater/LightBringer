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

		Transform(const glm::vec3& position, const glm::vec3& scale) : position(position), scale(scale) {
			BuildModelMatrix();
		}

		const glm::mat4& ModelMatrix() const {
			return modelMatrix;
		}

		const glm::vec3& WorldPosition() const {
			return position;
		}

		void SetPosition(const glm::vec3& position) {
			this->position = position;
		}

		virtual void DeSerilization(const json11::Json& json) override {
			position = DeSerilizationVec3(json["position"]);
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
			modelMatrix[0][3] = position.x;
			modelMatrix[1][3] = position.y;
			modelMatrix[2][3] = position.z;
		}
		void ModifiedScaleParms() {
			modelMatrix[0][0] = scale.x;
			modelMatrix[1][1] = scale.y;
			modelMatrix[2][2] = scale.z;
		}
		glm::vec3 position = glm::vec3(0, 0, 0);
		glm::vec3 scale = glm::vec3(1, 1, 1);
		glm::mat4 modelMatrix = glm::mat4(1);
	};
}