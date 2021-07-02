#include "SceneHierachyPanel.h"
#include "Proof/ImGui/ImGuiLayer.h"
#include "entt/entt.hpp"
#include "Proof3D/Scene/Entity.h"
#include "Proof3D/Scene/Component.h"
namespace Proof
{
	void SceneHierachyPanel::OnUpdate(FrameTime DeltaTime) {

	}
	void SceneHierachyPanel::ImGuiOnUpdate() {
		ImGui::Begin("Herieachy");
		m_CurrentWorld->Registry.each([&](auto entityID) {
			Entity entity{entityID,m_CurrentWorld};
			DrawEntityNode(entity);
		});
		if(ImGui::IsMouseDown(0) && ImGui::IsWindowHovered()){
			m_SelectedEntity = {};
		}
		ImGui::End();

		ImGui::Begin("Properties");
		if (m_SelectedEntity)
			DrawComponent(m_SelectedEntity);
		ImGui::End();
	}
	void SceneHierachyPanel::OnAttach() {

	}
	void SceneHierachyPanel::DrawEntityNode(Entity& entity) {
		auto& tc = entity.GetComponent<TagComponent>()->GetName();
		ImGuiTreeNodeFlags flags = ((m_SelectedEntity == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;

		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity.GetID(),flags,tc.c_str());
		if (ImGui::IsItemClicked()) {
			m_SelectedEntity = entity;
		}

		if (opened) {
			ImGui::TreePop();
		}
	}
	void SceneHierachyPanel::DrawComponent(Entity& entity) {
		if (entity.HasComponent<TagComponent>()) {
			char buffer[256];
			memset(buffer,0,sizeof(buffer));
			strcpy_s(buffer,sizeof(buffer),entity.GetComponent<TagComponent>()->GetName().c_str());
			if (ImGui::InputText("Name",buffer,sizeof(buffer))) {
				entity.GetComponent<TagComponent>()->SetName(buffer);
			}
		}

		if (entity.HasComponent<TransformComponent>()){
			if (ImGui::TreeNodeEx((void*) typeid(TransformComponent).hash_code(),ImGuiTreeNodeFlags_DefaultOpen,"Transform")) {
				ImGui::DragFloat("Location",&entity.GetComponent<TransformComponent>()->Location.X,0.1f);
				ImGui::TreePop();
			}
		}
	}
}