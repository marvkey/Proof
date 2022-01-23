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
					while (InputManager::m_ActionMapping.find(action) == InputManager::m_ActionMapping.end()) {
						iterate++;
						action = "NewAction(" + std::to_string(iterate) + ")";
					}
					InputManager::m_ActionMapping.emplace(action);

					for (auto& [name,action] : InputManager::m_ActionMapping) {
						ImGui::Text(name.c_str());
						for (auto& input : action.m_Inputs) {
						}
					}
				}
			}
		}
		ImGui::End();
	}
}