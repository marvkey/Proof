#pragma once
#include "Camera.h"
namespace Proof
{
	//https://github.com/InCloudsBelly/X2_RenderingEngine/blob/406c83663f46e08b882df40ef4ece1dc8e740c5c/Engine/X2/Scene/SceneCamera.h
	enum class ProjectionType { None = 0 ,Orthographic = 1, Perspective = 2 };
	class SceneCamera : public Camera
	{
	public:
		SceneCamera();
		void SetPerspective(float degfov, uint32_t witdth, uint32_t height, float nearPlane, float farPlane, const glm::vec3& position, const glm::vec3& rotation);
	};
}