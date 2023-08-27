#include "Proofprch.h"
#include "SceneCamera.h"

namespace Proof {
	SceneCamera::SceneCamera()
	{
	}

	void SceneCamera::SetPerspective(float degfov, uint32_t witdth, uint32_t height, float nearPlane, float farPlane, const glm::vec3& position, const glm::vec3& rotation)
	{
		glm::vec3 cameraDirection;
		cameraDirection.x = cos(glm::radians(rotation.z)) * cos(glm::radians(rotation.y));
		cameraDirection.y = sin(glm::radians(rotation.y));
		cameraDirection.z = sin(glm::radians(rotation.z)) * cos(glm::radians(rotation.y));
		SetPerspectiveProjectionMatrix(degfov, witdth, height, nearPlane, farPlane, position, cameraDirection);
	}
	
}
