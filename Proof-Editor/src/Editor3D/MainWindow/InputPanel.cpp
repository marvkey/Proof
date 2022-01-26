#include"InputPanel.h"
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include "../ImGUIAPI.h"
#include "Proof/Utils/PlatformUtils.h"
#include "../Editor3D.h"
#include "Proof/Renderer/3DRenderer/Renderer3DPBR.h"
#include "Proof/Input/InputManager.h"
namespace Proof {
	void InputPanel::ImGuiRender(FrameTime deltaTime){
		if (m_ShowWindow == false)
			return;
		ImGui::Begin("Input Panel", &m_ShowWindow);
		{
			const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;

			bool opened = ImGui::TreeNodeEx((void*)"Action Mapping", treeNodeFlags, "Action Mapping");
			if (opened) {
				ImGui::SameLine();
				if(ImGui::Button("+", { 20,20 })) {
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
						char keyName =(char) device.Key;
						ImGui::Text(&keyName);
					}
					ImGui::SameLine();

					if (ImGui::Button("New Key"))
						ImGui::OpenPopup("Add Key");
					
					if (ImGui::BeginPopup("Add Key")) {
						if (ImGui::MenuItem("R")) {
							InputManager::ActionAddKey(name, InputType(InputDevice::KeyBoard,(int) KeyBoardKey::R));
						}
						ImGui::EndPopup();
					}
					
				}
				ImGui::TreePop();
			}
		}
		ImGui::End();
	}
}