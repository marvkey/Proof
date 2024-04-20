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
		//https://github.com/Ant-Play/Ant/blob/2dab7c0362f017911df9090b1608ec4b81ad1f2c/Ant/src/Ant/Scene/SceneCamera.cpp
		//https://github.com/Ant-Play/Ant/blob/2dab7c0362f017911df9090b1608ec4b81ad1f2c/Ant/src/Ant/Renderer/Camera.h#L24

		glm::mat4 projection =glm::perspectiveFov(glm::radians(degfov), (float)width, (float)height, nearPlane, farPlane);
		glm::mat4 unReversedProjectionMatrix = glm::perspectiveFov(glm::radians(degfov), (float)width, (float)height, nearPlane, farPlane);

		SetProjectionMatrix(projection, view,unReversedProjectionMatrix);
	}
	
}
