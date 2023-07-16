#pragma once
#include "Proof/Scene/Script.h"
#include "Proof/Input/Input.h"
#include "Proof/Input/Mouse.h"
#include "Proof/Input/InputManager.h"
#include "Proof/Core/Core.h"
namespace Proof{
	class Proof_API MovementScript:public Script{
	public:
		CameraComponent* m_CameraComponent;
		TransformComponent* m_Transform;
		float m_Speed =15.6;
		float m_Sensitivity=2;
		void OnCreate()override{
			/*
			m_CameraComponent = GetComponent<CameraComponent>();
			m_Transform = GetComponent<TransformComponent>();
			PF_ENGINE_INFO("Created all inputs bind");
			InputManager::BindMotion("MoveX", PF_BIND_MOTION(MovementScript::MoveX));
			InputManager::BindMotion("MoveY", PF_BIND_MOTION(MovementScript::MoveY));
			InputManager::BindAction("CursorEnabled", InputEvent::KeyClicked, []() {
				PF_ENGINE_INFO("Cursor enabled ");
				Mouse::CaptureMouse(true);
			});
			InputManager::BindAction("CursorDisabled", InputEvent::KeyReleased, []() {
				PF_ENGINE_INFO("Cursor disbaled ");
				Mouse::CaptureMouse(false);
			});

			InputManager::BindMotion("RotateX", PF_BIND_MOTION(MovementScript::RotateX));
			InputManager::BindMotion("RotateY", PF_BIND_MOTION(MovementScript::RotateY));
			*/
		};
		void OnUpdate(FrameTime DeltaTime)override{
			if(m_CameraComponent==nullptr)
				return;
			/*
			if (Input::IsMouseButtonPressed(MouseButton::ButtonRight)) {
				glfwSetInputMode((GLFWwindow*)CurrentWindow::GetWindowAPI(),GLFW_CURSOR,GLFW_CURSOR_DISABLED);
				ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NoMouse; // no mouse capture
				if (m_FirstClick == true) {
					m_FirstClick = false;
					m_MouseLastPosX = Input::GetMousePosX();
					m_MouseLastPosY = Input::GetMousePosY();
				}

				m_Xoffset = Mouse::GetPosX() - m_MouseLastPosX;
				m_Yoffset = m_MouseLastPosY - Mouse::GetPosY();

				m_Xoffset *= m_Sensitivity * DeltaTime;
				m_Yoffset *= m_Sensitivity * DeltaTime;
				m_Yaw += m_Xoffset;
				m_Pitch += m_Yoffset;

				if (m_Pitch > 89.0f)
					m_Pitch = 89.0f;
				if (m_Pitch < -89.0f)
					m_Pitch = -89.0f;
				glm::vec3 CameraDirection;
				CameraDirection.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
				CameraDirection.y = sin(glm::radians(m_Pitch));
				CameraDirection.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
				GetComponent<TransformComponent>().Rotation = CameraDirection;
				m_MouseLastPosX = Input::GetMousePosX();
				m_MouseLastPosY = Input::GetMousePosY();
			}

			if (Input::IsMouseButtonReleased(MouseButton::ButtonRight)) {
				m_FirstClick = true;
				glfwSetInputMode((GLFWwindow*)CurrentWindow::GetWindowAPI(),GLFW_CURSOR,GLFW_CURSOR_NORMAL);
				ImGui::GetIO().ConfigFlags &= ~ImGuiConfigFlags_NoMouse; // alllows mouse capture
			}
			*/
			//if (Input::IsKeyPressed(KeyBoardKey::W)) {
			//	float tempy = m_Transform->Rotation.Y;
			//	m_Transform->Location += DeltaTime* m_Speed * m_Transform->Rotation;
			//	m_Transform->Rotation.Y = tempy;
			//}
		
			//if (Input::IsKeyPressed(KeyBoardKey::S)) {
			//	float tempy = m_Transform->Rotation.Y;
			//	m_Transform->Location += m_Speed * DeltaTime * -m_Transform->Rotation;
			//	m_Transform->Rotation.Y = tempy;
			//}
			
			//if (Input::IsKeyPressed(KeyBoardKey::A)) {
			//	float tempy = m_Transform->Rotation.Y;
			//	m_Transform->Location += m_Speed * DeltaTime * -Vector<float>::Normalize(Vector<float>::Cross(m_Transform->Rotation, m_CameraComponent->m_Up));
			//	m_Transform->Rotation.Y = tempy;
			//}
			
			//if (Input::IsKeyPressed(KeyBoardKey::D)) {
			//	float tempy = m_Transform->Rotation.Y;
			//	m_Transform->Location+= m_Speed * DeltaTime * Vector<float>::Normalize(Vector<float>::Cross(m_Transform->Rotation,m_CameraComponent->m_Up));
			//	m_Transform->Rotation.Y = tempy;
			//}
		}
	private:
		/*
		void MoveX(float motionValue) {
			if (Mouse::IsMouseCaptured() == false)return;
			PF_ENGINE_INFO("moved X");
			m_Transform->Location += m_Speed * FrameTime::GetWorldDeltaTime() * m_Transform->GetFowardVector() * motionValue;
		}
		void MoveY(float motionValue) {
			if (Mouse::IsMouseCaptured() == false)return;
			PF_ENGINE_INFO("moved Y");
			m_Transform->Location += FrameTime::GetWorldDeltaTime() * m_Speed * m_Transform->GetRightVector() * motionValue;
		}
		void RotateX(float motionValue) {
			if (Mouse::IsMouseCaptured() == false)return;
			PF_ENGINE_INFO("Rotate X");
			m_Transform->Rotation.Z += m_Speed * FrameTime::GetWorldDeltaTime() * motionValue;
			//if (m_Transform->Rotation.Z > 89)
			//	m_Transform->Rotation.Z = 89;
			//
			//if (m_Transform->Rotation.Z < -89.0f)
			//	m_Transform->Rotation.Z = -89.0f;
		}

		void RotateY(float motionValue) {
			if (Mouse::IsMouseCaptured() == false)return;
			PF_ENGINE_INFO("Rotate Y");
			m_Transform->Rotation.Y += m_Speed * FrameTime::GetWorldDeltaTime() * motionValue;
			if (m_Transform->Rotation.Y > 89)
				m_Transform->Rotation.Y = 89;
			
			if (m_Transform->Rotation.Y < -89.0f)
				m_Transform->Rotation.Y = -89.0f;
		}
		*/
		bool m_FirstClick=true;
		float m_MouseLastPosX;
		float m_MouseLastPosY;
		float m_Yaw =-90.f;
		float m_Pitch =0;
		float m_Xoffset,m_Yoffset;
	};
}	