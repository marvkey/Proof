#include "SceneHierachyPanel.h"
#include "Proof/ImGui/ImGuiLayer.h"
#include "Proof/Scene/Entity.h"
#include "Proof/Scene/Component.h"
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include "Proof/Scene/EntitiyComponentSystem/ECS.h"
#include "Proof/Renderer/3DRenderer/Renderer3D.h" // TEMPORARY

#include "Proof/Resources/Asset/TextureAsset/TextureAsset.h"
#include "Proof/Resources/Asset/MeshAsset.h"
#include "Proof/Resources/Asset/Asset.h"
#include "Proof/Scene/World.cpp"
#include "ContentBrowserPanel.h"
#include <vector>
#include "Proof/Resources/Asset/MaterialAsset.h"
#include "../ImGUIAPI.h"
#include "Proof/Scene/ExampleSccripts.h"
#include "Proof/Scene/Camera/Camera.h"
#include "Proof/Scene/Script.h"
namespace Proof{
#define InitilizeScript(InstanceNativeScriptComponent,Class)\
	InstanceNativeScriptComponent.Bind<Class>();\
	InstanceNativeScriptComponent.SetName(#Class)

	static MeshAsset* TempAsset =nullptr;
	static MeshAsset* TempLocation0Asset = nullptr;
	void SceneHierachyPanel::ImGuiRender(){
		ImGui::Begin("Herieachy");
		{
			for (uint32_t i = 0; i < m_CurrentWorld->Registry.GetAllID().size(); i++) {
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
		}
		ImGui::End();
		ImGui::Begin("Properties");
		{
			if (m_SelectedEntity)
				DrawComponent(m_SelectedEntity);
		}
		ImGui::End();
	}
	void SceneHierachyPanel::DrawEntityNode(Entity& entity) {
		auto& tc = entity.GetComponent<TagComponent>()->GetName();
		ImGuiTreeNodeFlags flags = ((m_SelectedEntity == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		flags |= ImGuiTreeNodeFlags_SpanFullWidth;
		bool opened = ImGui::TreeNodeEx((void*)(EntityID)(uint32_t)entity.GetID(),flags,tc.c_str());
		if (ImGui::IsItemClicked() || ImGui::IsItemClicked(1)) {
			m_SelectedEntity = entity;
		}
		if(ImGui::BeginPopupContextItem()){
			if(ImGui::MenuItem("Delete")){
				m_CurrentWorld->Registry.Delete(m_SelectedEntity.GetID());
				m_SelectedEntity ={};
			}
			ImGui::EndPopup();
		}
		if(ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0) && m_SelectedEntity){
			
			m_CurrentWorld->m_EditorCamera.m_Positon = m_SelectedEntity.GetComponent<TransformComponent>()->Location;
		}
		if (opened) {
			ImGui::TreePop();
		}
	}

	template<typename T,typename UIFunction>
	void SceneHierachyPanel::DrawComponents(const std::string& name,Entity& entity,T* Comp,uint32_t IndexValue,UIFunction Uifunction) {
		ImGui::PushID(&(*Comp));
		const ImGuiTreeNodeFlags treeNodeFlags =ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;
		T& component = *Comp;
		ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();

		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding,ImVec2{1.5,1.5});
		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImGui::Separator();
		bool open = false;
		open = ImGui::TreeNodeEx((void*)&(*Comp),treeNodeFlags,name.c_str());
		ImGui::PopStyleVar();
		ImGui::SameLine(contentRegionAvailable.x - lineHeight * 0.5f);
		if (ImGui::Button("+",ImVec2{lineHeight+3,lineHeight})) {
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
			entity.RemoveComponent<T>(IndexValue);
		}
		ImGui::PopID();
	}

	void SceneHierachyPanel::DrawComponent(Entity& entity) {
		auto& Tag = *entity.GetComponent<TagComponent>();
		char buffer[256];
		memset(buffer,0,sizeof(buffer));
		strcpy_s(buffer,sizeof(buffer),Tag.GetName().c_str());
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
				entity.AddComponent<MeshComponent>();
				ImGui::CloseCurrentPopup();
			}

			if(ImGui::MenuItem("Sprite Renderer")){
				entity.AddComponent<SpriteComponent>();
				ImGui::CloseCurrentPopup();
			}
			 
			if (ImGui::MenuItem("Native Script Component")) {
				entity.AddComponent<NativeScriptComponent>();
				ImGui::CloseCurrentPopup();
			}

			if (ImGui::MenuItem("Light Componet")) {
				entity.AddComponent<LightComponent>();
				ImGui::CloseCurrentPopup();
			}

			if (ImGui::MenuItem("Movement Script")) {
				entity.AddComponent<NativeScriptComponent>()->Bind<MovementScript>();
				ImGui::CloseCurrentPopup();
			}
			if (ImGui::MenuItem("Camera Component")) {
				entity.AddComponent<CameraComponent>();
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}
		
		uint32_t IndexValue= 0;
		for(Component* Comp: *m_CurrentWorld->Registry.GetEntities().at(entity.GetID())){
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
				DrawComponents<MeshComponent>({"Mesh: " + Meshes->GetName()},entity,Meshes,IndexValue,[](MeshComponent& component) {
					char buffer[256];
					memset(buffer,0,sizeof(buffer));
					strcpy_s(buffer,sizeof(buffer),component.GetName().c_str());
					if (ImGui::InputText("##Name",buffer,sizeof(buffer))) {
						component.SetName(buffer);
					}
					DrawVectorControl("Local Location",component.MeshLocalTransform.Location,0.0);
					DrawVectorControl("Local Rotation",component.MeshLocalTransform.Rotation,0.0);
					DrawVectorControl("Local Scale",component.MeshLocalTransform.Scale,0.0);
					ExternalAPI::ImGUIAPI::TextBar("Mesh",AssetManager::HasID(component.AssetID) ? AssetManager::GetAsset<Asset>(component.AssetID)->GetAssetName():"null");
					if (ImGui::BeginPopupContextItem("RemoveMesh")) {
						ImGui::EndPopup();
					}
					if (ImGui::BeginPopup("RemoveMesh")) {
						if (ImGui::MenuItem("Remove Mesh")) {
							component.AssetID = 0;
						}

						ImGui::EndPopup();
					}
					if (ImGui::BeginDragDropTarget()) {
						if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(MeshAsset::GetAssetTypeStaticName().c_str())) {
							uint32_t Data = *(const uint32_t*)payload->Data;
							component.AssetID = Data;
						}
						ImGui::EndDragDropTarget();
					}

					ExternalAPI::ImGUIAPI::TextBar("Material",component.HasMaterial()? AssetManager::GetAsset<Asset>(component.GetMaterialPointerID())->GetAssetName() : "null");
					if (ImGui::BeginPopupContextItem("RemoveMaterial")) {
						ImGui::EndPopup();
					}
					if (ImGui::BeginPopup("RemoveMaterial")) {
						if (ImGui::MenuItem("Remove Material")) {
							component.m_MeshMaterialID = 0;
						}

						ImGui::EndPopup();
					}
					if (ImGui::BeginDragDropTarget()) {
						if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(MaterialAsset::GetAssetTypeStaticName().c_str())) {
							uint32_t Data = *(const uint32_t*)payload->Data;
							component.m_MeshMaterialID = Data;
							PF_ENGINE_INFO("DROPPED %i",Data);
						}
						ImGui::EndDragDropTarget();
					}
				});
				IndexValue += 1;
				continue; 
			}
			SpriteComponent* Sprite = dynamic_cast<SpriteComponent*>(Comp);
			if(Sprite != nullptr){
				DrawComponents<SpriteComponent>({"Sprite: "+Sprite->GetName()},entity,Sprite,IndexValue,[](SpriteComponent& component){
					char buffer[256];
					memset(buffer,0,sizeof(buffer));
					strcpy_s(buffer,sizeof(buffer),component.GetName().c_str());
					if (ImGui::InputText("##N",buffer,sizeof(buffer))) {
						component.SetName(buffer);
					}
					DrawVectorControl("Location",component.SpriteTransfrom.Location,0.0,125);
					DrawVectorControl("Rotation",component.SpriteTransfrom.Rotation,0.0,125);
					DrawVectorControl("Scale",component.SpriteTransfrom.Scale,0.0,125);
					if(component.GetTexture() != nullptr){
							ImGui::Image((ImTextureID)component.GetTexture()->GetID(),{30,30});
					}else{
						ImGui::Image((ImTextureID)InstancedRenderer3D::m_WhiteTexture->GetID(),{30,30});
					}
					if(ImGui::BeginPopupContextItem("RemoveTexture")){
						ImGui::EndPopup();
					}
					if (ImGui::BeginPopup("RemoveTexture")) {
						if (ImGui::MenuItem("Remove Texture")) {
							component.AssetID = 0;
						}
						ImGui::EndPopup();
					}
					
					if (ImGui::BeginDragDropTarget()) {
						if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(Texture2DAsset::GetAssetTypeStaticName().c_str())) {
							uint32_t Data = *(const uint32_t*)payload->Data;
							component.AssetID =Data;
						}
						ImGui::EndDragDropTarget();
					}
					
					ImGui::ColorEdit4("##Colour",glm::value_ptr( component.Colour));
				});
				IndexValue += 1;
				continue;
			}

			NativeScriptComponent* Script = dynamic_cast<NativeScriptComponent*>(Comp);
			if(Script !=nullptr){
				DrawComponents<NativeScriptComponent>("Native Script: ",entity,Script,IndexValue,[](NativeScriptComponent& NativeScriptComp){
					ExternalAPI::ImGUIAPI::TextBar("Sript",NativeScriptComp.GetName());
					if(ImGui::IsItemClicked()){
						ImGui::OpenPopup("Scripts");
						
					}
					if (ImGui::BeginPopup("Scripts")) {
						/*
						if (ImGui::MenuItem("Movement")) {
							/*
							NativeScriptComp.Bind<MovementScript>();
							NativeScriptComp.Name="MovementScript";
							
							InitilizeScript(NativeScriptComp,MovementScript);
						}
					*/
						/*
						for(auto&it: ScriptDetail::GetScriptRegisry()){
							const std::string& temp = it.second.first;
							if(ImGui::MenuItem(temp.c_str())){								
								InitilizeScript(NativeScriptComp,Player);
							}
						}
						*/
						ImGui::EndPopup();
					}
					
					if(NativeScriptComp.GetName() =="MovementScript"){
						if(NativeScriptComp.Instance!=nullptr){
							/*
							MovementScript* temp =dynamic_cast<MovementScript*>(NativeScriptComp.Instance);
							*/
							//ImGui::DragFloat("Speed",&temp->Speed);
						}
					} 
				});
				IndexValue += 1;
				continue;
			}
			LightComponent* Light = dynamic_cast<LightComponent*>(Comp);
			if (Light != nullptr) {
				DrawComponents<LightComponent>("Light Component: ",entity,Light,IndexValue,[](LightComponent& LightComp) {
					char buffer[256];
					memset(buffer,0,sizeof(buffer));
					strcpy_s(buffer,sizeof(buffer),LightComp.GetName().c_str());
					if (ImGui::InputText("##N",buffer,sizeof(buffer))) {
						LightComp.SetName(buffer);
					}

					if(LightComp.m_LightType == LightComp.Direction){
						DrawVectorControl("Direction",LightComp.m_Direction);
						ImGui::NewLine();
						ImGui::ColorEdit3("Ambient",glm::value_ptr(LightComp.m_Ambient));
						ImGui::ColorEdit3("Diffuse",glm::value_ptr(LightComp.m_Diffuse));
						ImGui::ColorEdit3("Specular",glm::value_ptr(LightComp.m_Specular));
					}
					else if(LightComp.m_LightType == LightComp.Point){
						DrawVectorControl("Position",LightComp.m_Position);
						ImGui::NewLine();

						ImGui::Text("Constant");
						ImGui::SameLine();
						ImGui::DragFloat("##Constant",&LightComp.m_Constant,0.001);
						ImGui::DragFloat("Linear",&LightComp.m_Linear,0.001);
						ImGui::DragFloat("Quadratic",&LightComp.m_Quadratic,0.001);


						ImGui::ColorEdit3("Ambient",glm::value_ptr(LightComp.m_Ambient));
						ImGui::ColorEdit3("Diffuse",glm::value_ptr(LightComp.m_Diffuse));
						ImGui::ColorEdit3("Specular",glm::value_ptr(LightComp.m_Specular));
						
					}else if(LightComp.m_LightType == LightComp.Spot){
						DrawVectorControl("Position",LightComp.m_Position);
						DrawVectorControl("Direction",LightComp.m_Direction);
						ImGui::NewLine();
						ImGui::DragFloat("Constant",&LightComp.m_Constant,0.001);
						ImGui::DragFloat("Linear",&LightComp.m_Linear,0.001);
						ImGui::DragFloat("Quadratic",&LightComp.m_Quadratic,0.001);
						ImGui::DragFloat("CutOff",&LightComp.m_CutOff,0.001);
						ImGui::DragFloat("Outer-Cutoff",&LightComp.m_OuterCutOff,0.001);

						ImGui::ColorEdit3("Ambient",glm::value_ptr(LightComp.m_Ambient));
						ImGui::ColorEdit3("Diffuse",glm::value_ptr(LightComp.m_Diffuse));
						ImGui::ColorEdit3("Specular",glm::value_ptr(LightComp.m_Specular));
					}		
					int elementCount = 2;
					const char* elementNames[] = {"Direction","Point","Spot"};
					const char* elementName =(LightComp.m_LightType >=0 && LightComp.m_LightType <elementCount+1) ?elementNames[LightComp.m_LightType]:"Unknown";
					ImGui::SliderInt("Type",&LightComp.m_LightType,0,elementCount,elementName);
				});
				IndexValue += 1;
				continue;
			}
			CameraComponent* cameraComp = dynamic_cast<CameraComponent*>(Comp);

			if(Comp !=nullptr){
				DrawComponents<CameraComponent>("Camera Component: ",entity,cameraComp,IndexValue,[](CameraComponent& CameraComp){
					char buffer[256];
					memset(buffer,0,sizeof(buffer));
					strcpy_s(buffer,sizeof(buffer),CameraComp.GetName().c_str());
					if (ImGui::InputText("##N",buffer,sizeof(buffer))) {
						CameraComp.SetName(buffer);
					}

					ImGui::NewLine();
					
					ImGui::SliderFloat("Field ov fiew",&CameraComp.m_Camera.m_FovDeg,0,90);
					ImGui::DragFloat("Near plane",&CameraComp.m_Camera.m_NearPlane,0,100000);
					ImGui::DragFloat("Far plane",&CameraComp.m_Camera.m_FarPlane,0,100000);

					ExternalAPI::ImGUIAPI::CheckBox("set Automatic resize",&CameraComp.m_Camera.m_AutoSetDimension);
					if(CameraComp.m_Camera.m_AutoSetDimension ==false){
						int tempWidth =(int)CameraComp.m_Camera.m_Width;
						int tempHeight =(int)CameraComp.m_Camera.m_Height;
						ImGui::DragInt("view width",&tempWidth,1,0);
						ImGui::DragInt("view height",&tempHeight,1,0);
						CameraComp.m_Camera.m_Width = tempWidth;
						CameraComp.m_Camera.m_Height= tempHeight;
					}
				});
			}
		} 
	}



	void SceneHierachyPanel::DrawVectorControl(const std::string& UniqeLabel,Vector& Vec,float ResetValue,float columnWidth,float Speed) {
		ImGuiIO& io = ImGui::GetIO();
		auto boldFont = io.Fonts->Fonts[0];
		ImGui::PushID(UniqeLabel.c_str());// this id is for everything here so imgui does not assign something to the value that we have here
		ImGui::Columns(2); // distance between label and edits
		ImGui::SetColumnWidth(0,columnWidth);
		ImGui::Text(UniqeLabel.c_str());
		ImGui::NextColumn();

		ImGui::PushMultiItemsWidths(3,ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing,ImVec2{0,0});
		ImGui::GetStyle().FrameRounding = 0; 

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
		ImGui::DragFloat("##x",&Vec.X,Speed,0,0,"%.3f"); // does not show ## as label
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
		ImGui::DragFloat("##Y",&Vec.Y,Speed,0,0,"%.3f"); // does not show ## as label
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
		ImGui::DragFloat("##Z",&Vec.Z,Speed,0,0,"%.3f"); // does not show ## as label
		ImGui::PopItemWidth();
		ImGui::PopStyleColor(3);

		ImGui::GetStyle().FrameRounding = 6;

		ImGui::PopStyleVar(1);
		ImGui::Columns(1);
		ImGui::PopID();
	}
	void SceneHierachyPanel::DrawVector4Control(const std::string& UniqeLabel,glm::vec4& Vec,float ResetValue,float columnWidth) {
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
			Vec.r = ResetValue;
		}
		ImGui::PopFont();
		ImGui::SameLine();
		ImGui::DragFloat("##x",&Vec.r,0.1f,0,0,"%.3f"); // does not show ## as label
		ImGui::PopItemWidth();
		ImGui::SameLine();
		ImGui::PopStyleColor(3);

		ImGui::PushStyleColor(ImGuiCol_Button,ImVec4{0.0f,.5f,0.0f,1.0f});
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered,ImVec4{1.0f,0.5f,0.0f,1.0f});
		ImGui::PushStyleColor(ImGuiCol_ButtonActive,ImVec4{0.0f,.5f,0.0f,1.0f});

		ImGui::PushFont(boldFont);
		if (ImGui::Button("Y",buttonSize)) {
			Vec.y = ResetValue;
		}
		ImGui::PopFont();

		ImGui::SameLine();
		ImGui::DragFloat("##Y",&Vec.y,0.1f,0,0,"%.3f"); // does not show ## as label
		ImGui::PopItemWidth();
		ImGui::SameLine();
		ImGui::PopStyleColor(3);

		ImGui::PushStyleColor(ImGuiCol_Button,ImVec4{0.0f,0.0f,1.0f,1.0f});
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered,ImVec4{1.0f,0.5f,0.0f,1.0f});
		ImGui::PushStyleColor(ImGuiCol_ButtonActive,ImVec4{0.0f,0.0f,1.0f,1.0f});
		ImGui::PushFont(boldFont);
		if (ImGui::Button("Z",buttonSize)) {
			Vec.z = ResetValue;
		}
		ImGui::PopFont();

		ImGui::SameLine();
		ImGui::DragFloat("##Z",&Vec.z,0.1f,0,0,"%.3f"); // does not show ## as label
		ImGui::PopItemWidth();
		ImGui::PopStyleColor(3);

		ImGui::PushStyleColor(ImGuiCol_Button,ImVec4{1.0f,1.0f,1.0f,1.0f});
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered,ImVec4{1.0f,1.0f,1.0f,1.0f});
		ImGui::PushStyleColor(ImGuiCol_ButtonActive,ImVec4{1.0f,1.0f,1.0f,1.0f});

		ImGui::PushFont(boldFont);
		if (ImGui::Button("A",buttonSize)) {
			Vec.a = ResetValue;
		}
		ImGui::PopFont();

		ImGui::SameLine();
		ImGui::DragFloat("##A",&Vec.a,0.1f,0,0,"%.3f"); // does not show ## as label
		ImGui::PopItemWidth();
		ImGui::PopStyleColor(3);
		ImGui::PopStyleVar();
		ImGui::Columns(1);
		ImGui::PopID();
	}
}