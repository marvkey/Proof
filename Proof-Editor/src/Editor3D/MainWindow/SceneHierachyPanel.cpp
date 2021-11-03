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
#include "Proof/Scene/Script.h"
#include "Proof/Scene/ComponentUnOptimized.h"
#include "Proof/Input/KeyCodes.h"
#include "ImGui/imgui_internal.h"

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
				if(entity.GetComponent<SubEntityComponet>()->HasEntityOwner()==false)
					DrawEntityNode(entity);
			}
			
			if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered()&& ImGui::IsAnyItemHovered()==false) {
				m_SelectedEntity = {};
			}
			if (ImGui::BeginPopupContextWindow(0,1,false)){ // right click adn open a new entitiy
				if (ImGui::MenuItem("Create Entity"))
					m_SelectedEntity=m_CurrentWorld->CreateEntity("Empty Entity");
				ImGui::EndPopup();
			}
		}
		ImGui::End();

		ImGui::Begin("Properties");
		{
			if (m_SelectedEntity){
				DrawComponent(m_SelectedEntity);
			}
		}
		ImGui::End();
	}
	void SceneHierachyPanel::DrawEntityNode(Entity& entity) {
		if(m_CurrentWorld->Registry.HasEntity(entity.GetID())==false)
			return;
		auto& tc = entity.GetComponent<TagComponent>()->GetName();
		ImGuiTreeNodeFlags flags = ((m_SelectedEntity == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		
		if(entity.GetComponent<SubEntityComponet>()->m_AllSubEntity.size()==0){
			flags |=ImGuiTreeNodeFlags_Leaf;//makes the tree not use an arrow
		}
		
		flags |= ImGuiTreeNodeFlags_SpanFullWidth;
		bool opened = ImGui::TreeNodeEx((void*)(EntityID)(uint32_t)entity.GetID(),flags,tc.c_str());
		if (ImGui::BeginDragDropTarget()) {
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("EntityNewOwner")) {
				Entity Data = *(const Entity*)payload->Data;
				if(Data.GetComponent<SubEntityComponet>()->HasEntityOwner())
					Data.GetComponent<SubEntityComponet>()->SwapEntityOwner(entity);
				else
					entity.GetComponent<SubEntityComponet>()->AddSubEntity(Data);
			}
			ImGui::EndDragDropTarget();
		}
		if (ImGui::BeginDragDropSource()) {
			ImGui::SetDragDropPayload("EntityNewOwner",&m_SelectedEntity,sizeof(Entity));

			ImGui::TreeNodeEx((void*)&(m_SelectedEntity),ImGuiTreeNodeFlags_SpanFullWidth,tc.c_str());
			ImGui::EndDragDropSource();
		}
		if (ImGui::IsItemClicked() || ImGui::IsItemClicked(1)) {
			m_SelectedEntity = entity;
		}
		if(ImGui::BeginPopupContextItem()){
			if(m_SelectedEntity.GetComponent<SubEntityComponet>()->HasEntityOwner()){
				if(ImGui::MenuItem("Single entity")){
					m_SelectedEntity.GetComponent<SubEntityComponet>()->GetEntityOwner().GetComponent<SubEntityComponet>()->RemoveSubEnity(m_SelectedEntity);
					m_SelectedEntity.GetComponent<SubEntityComponet>()->m_EntitySubOwner={};
					ImGui::CloseCurrentPopup();
				}
			}
			if(ImGui::MenuItem("Create Child entity")){
				Entity childEntity = m_CurrentWorld->CreateEntity("Empty Child Entity");
				childEntity.GetComponent<SubEntityComponet>()->m_EntitySubOwner = m_SelectedEntity;
				m_SelectedEntity.GetComponent<SubEntityComponet>()->AddSubEntity(childEntity);
				ImGui::CloseCurrentPopup();
			}
			if(ImGui::MenuItem("Delete")){
				m_CurrentWorld->DeleteEntity(m_SelectedEntity);

				m_SelectedEntity ={};
				ImGui::CloseCurrentPopup();
				if(opened){
					ImGui::EndPopup();
					ImGui::TreePop();
					return;
				}
			}
			ImGui::EndPopup();
		}
		if(ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0) && m_SelectedEntity){
			m_CurrentWorld->m_EditorCamera.m_Positon = m_SelectedEntity.GetComponent<TransformComponent>()->Location;
		}
		if(m_SelectedEntity && ImGui::IsKeyPressed((int)KeyBoardKey::F)){
			m_CurrentWorld->m_EditorCamera.m_Positon = m_SelectedEntity.GetComponent<TransformComponent>()->Location;
		}
		if (opened) {
			
			if (entity.GetComponent<SubEntityComponet>()->m_AllSubEntity.size() > 0) {
				for (Entity& i : entity.GetComponent<SubEntityComponet>()->m_AllSubEntity) {
					DrawEntityNode(i);
				}
			}
			
			ImGui::TreePop();
		}
	}

	template<typename T,typename UIFunction>
	void SceneHierachyPanel::DrawComponents(const std::string& name,Entity& entity,T* Comp,uint32_t IndexValue,UIFunction Uifunction,const std::string& toolTip) {
		ImGui::PushID(&(*Comp));
		const ImGuiTreeNodeFlags treeNodeFlags =ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;
		T& component = *Comp;
		ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();

		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding,ImVec2{1.5,1.5});
		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImGui::Separator();
		bool open = false;
		open = ImGui::TreeNodeEx((void*)&(*Comp),treeNodeFlags,name.c_str());
		if(ImGui::IsItemHovered()&& toolTip.empty()==false){
			ImGui::BeginTooltip();
			{
				ImGui::SetTooltip(toolTip.c_str());
			}
			ImGui::EndTooltip();
		}
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
		//ImGui::Text(std::to_string(std<uint32_t>()((uint32_t)entity.GetID())).c_str());
		//ImGui::SameLine();
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
				DrawComponents<TagComponent>("Tag",entity,Tag,IndexValue,[](TagComponent& component) {
					uint32_t iterate = 0;
					if (ImGui::Button("Add tag")) {
						component.AddTag(" ");
					}
					for (std::string& tag : component.m_Tags) {
						char buffer[256];
						memset(buffer,0,sizeof(buffer));
						strcpy_s(buffer,sizeof(buffer),tag.c_str());
						const std::string name = std::to_string(iterate);
						ImGui::Text(name.c_str());
						ImGui::SameLine();
						ImGui::PushID(name.c_str());
						if (ImGui::InputTextWithHint("##temp","tag",buffer,sizeof(buffer))) {
							tag = buffer;
						}
						ImGui::SameLine();
						if (ImGui::Button("-",{20,20})) {
							component.RemoveTag(iterate);
						}
						ImGui::PopID();

						iterate++;

					}
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

			SubEntityComponet* subComponet = dynamic_cast<SubEntityComponet*>(Comp);
			if(subComponet!=nullptr){
				DrawComponents<SubEntityComponet>("SubEntityComponet",entity,subComponet,IndexValue,[](SubEntityComponet& component) {

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
					ExternalAPI::ImGUIAPI::TextBar("Mesh",component.GetAsset()!=nullptr ? component.GetAsset()->GetName():"null");
					if (ImGui::BeginPopupContextItem("RemoveMesh")) {
						ImGui::EndPopup();
					}
					if (ImGui::BeginPopup("RemoveMesh")) {
						if (ImGui::MenuItem("Remove Mesh")) {
							component.m_MeshAssetPointerID = 0;
						}

						ImGui::EndPopup();
					}
					if (ImGui::BeginDragDropTarget()) {
						if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(MeshAsset::GetAssetType().c_str())) {
							AssetID Data = *(const AssetID*)payload->Data;
							component.m_MeshAssetPointerID = Data;
						}
						ImGui::EndDragDropTarget();
					}

					ExternalAPI::ImGUIAPI::TextBar("Material",component.HasMaterial()? AssetManager::GetAsset<Asset>(component.GetMaterialPointerID())->GetName(): "null");
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
						if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(MaterialAsset::GetAssetType().c_str())) {
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
							component.m_TextureAssetPointerID = 0;
						}
						ImGui::EndPopup();
					}
					
					if (ImGui::BeginDragDropTarget()) {
						if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(Texture2DAsset::GetAssetType().c_str())) {
							AssetID Data = *(const AssetID*)payload->Data;
							component.m_TextureAssetPointerID =Data;
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
						ImGui::ColorEdit3("Ambient",glm::value_ptr(LightComp.m_Ambient));
						ImGui::ColorEdit3("Diffuse",glm::value_ptr(LightComp.m_Diffuse));
						ImGui::ColorEdit3("Specular",glm::value_ptr(LightComp.m_Specular));
					}
					else if(LightComp.m_LightType == LightComp.Point){
						ImGui::Text("Constant");
						ImGui::SameLine();
						ImGui::DragFloat("##Constant",&LightComp.m_Constant,0.001);
						ImGui::DragFloat("Linear",&LightComp.m_Linear,0.001);
						ImGui::DragFloat("Quadratic",&LightComp.m_Quadratic,0.001);


						ImGui::ColorEdit3("Ambient",glm::value_ptr(LightComp.m_Ambient));
						ImGui::ColorEdit3("Diffuse",glm::value_ptr(LightComp.m_Diffuse));
						ImGui::ColorEdit3("Specular",glm::value_ptr(LightComp.m_Specular));
						
					}else if(LightComp.m_LightType == LightComp.Spot){
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
					
					ImGui::SliderFloat("Field ov fiew",&CameraComp.m_FovDeg,0,360);
					ImGui::SliderFloat("Near plane",&CameraComp.m_NearPlane,-1,1);
					ImGui::SliderFloat("Far plane",&CameraComp.m_FarPlane,0,10000);

					ExternalAPI::ImGUIAPI::CheckBox("set Automatic resize",&CameraComp.m_AutoSetDimension);
					if(CameraComp.m_AutoSetDimension ==false){
						int tempWidth =(int)CameraComp.m_Width;
						int tempHeight =(int)CameraComp.m_Height;
						ImGui::DragInt("view width",&tempWidth,1,0);
						ImGui::DragInt("view height",&tempHeight,1,0);
						CameraComp.m_Width = tempWidth;
						CameraComp.m_Height= tempHeight;
					}
				},"if nothing visible set roation of z axis to 1");
				IndexValue += 1;
				continue;
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