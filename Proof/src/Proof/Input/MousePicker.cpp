#include "Proofprch.h"
#include "MousePicker.h"
#include "Proof/Scene/Camera/EditorCamera.h"
namespace Proof{
	MousePicker::MousePicker(glm::mat4 ProjectionMatrix,EditorCamera3D& camera):
		m_ProjectionMatrix(glm::mat4(1.0f)),m_ViewMatrix(glm::mat4(1.0f))
	{
		m_ProjectionMatrix = ProjectionMatrix;
		m_ViewMatrix = camera.GetCameraView();
		m_Camera = &camera;
	}
}