#include"InputPanel.h"
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include "../ImGUIAPI.h"
#include "Proof/Utils/PlatformUtils.h"
#include "../Editor3D.h"
#include "Proof/Renderer/3DRenderer/Renderer3DPBR.h"
#include "Proof/Input/InputManager.h"
#include <unordered_map>
namespace Proof {
	void InputPanel::ImGuiRender(FrameTime deltaTime){
		if (m_ShowWindow == false)
			return;

		PF_PROFILE_FUNC();

		ImGui::Begin("Input Panel", &m_ShowWindow);
		{
			const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;

			bool opened = ImGui::TreeNodeEx((void*)"Action Mapping", treeNodeFlags, "Action Mapping");
			if (opened) {
				Action();
				ImGui::TreePop();
			}
			bool openedMotion = ImGui::TreeNodeEx((void*)"Motion Mapping", treeNodeFlags, "Motion Mapping");

			if (openedMotion) {
				MotionInput();
				ImGui::TreePop();
			}
		}
		ImGui::End();
	}
	void InputPanel::Action()
	{
		ImGui::SameLine();
		if (ImGui::Button("+", { 20,20 })) {
			std::string action = "NewAction";
			int iterate = 0;
			while (InputManager::S_ActionMapping.find(action) != InputManager::S_ActionMapping.end()) {
				iterate++;
				action = "NewAction(" + std::to_string(iterate) + ")";
			}

			PF_INFO("action added");
			InputManager::AddAction(action);
		}

		for (auto& [name, action] : InputManager::S_ActionMapping) {
			ImGui::Text(name.c_str());
			for (InputType& device : action.m_Inputs) {

				ImGui::SameLine();
				ImGui::Text(std::to_string((char)device.Key).c_str());
			}
			ImGui::SameLine();

			if (ImGui::Button("New Key"))
				ImGui::OpenPopup("Add Key");

			if (ImGui::BeginPopup("Add Key")) {
				if (ImGui::MenuItem("R")) {
					InputManager::ActionAddKey(name, InputType(InputDevice::KeyBoard, (int)KeyBoardKey::R));
				}
				ImGui::EndPopup();
			}

		}
	}
	void InputPanel::MotionInput(){
		ImGui::SameLine();
		if (ImGui::Button("+", { 20,20 })) {
			std::string action = "NewMotion";
			int iterate = 0;
			while (InputManager::s_MotionMapping.find(action) != InputManager::s_MotionMapping.end()) {
				iterate++;
				action = "NewMotion(" + std::to_string(iterate) + ")";
			}

			PF_INFO("New Motion");
			InputManager::AddMotion(action);
		}
		int iterationMotion = 0;
		for (auto& [name, motion] : InputManager::s_MotionMapping) {
			ImGui::Text(name.c_str());
			int iteration = 0;
			for (MotionInputType& device : motion.Inputs) {
				ImGui::PushID(iteration+iterationMotion);
				std::stringstream ss;
				ss << (char)device.Key;
				ImGui::Text(ss.str().c_str());
				ImGui::SameLine();
				ImGui::SliderFloat("MotionValue", &device.MotionValue,-1000,1000);
				ImGui::PopID();
				iteration++;
			}
			ImGui::SameLine();

			if (ImGui::Button("New Key")) {
				m_EditedValue = name;
				ImGui::OpenPopup("Add Key");
			}
			if (m_EditedValue != name)
				goto a;
			if (ImGui::BeginPopup("Add Key")) {
				if (ImGui::MenuItem("W")) {
					m_EditedValue = "";
					InputManager::MotionAddKey(name, MotionInputType(InputDevice::KeyBoard, (int)KeyBoardKey::W));
				}
				if (ImGui::MenuItem("S")) {
					m_EditedValue = "";
					InputManager::MotionAddKey(name, MotionInputType(InputDevice::KeyBoard, (int)KeyBoardKey::S));
				}
				if (ImGui::MenuItem("A")) {
					m_EditedValue = "";
					InputManager::MotionAddKey(name, MotionInputType(InputDevice::KeyBoard, (int)KeyBoardKey::A));
				}
				if (ImGui::MenuItem("D")) {
					m_EditedValue = "";
					InputManager::MotionAddKey(name, MotionInputType(InputDevice::KeyBoard, (int)KeyBoardKey::D));
				}
				ImGui::EndPopup();
			}
			a:
			iterationMotion++;
		}
	}
}