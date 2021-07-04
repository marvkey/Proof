#include "SceneHierachyPanel.h"
#include "Proof/ImGui/ImGuiLayer.h"
#include "entt/entt.hpp"
#include "Proof3D/Scene/Entity.h"
#include "Proof3D/Scene/Component.h"
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
namespace Proof
{

	void SceneHierachyPanel::ImGuiOnUpdate() {
		ImGui::Begin("Herieachy");
		for (uint32_t i = 0; i <= m_CurrentWorld->Registry.GetAllID().size() - 1; i++) {
			Entity entity = {m_CurrentWorld->Registry.GetAllID().at(i),m_CurrentWorld};
			DrawEntityNode(entity);
			if (m_CurrentWorld->Registry.GetAllID().size() == 1)break;
		}

		if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered()) {
			m_SelectedEntity = {};
		}
		if (ImGui::BeginPopupContextWindow(0,1,false)){ // right click adn open a new entitiy
			if (ImGui::MenuItem("Create Entity"))
				m_CurrentWorld->CreateEntity("Empty Entity");
			ImGui::EndPopup();
		}
		ImGui::End();

		ImGui::Begin("Properties");
		if (m_SelectedEntity)
			DrawComponent(m_SelectedEntity);
		ImGui::End();
	}
	void SceneHierachyPanel::DrawEntityNode(Entity& entity) {
		auto& tc = entity.GetComponent<TagComponent>()->GetName();
		ImGuiTreeNodeFlags flags = ((m_SelectedEntity == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		flags |= ImGuiTreeNodeFlags_SpanFullWidth;
		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity.GetID(),flags,tc.c_str());
		if (ImGui::IsItemClicked()) {
			m_SelectedEntity = entity;
		}

		if (opened) {
			ImGui::TreePop();
		}
	}

	template<typename T,typename UIFunction>
	void SceneHierachyPanel::DrawComponents(const std::string& name,Entity& entity,T* Comp,uint32_t IndexValue,UIFunction Uifunction) {
		const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;
		auto& component = *Comp;
		ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();

		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding,ImVec2{1.5,1.5});
		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImGui::Separator();
		bool open = false;
		open = ImGui::TreeNodeEx((void*)&(*Comp),treeNodeFlags,name.c_str());
		ImGui::PopStyleVar();
		ImGui::SameLine(contentRegionAvailable.x - lineHeight * 0.5f);
		if (ImGui::Button("+",ImVec2{lineHeight,lineHeight})) {
			ImGui::OpenPopup("ComponentSettings");
		}

		bool removeComponent = false;
		if (ImGui::BeginPopup("ComponentSettings")) {
			if (ImGui::MenuItem("Remove component"))
				removeComponent = true;
			ImGui::EndPopup();
		}

		if (open) {
			Uifunction(component);
			ImGui::Text("Index Value: %i ",IndexValue);

			ImGui::TreePop();
		}

		if (removeComponent) {
			entity.RemoveComponent(IndexValue);
		}
	}

	void SceneHierachyPanel::DrawComponent(Entity& entity) {
		auto& Tag = *entity.GetComponent<TagComponent>();
		char buffer[256];
		memset(buffer,0,sizeof(buffer));
		strcpy_s(buffer,sizeof(buffer),Tag.GetName().c_str());
		ImGui::SameLine();
		if (ImGui::InputText("##Tag",buffer,sizeof(buffer))) {
			Tag.SetName(buffer);
		}
		ImGui::SameLine();
		ImGui::PushItemWidth(-1);
		if(ImGui::Button("Add Component"))
			ImGui::OpenPopup("AddComponent");
		ImGui::PopItemWidth();
		if(ImGui::BeginPopup("AddComponent")){
			
			if(ImGui::MenuItem("Mesh Component")){
				
				ImGui::CloseCurrentPopup();
			}

			if(ImGui::MenuItem("Sprite Renderer")){
				
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}

		uint32_t IndexValue= 0;
		for(Component* Comp: (*m_CurrentWorld->Registry.GetEntities().at(entity.GetID()))){
			TagComponent*Tag= dynamic_cast<TagComponent*>(Comp);
			if(Tag != nullptr){
				DrawComponents<TagComponent>("Tag",entity,Tag,IndexValue,[](auto& component) {
					
				});
				IndexValue += 1;
				continue;
			}
			TransformComponent* Transfrom = dynamic_cast<TransformComponent*>(Comp);
			if (Transfrom != nullptr) {
				DrawComponents<TransformComponent>("Transform",entity,Transfrom,IndexValue,[](auto& component) {
					DrawVectorControl("Location",component.Location);
					DrawVectorControl("Rotation",component.Rotation);
					DrawVectorControl("Scale",component.Scale,1.0f);
				});
				IndexValue += 1;
				continue;
			}

			MeshComponent* Meshes = dynamic_cast<MeshComponent*>(Comp);
			if (Meshes != nullptr) {
				DrawComponents<MeshComponent>({"Mesh: " + Meshes->GetName()},entity,Meshes,IndexValue,[](auto& component) {
					char buffer[256];
					memset(buffer,0,sizeof(buffer));
					strcpy_s(buffer,sizeof(buffer),component.GetName().c_str());
					if (ImGui::InputText("##Name",buffer,sizeof(buffer))) {
						component.SetName(buffer);
					}
					DrawVectorControl("Local Location",component.MeshLocalTransform.Location,0.0,125);
					DrawVectorControl("Local Rotation",component.MeshLocalTransform.Rotation,0.0,125);
					DrawVectorControl("Local Scale",component.MeshLocalTransform.Scale,0.0,125);
				});
				IndexValue += 1;
				continue;
			}
		}
	}
	void SceneHierachyPanel::DrawMeshComponent(MeshComponent* Meshes) {
		if (Meshes != nullptr) {
			if (ImGui::TreeNodeEx((void*)&(*Meshes),ImGuiTreeNodeFlags_None,Meshes->GetName().c_str())) {// &(*Meshes) is the unique ID wich is the element its pointing to in memory
				ImGui::Text("Name");
				char buffer[256];
				memset(buffer,0,sizeof(buffer));
				strcpy_s(buffer,sizeof(buffer),Meshes->GetName().c_str());
				ImGui::SameLine();
				if (ImGui::InputText("##Name",buffer,sizeof(buffer))) {
					Meshes->SetName(buffer);
				}
				DrawVectorControl("Local Location",Meshes->MeshLocalTransform.Location);
				DrawVectorControl("Local Rotation",Meshes->MeshLocalTransform.Rotation);
				DrawVectorControl("Local Scale",Meshes->MeshLocalTransform.Scale,0.0f);
				ImGui::TreePop();
			}
		}
	}

	void SceneHierachyPanel::DrawVectorControl(const std::string& UniqeLabel,Vector& Vec,float ResetValue,float columnWidth) {
		ImGuiIO& io = ImGui::GetIO();
		auto boldFont = io.Fonts->Fonts[0];
		ImGui::PushID(UniqeLabel.c_str());// this id is for everything here so imgui does not assign something to the value that we have here
		ImGui::Columns(2); // distance between label and edits
		ImGui::SetColumnWidth(0,columnWidth);
		ImGui::Text(UniqeLabel.c_str());
		ImGui::NextColumn();

		ImGui::PushMultiItemsWidths(3,ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing,ImVec2{0,0});

		float LineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f; // comes from IMGUI
		ImVec2 buttonSize = {LineHeight + 3.0f,LineHeight};

		ImGui::PushStyleColor(ImGuiCol_Button,ImVec4{1.0f,0.0f,0.0f,1.0f});
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered,ImVec4{1.0f,0.5f,0.0f,1.0f});
		ImGui::PushStyleColor(ImGuiCol_ButtonActive,ImVec4{1.0f,0.0f,0.0f,1.0f});
		ImGui::PushFont(boldFont);
		if (ImGui::Button("X",buttonSize)) {
			Vec.X = ResetValue;
		}
		ImGui::PopFont();
		ImGui::SameLine();
		ImGui::DragFloat("##x",&Vec.X,0.1f,0,0,"%.2f"); // does not show ## as label
		ImGui::PopItemWidth();
		ImGui::SameLine();
		ImGui::PopStyleColor(3);

		ImGui::PushStyleColor(ImGuiCol_Button,ImVec4{0.0f,.5f,0.0f,1.0f});
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered,ImVec4{1.0f,0.5f,0.0f,1.0f});
		ImGui::PushStyleColor(ImGuiCol_ButtonActive,ImVec4{0.0f,.5f,0.0f,1.0f});

		ImGui::PushFont(boldFont);
		if (ImGui::Button("Y",buttonSize)) {
			Vec.Y = ResetValue;
		}
		ImGui::PopFont();

		ImGui::SameLine();
		ImGui::DragFloat("##Y",&Vec.Y,0.1f,0,0,"%.2f"); // does not show ## as label
		ImGui::PopItemWidth();
		ImGui::SameLine();
		ImGui::PopStyleColor(3);

		ImGui::PushStyleColor(ImGuiCol_Button,ImVec4{0.0f,0.0f,1.0f,1.0f});
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered,ImVec4{1.0f,0.5f,0.0f,1.0f});
		ImGui::PushStyleColor(ImGuiCol_ButtonActive,ImVec4{0.0f,0.0f,1.0f,1.0f});

		ImGui::PushFont(boldFont);
		if (ImGui::Button("Z",buttonSize)) {
			Vec.Z = ResetValue;
		}
		ImGui::PopFont();

		ImGui::SameLine();
		ImGui::DragFloat("##Z",&Vec.Z,0.1f,0,0,"%.2f"); // does not show ## as label
		ImGui::PopItemWidth();
		ImGui::PopStyleColor(3);

		ImGui::PopStyleVar();
		ImGui::Columns(1);
		ImGui::PopID();
	}
}