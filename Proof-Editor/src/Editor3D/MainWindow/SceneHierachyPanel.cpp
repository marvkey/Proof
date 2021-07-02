#include "SceneHierachyPanel.h"
#include "Proof/ImGui/ImGuiLayer.h"
#include "entt/entt.hpp"
#include "Proof3D/Scene/Entity.h"
#include "Proof3D/Scene/Component.h"
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
namespace Proof
{
	void SceneHierachyPanel::OnUpdate(FrameTime DeltaTime) {

	}
	void SceneHierachyPanel::ImGuiOnUpdate() {
		ImGui::Begin("Herieachy");
		for(uint32_t i=0;i<= m_CurrentWorld->Registry.GetEntities().size();i++){
			Entity entity= {m_CurrentWorld->Registry.GetEntities().begin()->first,m_CurrentWorld};
			DrawEntityNode(entity);
			if(m_CurrentWorld->Registry.GetEntities().size() ==1)break;
		}
		
		if(ImGui::IsMouseDown(0) && ImGui::IsWindowHovered()){
			m_SelectedEntity ={};
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
			auto Tag = entity.GetComponent<TagComponent>();
			if (ImGui::TreeNodeEx((void*) typeid(TagComponent).hash_code(),ImGuiTreeNodeFlags_DefaultOpen,Tag->GetName().c_str())){
				char buffer[256];
				memset(buffer,0,sizeof(buffer));
				strcpy_s(buffer,sizeof(buffer),entity.GetComponent<TagComponent>()->GetName().c_str());

				if (ImGui::InputText("##Tag",buffer,sizeof(buffer))) {
					Tag->SetName(buffer);
				}
				if(ImGui::Button("+"))
					ImGui::OpenPopup("AddTag");
				if(ImGui::BeginPopup("AddTag")) {
					std::string NewTag;
					char NewBuffer[250];
					memset(NewBuffer,0,sizeof(NewBuffer));
					strcpy_s(NewBuffer,sizeof(NewBuffer),NewTag.c_str());
					ImGui::Text("NewTag");
					ImGui::SameLine();
					if(ImGui::InputText("",NewBuffer,sizeof(NewBuffer))) {
						ImGui::SameLine();
						if(ImGui::Button("Done")){
							Tag->AddTag(NewBuffer);
						}
					}
					ImGui::EndPopup();
				}
				ImGui::SameLine();
				for(uint32_t i=0; i<Tag->Tags.size(); i++){
					ImGui::Text(Tag->Tags[i].c_str());
					ImGui::SameLine();
				}
				ImGui::TreePop();
			}
		}
		
		ImGui::NewLine();
		if (entity.HasComponent<TransformComponent>()){
			if(ImGui::TreeNodeEx((void*) typeid(TransformComponent).hash_code(),ImGuiTreeNodeFlags_DefaultOpen,"Transform")){
				auto& Transform = *entity.GetComponent<TransformComponent>();
				DrawVectorControl("Location",Transform.Location);
				DrawVectorControl("Rotation",Transform.Rotation);
				DrawVectorControl("Scale",Transform.Scale,1.0f);
				ImGui::TreePop();
			}
		}
	}
	void SceneHierachyPanel::DrawVectorControl(const std::string& UniqeLabel,Vector& Vec,float ResetValue,float columnWidth ) {
		ImGui::PushID(UniqeLabel.c_str());// this id is for everything here so imgui does not assign something to the value that we have here
		ImGui::Columns(2); // distance between label and edits
		ImGui::SetColumnWidth(0,columnWidth);
		ImGui::Text(UniqeLabel.c_str());
		ImGui::NextColumn();
		ImGui::PushMultiItemsWidths(3,ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing,ImVec2{0,0});
		float LineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f; // comes from IMGUI
		ImVec2 buttonSize ={LineHeight+3.0f,LineHeight};
		
		ImGui::PushStyleColor(ImGuiCol_Button,ImVec4{1.0f,0.0f,0.0f,1.0f});
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered,ImVec4{1.0f,0.5f,0.0f,1.0f});
		ImGui::PushStyleColor(ImGuiCol_ButtonActive,ImVec4{1.0f,0.0f,0.0f,1.0f});
		if(ImGui::Button("X",buttonSize)){
			Vec.X =ResetValue;
		}
		ImGui::SameLine();
		ImGui::DragFloat("##x",&Vec.X,0.1f,0,0,"%.2f"); // does not show ## as label
		ImGui::PopItemWidth();
		ImGui::SameLine();
		ImGui::PopStyleColor(3);

		ImGui::PushStyleColor(ImGuiCol_Button,ImVec4{0.0f,.5f,0.0f,1.0f});
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered,ImVec4{1.0f,0.5f,0.0f,1.0f});
		ImGui::PushStyleColor(ImGuiCol_ButtonActive,ImVec4{0.0f,.5f,0.0f,1.0f});
		if (ImGui::Button("Y",buttonSize)) {
			Vec.Y = ResetValue;
		}
		ImGui::SameLine();
		ImGui::DragFloat("##Y",&Vec.Y,0.1f,0,0,"%.2f"); // does not show ## as label
		ImGui::PopItemWidth();
		ImGui::SameLine();
		ImGui::PopStyleColor(3);

		ImGui::PushStyleColor(ImGuiCol_Button,ImVec4{0.0f,0.0f,1.0f,1.0f});
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered,ImVec4{1.0f,0.5f,0.0f,1.0f});
		ImGui::PushStyleColor(ImGuiCol_ButtonActive,ImVec4{0.0f,0.0f,1.0f,1.0f});
		if (ImGui::Button("Z",buttonSize)) {
			Vec.Z = ResetValue;
		}
		ImGui::SameLine();
		ImGui::DragFloat("##Z",&Vec.Z,0.1f,0,0,"%.2f"); // does not show ## as label
		ImGui::PopItemWidth();
		ImGui::PopStyleColor(3);

		ImGui::PopStyleVar();
		ImGui::Columns(1);
		ImGui::PopID();
	}
	
}