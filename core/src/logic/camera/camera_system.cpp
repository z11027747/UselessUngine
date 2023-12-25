
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include "logic/transform/transform_comp.h"
#include "logic/camera/camera_comp.h"
#include "logic/camera/camera_system.h"
#include "context.h"
#include "base.h"

namespace Logic {

	void CameraSystem::Add(std::shared_ptr<EngineObject> eo, Frustum frustum) {
		auto camera = std::make_shared<Camera>();
		camera->frustum = frustum;
		eo->AddComponent<Camera>(camera);

		UpdateVP(eo);
	}

	void CameraSystem::UpdateVP(std::shared_ptr<EngineObject> eo) {
		auto transform = eo->GetComponent<Transform>();
		auto& position = transform->position;
		auto& eulerAngles = transform->eulerAngles;

		glm::mat4 rotationMatrix = glm::eulerAngleYXZ(eulerAngles.y, eulerAngles.x, eulerAngles.z);
		auto forward = glm::vec3(rotationMatrix[2]);

		auto camera = eo->GetComponent<Camera>();

		camera->view = glm::lookAt(position, position + forward, glm::vec3(0, 0, 1));

		float near = camera->frustum.near;
		float far = camera->frustum.far;
		float aspect = camera->frustum.aspect;
		float fov = camera->frustum.fov;

		camera->projection = glm::perspective(glm::radians(fov), aspect, near, far);
	}

}