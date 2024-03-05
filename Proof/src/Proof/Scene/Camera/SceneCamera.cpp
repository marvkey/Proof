#include "Proofprch.h"
#include "SceneCamera.h"

namespace Proof {
	SceneCamera::SceneCamera()
	{
	}

	void SceneCamera::SetPerspective(float degfov, uint32_t witdth, uint32_t height, float nearPlane, float farPlane, const glm::vec3& position, const glm::vec3& rotationRadians)
	{
		glm::vec3 cameraDirection;
		cameraDirection.x = cos(rotationRadians.z) * cos(rotationRadians.y);
		cameraDirection.y = sin(rotationRadians.y);
		cameraDirection.z = sin(rotationRadians.z) * cos(rotationRadians.y);
		SetPerspectiveProjectionMatrix(degfov, witdth, height, nearPlane, farPlane, position, cameraDirection);
	}
	
}
