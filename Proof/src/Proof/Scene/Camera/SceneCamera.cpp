#include "Proofprch.h"
#include "SceneCamera.h"

namespace Proof {
	SceneCamera::SceneCamera()
	{
	}

	void SceneCamera::SetPerspective(float degfov, uint32_t witdth, uint32_t height, float nearPlane, float farPlane, const glm::vec3& position, const glm::vec3& rotationRadians)
	{
		glm::vec3 cameraDirection;
		cameraDirection.x = cos(rotationRadians.x) * cos(rotationRadians.y);
		cameraDirection.y = sin(rotationRadians.y);
		cameraDirection.z = sin(rotationRadians.x) * cos(rotationRadians.y);
		SetPerspectiveProjectionMatrix(degfov, witdth, height, nearPlane, farPlane, position, cameraDirection);
	}

	void SceneCamera::SetData(float degfov, float nearPlane, float farPlane, uint32_t width, uint32_t height, const glm::mat4& view)
	{
		glm::mat4 projection = glm::perspective(glm::radians(degfov), float(width / height), nearPlane, farPlane);
		glm::mat4 unReversedProjectionMatrix = glm::perspectiveFov(glm::radians(degfov), (float)width, (float)height, nearPlane, farPlane);

		SetProjectionMatrix(projection, view,unReversedProjectionMatrix);
	}
	
}
