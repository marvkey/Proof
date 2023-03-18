#include "SceneHierachyPanel.h"
#include "Proof/ImGui/ImGuiLayer.h"
#include "Proof/Scene/Entity.h"
#include "Proof/Scene/Component.h"
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include "Proof/Scene/EntitiyComponentSystem/ECS.h"
#include "Proof/Renderer/3DRenderer/Renderer3D.h" // TEMPORARY
#include "Proofprch.h"
#include "Proof/Asset/Asset.h"
#include "ContentBrowserPanel.h"
#include <vector>
#include "../ImGUIAPI.h"
#include "Proof/Scene/ExampleSccripts.h"
#include "Proof/Scene/Script.h"
#include "Proof/Input/KeyCodes.h"
#include "ImGui/imgui_internal.h"
#include "Editor3D/Editor3D.h"
#include "Proof/Scene/Component.h"
#include "Proof/Asset/AssetManager.h"
#include <string.h>
#include<magic_enum.hpp>
#include "Proof/Resources/EnumReflection.h"
#include "Proof/Scripting/ScriptEngine.h"
#include "Proof/Imgui/UI.h"
#include "Proof/Renderer/Renderer.h"
namespace Proof
{
	#define SET_FEILD_DEFAULT(FieldType, Type)           \
			case ScriptFieldType::FieldType:          \
				scriptInstance.SetValue<float>(entityClass->GetFieldDefaultValue<float>(fieldName));  \
				break
	
	#define SET_FIELD_NUMERICAL_VALUE(FieldType, Type, name, ImguiDataType)           \
			case ScriptFieldType::FieldType:          \
			{												\
				Type data = scriptField.GetValue<Type>();	\
				Type* pointerData = &data;\
				if(ImGui::DragScalar(name.c_str(), ImguiDataType, (void*)pointerData, 1.0f)){\
					scriptField.SetValue<Type>(data); \
				}								\
				break;							\
			}
	#define SET_FIELD_NUMERICAL_VALUE_RUNTIME(FieldType, Type, name, ImguiDataType)           \
			case ScriptFieldType::FieldType:          \
			{												\
				Type data = instance->GetFieldValue<Type>(name);	\
				Type* pointerData = &data;\
				if(ImGui::DragScalar(name.c_str(), ImguiDataType, (void*)pointerData, 1.0f)){\
					instance->SetFieldValue<Type>(name,data); \
				}								\
				break;							\
			}	
	void SceneHierachyPanel::ImGuiRender(class FrameTime deltaTime) {

		if (m_ShowWindow == false)
			return;
		PF_PROFILE_FUNC();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0,0 });
		if (ImGui::Begin("Herieachy", &m_ShowWindow));
		{
			/*
			auto &a =m_CurrentWorld->m_Registry.view<>();
			for (auto& g : a) {

			}
			*/
			ImGui::PushStyleColor(ImGuiCol_ChildBg, { 0,0,0,1 });
			ImGui::BeginChild("Child Herieachy", { ImGui::GetContentRegionAvail().x,ImGui::GetWindowHeight() / 2 });
			if (ImGui::BeginPopupContextWindow(0)) { // right click adn open a new entitiy
				CreateEntityMenu();
				ImGui::EndPopup();
			}
			{
				m_WindowHoveredorFocus = ImGui::IsWindowHovered() || ImGui::IsWindowFocused();
				// when copying ot temporayr since copies backwards have to do this
				if (m_CurrentWorld->m_CurrentState == WorldState::Edit) {
					for (uint64_t i = 0; i < m_CurrentWorld->m_Registry.size(); i++) {
						Entity entity = { m_CurrentWorld->m_Registry.entities[i],m_CurrentWorld };
						if (entity.HasOwner() == false)
							DrawEntityNode(entity);
					}
				}
				else {
					for (uint64_t i = 0; i < m_CurrentWorld->m_Registry.size(); i++)
					{
						Entity entity = { m_CurrentWorld->m_Registry.entities[i],m_CurrentWorld };
						if (entity.HasOwner() == false)
							DrawEntityNode(entity);
					}
					//for (uint64_t i = m_CurrentWorld->m_Registry.size()-1; i >=0; i--)
					//{
					//	Entity entity = { m_CurrentWorld->m_Registry.entities[i],m_CurrentWorld };
					//	IDComponent* comp = entity.GetComponent<IDComponent>();
					//	uint64_t iereewrsdfasfd = comp->GetID().Get();
					//	if (entity.HasOwner() == false)
					//		DrawEntityNode(entity);
					//}
					//m_CurrentWorld->ForEachEntityBackwards([&](Entity entity) {
					//	if (entity.HasOwner() == false)
					//		DrawEntityNode(entity);
					//});
				}

				if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered() && ImGui::IsAnyItemHovered() == false) {
					m_SelectedEntity = {};
				}
			
			}
			ImGui::EndChild();
			if (ImGui::BeginDragDropTarget()) {
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("EntityNewOwner")) {

					Entity Data = *(const Entity*)payload->Data;
					Data.SetOwner(Entity{});
				}
				ImGui::EndDragDropTarget();
			}
			ImGui::PopStyleColor();
			ImGui::BeginChild("Properties", ImGui::GetContentRegionAvail());
			{
				{
					if (m_SelectedEntity) {
						DrawComponent(m_SelectedEntity);
					}
				}
				{

				}
			}
			ImGui::EndChild();
		}

		ImGui::End();
		ImGui::PopStyleVar();

	}

	void SceneHierachyPanel::OnKeyClicked(KeyClickedEvent& e) {
	}

	bool  SceneHierachyPanel::CreateEntityMenu(Entity owner) {
		uint64_t selectedPreviousEntityID = m_SelectedEntity.GetEntityID();// we are doing this inncase we created a child entity
		Entity newEntity;
		if (ImGui::MenuItem("Entity"))
			newEntity = m_CurrentWorld->CreateEntity();
		if (ImGui::BeginMenu("Light")) {
			if (ImGui::MenuItem("Point ")) {
				newEntity = m_CurrentWorld->CreateEntity("Point Light");
				newEntity.AddComponent<PointLightComponent>();
			}
			if (ImGui::MenuItem("Spot")) {
				newEntity = m_CurrentWorld->CreateEntity("Spot Light");
				newEntity.AddComponent<SpotLightComponent>();
			}
			if (ImGui::MenuItem("Directional")) {
				newEntity = m_CurrentWorld->CreateEntity("Directional Light");
				newEntity.AddComponent<DirectionalLightComponent>();
			}
			ImGui::EndMenu();
		}
		if (ImGui::MenuItem("Mesh")) {
			newEntity = m_CurrentWorld->CreateEntity("Mesh");
			newEntity.AddComponent<MeshComponent>();
		}
		if (ImGui::MenuItem("Camera")) {
			newEntity = m_CurrentWorld->CreateEntity("Camera");
			newEntity.AddComponent<CameraComponent>();
		}
		if (owner && newEntity.GetEntityID() != 0) {
			newEntity.SetOwner(owner);
		}
		if (newEntity.GetEntityID() != 0)
			return true;
		return false;
	}
	void SceneHierachyPanel::DrawEntityNode(Entity entity) {
		auto& tc = entity.GetComponent<TagComponent>()->Tag;
		ImGui::PushID(entity.GetEntityID());
		ImGuiTreeNodeFlags flags = ((m_SelectedEntity == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;

		if (entity.GetComponent<ChildComponent>()->HasChildren() == false) {
			flags |= ImGuiTreeNodeFlags_Leaf;//makes the tree not use an arrow
		}

		flags |= ImGuiTreeNodeFlags_SpanFullWidth;
		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity.GetEntityID(), flags, tc.c_str());
		if (ImGui::BeginDragDropTarget()) {
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("SceneEntity")) {

				Entity Data = *(const Entity*)payload->Data;
				entity.AddChild(Data);
			}
			ImGui::EndDragDropTarget();
		}
		if (ImGui::BeginDragDropSource()) {
			ImGui::SetDragDropPayload("SceneEntity", &entity, sizeof(Entity));

			ImGui::TreeNodeEx((void*)&(entity), ImGuiTreeNodeFlags_SpanFullWidth, tc.c_str());
			ImGui::EndDragDropSource();
		}
		if ( ImGui::IsItemClicked() && ImGui::IsKeyDown((ImGuiKey)KeyBoardKey::E) ==false) {
			m_SelectedEntity = entity;
		}
		//if (ImGui::BeginPopupContextItem()) {
		if(ImGui::BeginPopupContextItem("Entity Settings")) {
			ImGui::EndPopup();
		}
		if (ImGui::BeginPopup("Entity Settings")) {
			if (ImGui::BeginMenu("Child Entity")) {
				bool temp = CreateEntityMenu(m_SelectedEntity);
				// not setting to opne because 
				// if it is already opened what if we
				// do not actually create and entity we are closing
				// for no reason

				ImGui::EndMenu();
			}
			if (ImGui::MenuItem("Delete")) {
				m_CurrentWorld->DeleteEntity(m_SelectedEntity, true);

				m_SelectedEntity = {};

				if (opened) {
					ImGui::EndPopup();
					ImGui::TreePop();
					ImGui::PopID();
					return;
				}
			}
			ImGui::EndPopup();
		}
		if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0) && m_SelectedEntity) {
			//m_CurrentWorld->m_EditorCamera.m_Positon = m_SelectedEntity.GetComponent<TransformComponent>()->Location;
		}
		if (m_SelectedEntity && ImGui::IsKeyPressed((ImGuiKey)KeyBoardKey::F)) {
			//m_CurrentWorld->m_EditorCamera.m_Positon = m_SelectedEntity.GetComponent<TransformComponent>()->Location;
		}

		if (opened) {
			for (const UUID& I : entity.GetComponent<ChildComponent>()->m_Children) {
				DrawEntityNode(Entity{ I,m_CurrentWorld });
			}
			ImGui::TreePop();
		}
		ImGui::PopID();
	}

	template<typename T, typename UIFunction>
	void SceneHierachyPanel::DrawComponents(const std::string& name, Entity& entity, UIFunction Uifunction, const std::string& toolTip) {
		if (entity.HasComponent<T>() == false)
			return;

		ImGui::PushID((void*)typeid(T).hash_code());
		const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;
		T& component = *entity.GetComponent<T>();
		ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();

		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 1.5,1.5 });
		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImGui::Separator();
		bool open = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), treeNodeFlags, name.c_str());
		if (ImGui::IsItemHovered() && toolTip.empty() == false) {
			ImGui::BeginTooltip();
			{
				ImGui::SetTooltip(toolTip.c_str());
			}
			ImGui::EndTooltip();
		}
		ImGui::PopStyleVar();
		ImGui::SameLine(contentRegionAvailable.x - lineHeight * 0.5f);
		if (ImGui::Button("+", ImVec2{ lineHeight + 3,lineHeight })) {
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
			ImGui::TreePop();
		}

		if (removeComponent) {
			entity.RemoveComponent<T>();
		}
		ImGui::PopID();
	}
	void SceneHierachyPanel::DrawComponent(Entity& entity) {
		auto& Tag = *entity.GetComponent<TagComponent>();
		char buffer[256];
		memset(buffer, 0, sizeof(buffer));
		strcpy_s(buffer, sizeof(buffer), Tag.Tag.c_str());
		if (ImGui::InputText("##Tag", buffer, sizeof(buffer))) {
			Tag.Tag = buffer;
		}
		ImGui::SameLine();
		ImGui::PushItemWidth(-1);
		//ImGui::Text(std::to_string(std<uint32_t>()((uint32_t)entity.GetID())).c_str());
		//ImGui::SameLine();
		if (ImGui::Button("Add Component"))
			ImGui::OpenPopup("AddComponent");
		ImGui::PopItemWidth();
		if (ImGui::BeginPopup("AddComponent")) {

			AddComponentGui<MeshComponent>(entity, "Mesh");
			AddComponentGui<SpriteComponent>(entity, "Sprite");
			AddComponentGui<NativeScriptComponent>(entity, "Native Script");
			AddComponentGui<DirectionalLightComponent>(entity, "Directional Light");
			AddComponentGui<PointLightComponent>(entity, "Point Light");
			AddComponentGui<SpotLightComponent>(entity, "Spot Light");
			AddComponentGui<CameraComponent>(entity, "Camera");

			AddComponentGui<CubeColliderComponent>(entity, "Cube Collider");
			AddComponentGui<SphereColliderComponent>(entity, "Sphere Collider");
			AddComponentGui<CapsuleColliderComponent>(entity, "Capsule Collider");
			AddComponentGui<MeshColliderComponent>(entity, "Mesh Collider");
			AddComponentGui<RigidBodyComponent>(entity, "Rigid Body");

			AddComponentGui<ScriptComponent>(entity, "Scripts");
			ImGui::EndPopup();
		}
		DrawComponents<TagComponent>("Tag", entity, [](TagComponent& subTag) {
			uint32_t iterate = 0;
			if (ImGui::Button("Add tag")) {
				subTag.AddTag("");
			}
			for (std::string& tag : subTag.m_Tags) {
				char buffer[256];
				memset(buffer, 0, sizeof(buffer));
				strcpy_s(buffer, sizeof(buffer), tag.c_str());
				const std::string name = std::to_string(iterate);
				ImGui::Text(name.c_str());
				ImGui::SameLine();
				ImGui::PushID(name.c_str());
				if (ImGui::InputTextWithHint("##temp", "tag", buffer, sizeof(buffer))) {
					tag = buffer;
				}
				ImGui::SameLine();
				if (ImGui::Button("-", { 20,20 })) {
					subTag.RemoveTag(iterate);
				}
				ImGui::PopID();

				iterate++;
			}
		});
		DrawComponents<TransformComponent>("Transform", entity, [](auto& transformComp) {
			DrawVectorControl("Location", transformComp.Location);
			DrawVectorControl("Rotation", transformComp.Rotation);
			DrawVectorControl("Scale", transformComp.Scale, 1.0f);
		});
		DrawComponents<MeshComponent>("Mesh", entity, [](MeshComponent& meshComp) {
			if (AssetManager::HasAsset(meshComp.GetMesh())) {
				auto assetInfo= AssetManager::GetAssetInfo(meshComp.GetMesh());
				ExternalAPI::ImGUIAPI::TextBar("Mesh", assetInfo.GetName());
			}
			else
			{
				ExternalAPI::ImGUIAPI::TextBar("Mesh", "null");
			}
			if (ImGui::BeginDragDropTarget()) {
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(EnumReflection::EnumString<AssetType>(AssetType::Mesh).c_str())) {
					UUID Data = *(const UUID*)payload->Data;
					meshComp.SetMesh(Data);
				}
				ImGui::EndDragDropTarget();
			}
			if (ImGui::BeginPopupContextItem("RemoveMesh")) {
				ImGui::EndPopup();
			}
			if (ImGui::BeginPopup("RemoveMesh")) {
				if (ImGui::MenuItem("Remove Mesh")) {
					meshComp.RemoveMesh();
				}

				ImGui::EndPopup();
			}
			ImGui::Separator();
			const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;
			UI::ScopedStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 0,1.5 });
			bool open = ImGui::TreeNodeEx("MaterialTablerwrs", treeNodeFlags, "Material Table");
			if (!open)return;
			for (auto& [index, material] : meshComp.MaterialTable->GetMaterials())
			{
				std::string name = material != nullptr ? material->Name : "null";
				ExternalAPI::ImGUIAPI::TextBar(fmt::format("Index {}",index), name);
				if (ImGui::BeginDragDropTarget())
				{
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(EnumReflection::EnumString<AssetType>(AssetType::Material).c_str()))
					{
						uint64_t Data = *(const uint64_t*)payload->Data;
						if (AssetManager::HasAsset(Data))
						{
							auto material = AssetManager::GetAsset<Material>(Data);
							meshComp.MaterialTable->SetMaterial(index, material);
						}
					}
					ImGui::EndDragDropTarget();
				}
			}

			ImGui::TreePop();
		});
		DrawComponents<SpriteComponent>({ "Sprite" }, entity, [](SpriteComponent& spriteComp) {
			//if (spriteComp.GetTexture() != nullptr) {
			//	ImGui::Image((ImTextureID)spriteComp.GetTexture()->GetID(), { 30,30 });
			//}
			//else {
			//}
			// get ID crashing applicaiton needs fix
			//ImGui::Image((ImTextureID)Renderer::GetWhiteTexture()->GetID(), {30,30});
			if (ImGui::BeginPopupContextItem("RemoveTexture")) {
				ImGui::EndPopup();
			}
			if (ImGui::BeginPopup("RemoveTexture")) {
				if (ImGui::MenuItem("Remove Texture")) {
					spriteComp.RemoveTexture();
				}
				ImGui::EndPopup();
			}

			if (ImGui::BeginDragDropTarget()) {
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("")) {
					uint64_t Data = *(const uint64_t*)payload->Data;
					spriteComp.m_TextureAssetPointerID = Data;
				}
				ImGui::EndDragDropTarget();
			}

			ImGui::ColorEdit4("##Colour", glm::value_ptr(spriteComp.Colour));
		});
		DrawComponents<NativeScriptComponent>("Native Script", entity, [](NativeScriptComponent& NativeScriptComp) {
			ExternalAPI::ImGUIAPI::TextBar("Sript", NativeScriptComp.GetScriptName());
		});
		
		DrawComponents<DirectionalLightComponent>("Directonal Light", entity, [](DirectionalLightComponent& drl) {
			ImGui::ColorEdit3("Ambient", drl.Color.GetValue_Ptr());
			ImGui::DragFloat("Intensity", &drl.Intensity, 0.01, 0.0f, 100);
		});

		DrawComponents<PointLightComponent>("Directonal Light", entity, [](PointLightComponent& pl) {
			ImGui::ColorEdit3("Ambient", pl.Color.GetValue_Ptr());
			ImGui::DragFloat("Intensity", &pl.Intensity, 0.01, 0.0f, 100);
			ImGui::DragFloat("Intensity", &pl.Intensity, 0.01, 0.0f, 100);
			ImGui::DragFloat("Radius", &pl.Radius, 0.01, 0.0f, 100);
			ImGui::DragFloat("Constant", &pl.Constant, 0.001);
			ImGui::DragFloat("Linear", &pl.Linear, 0.001);
			ImGui::DragFloat("Quadratic", &pl.Quadratic, 0.001);
		});

		DrawComponents<SpotLightComponent>("Directonal Light", entity, [](SpotLightComponent& sl) {
			ImGui::ColorEdit3("Ambient", sl.Color.GetValue_Ptr());
			ImGui::DragFloat("Intensity", &sl.Intensity, 0.01, 0.0f, 100);

			ImGui::DragFloat("Radius", &sl.Radius, 0.01, 0.0f, 100);

			ImGui::NewLine();
			ImGui::DragFloat("Constant", &sl.Constant, 0.001);
			ImGui::DragFloat("Linear", &sl.Linear, 0.001);
			ImGui::DragFloat("Quadratic", &sl.Quadratic, 0.001);
			ImGui::DragFloat("CutOff", &sl.CutOff, 0.001);
			ImGui::DragFloat("Outer-Cutoff", &sl.OuterCutOff, 0.001);
		});

		DrawComponents<CameraComponent>("Camera", entity, [](CameraComponent& cameraComp) {
			ImGui::SliderFloat("Field ov fiew", &cameraComp.FovDeg, 0, 360);
			ImGui::SliderFloat("Near plane", &cameraComp.NearPlane, -1, 1);
			ImGui::SliderFloat("Far plane", &cameraComp.FarPlane, 0, 10000);
			if (ImGui::IsItemHovered()) {
				ImGui::BeginTooltip();
				ImGui::Text("Setting to 0 means you can see any object no matter how far away it is");
				ImGui::EndTooltip();
			}
			ExternalAPI::ImGUIAPI::EnumCombo("Type", cameraComp.Type);
			/*
			if (cameraComp.m_AutoSetDimension == false) {
				int tempWidth = (int)cameraComp.m_Width;
				int tempHeight = (int)cameraComp.m_Height;
				ImGui::DragInt("view width", &tempWidth, 1, 0);
				ImGui::DragInt("view height", &tempHeight, 1, 0);
				cameraComp.m_Width = tempWidth;
				cameraComp.m_Height = tempHeight;
			}
			*/

		}, "if nothing visible set roation of z axis to 1");

		DrawComponents<CubeColliderComponent>("Cube Collider", entity, [](CubeColliderComponent& cubeCollider) {
			ExternalAPI::ImGUIAPI::CheckBox("IsTrigger", &cubeCollider.IsTrigger);
			DrawVectorControl("Offset Location", cubeCollider.OffsetLocation);
			DrawVectorControl("Offset Scale", cubeCollider.OffsetScale, 1.0f);

			ExternalAPI::ImGUIAPI::TextBar("PhysicsMaterial", cubeCollider.HasPhysicsMaterial() != false ? AssetManager::GetAssetInfo(cubeCollider.m_PhysicsMaterialPointerID).GetName() : "null");
			if (ImGui::BeginPopupContextItem("Remove Physics Material")) {
				ImGui::EndPopup();
			}
			if (ImGui::BeginPopup("Remove Physics Material")) {
				if (ImGui::MenuItem("Remove Material")) {
					cubeCollider.RemovePhysicsMaterial();
				}

				ImGui::EndPopup();
			}
			if (ImGui::BeginDragDropTarget()) {
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(EnumReflection::EnumString(AssetType::PhysicsMaterial).c_str())) {
					UUID Data = *(const UUID*)payload->Data;
					cubeCollider.m_PhysicsMaterialPointerID = Data;
				}
				ImGui::EndDragDropTarget();
			}
		});
		DrawComponents<SphereColliderComponent>("Sphere Collider", entity, [](SphereColliderComponent& sphereCollider) {
			ExternalAPI::ImGUIAPI::CheckBox("IsTrigger", &sphereCollider.IsTrigger);
			ImGui::DragFloat("Radius", &sphereCollider.Radius, 0.5);
			DrawVectorControl("Offset Location", sphereCollider.OffsetLocation);

			ExternalAPI::ImGUIAPI::TextBar("PhysicsMaterial", sphereCollider.HasPhysicsMaterial() != false ? AssetManager::GetAssetInfo(sphereCollider.m_PhysicsMaterialPointerID).GetName() : "null");
			if (ImGui::BeginPopupContextItem("Remove Physics Material")) {
				ImGui::EndPopup();
			}
			if (ImGui::BeginPopup("Remove Physics Material")) {
				if (ImGui::MenuItem("Remove Material")) {
					sphereCollider.RemovePhysicsMaterial();
				}

				ImGui::EndPopup();
			}
			if (ImGui::BeginDragDropTarget()) {
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(EnumReflection::EnumString(AssetType::PhysicsMaterial).c_str())) {
					UUID Data = *(const UUID*)payload->Data;
					sphereCollider.m_PhysicsMaterialPointerID = Data;
				}
				ImGui::EndDragDropTarget();
			}
		});
		DrawComponents<CapsuleColliderComponent>("Capsule Collider", entity, [](CapsuleColliderComponent& capsuleCollider) {
			ExternalAPI::ImGUIAPI::CheckBox("IsTrigger", &capsuleCollider.IsTrigger);
			ImGui::DragFloat("Radius", &capsuleCollider.Radius, 0.5);
			ImGui::DragFloat("Height", &capsuleCollider.Height, 0.5);
			ExternalAPI::ImGUIAPI::EnumCombo("Direction", capsuleCollider.Direction);
			DrawVectorControl("Offset Location", capsuleCollider.OffsetLocation);

			ExternalAPI::ImGUIAPI::TextBar("PhysicsMaterial", capsuleCollider.HasPhysicsMaterial() != false ? AssetManager::GetAssetInfo(capsuleCollider.m_PhysicsMaterialPointerID).GetName() : "null");
			if (ImGui::BeginPopupContextItem("Remove Physics Material")) {
				ImGui::EndPopup();
			}
			if (ImGui::BeginPopup("Remove Physics Material")) {
				if (ImGui::MenuItem("Remove Material")) {
					capsuleCollider.RemovePhysicsMaterial();
				}

				ImGui::EndPopup();
			}
			if (ImGui::BeginDragDropTarget()) {
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(EnumReflection::EnumString(AssetType::PhysicsMaterial).c_str())) {
					UUID Data = *(const UUID*)payload->Data;

					capsuleCollider.m_PhysicsMaterialPointerID = Data;
				}
				ImGui::EndDragDropTarget();
			}
		});
		DrawComponents<MeshColliderComponent>("Mesh Collider", entity, [](MeshColliderComponent& meshCollider) {
			ExternalAPI::ImGUIAPI::CheckBox("IsTrigger", &meshCollider.IsTrigger);

			ExternalAPI::ImGUIAPI::TextBar("Mesh", meshCollider.GetMesh() != nullptr ? meshCollider.GetMesh()->GetName() : "null");
			if (ImGui::BeginPopupContextItem("RemoveMesh")) {
				ImGui::EndPopup();
			}
			if (ImGui::BeginPopup("RemoveMesh")) {
				if (ImGui::MenuItem("Remove Mesh")) {
					meshCollider.RemoveMeshSource();
				}

				ImGui::EndPopup();
			}
			if (ImGui::BeginDragDropTarget()) {
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(EnumReflection::EnumString<AssetType>(AssetType::Mesh).c_str())) {
					UUID Data = *(const UUID*)payload->Data;
					meshCollider.m_MeshAssetPointerID = Data;
				}
				ImGui::EndDragDropTarget();
			}


			ExternalAPI::ImGUIAPI::TextBar("PhysicsMaterial", meshCollider.HasPhysicsMaterial() != false ? AssetManager::GetAssetInfo(meshCollider.m_PhysicsMaterialPointerID).GetName() : "null");
			if (ImGui::BeginPopupContextItem("Remove Physics Material")) {
				ImGui::EndPopup();
			}
			if (ImGui::BeginPopup("Remove Physics Material")) {
				if (ImGui::MenuItem("Remove Material")) {
					meshCollider.RemovePhysicsMaterial();
				}

				ImGui::EndPopup();
			}
			if (ImGui::BeginDragDropTarget()) {
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(EnumReflection::EnumString(AssetType::PhysicsMaterial).c_str())) {
					UUID Data = *(const UUID*)payload->Data;
					meshCollider.m_PhysicsMaterialPointerID = Data;
				}
				ImGui::EndDragDropTarget();
			}

	

		});
		DrawComponents<RigidBodyComponent>("RigidBody", entity, [](RigidBodyComponent& rigidBody) {
			ExternalAPI::ImGUIAPI::EnumCombo<RigidBodyType>("Type", rigidBody.m_RigidBodyType);
			if (rigidBody.m_RigidBodyType == RigidBodyType::Static)return;

			ImGui::DragFloat("Mass", &rigidBody.Mass, 0.5, 0);
			ImGui::DragFloat("LinearDrag", &rigidBody.LinearDrag, 0.5, 0);
			ImGui::DragFloat("AngularDrag", &rigidBody.AngularDrag, 0.5);
			ExternalAPI::ImGUIAPI::CheckBox("Gravity", &rigidBody.Gravity);
			ExternalAPI::ImGUIAPI::CheckBox("Kinematic", &rigidBody.Kinimatic);

			DrawVectorControl("Freeze Location", rigidBody.FreezeLocation, false);
			DrawVectorControl("Freeze Rotation", rigidBody.FreezeRotation, false);
		});

		DrawComponents<ScriptComponent>("Scripts", entity, [&](ScriptComponent& scriptComp) {
			if (ImGui::Button("Add Script")) {
				ImGui::OpenPopup("Open Scripts");
			}
			if (ImGui::BeginPopup("Open Scripts")) {
				for (const auto [scriptName, script] : ScriptEngine::GetScripts()) {
					if (ImGui::MenuItem(scriptName.c_str())) {
						scriptComp.ScriptsNames.insert(scriptName);
						ImGui::CloseCurrentPopup();
					}
				}
				ImGui::EndPopup();
			}
			/*
			{
				std::vector<std::string> deletes;
				for (const auto& scriptName : scriptComp.ScriptsNames)
				{
					
					if (!ScriptEngine::EntityClassExists(scriptName))
					{
						deletes.emplace_back(scriptName);
						continue;
					}
					UI::ScopedID(scriptName.c_str());
					if (ImGui::IsItemClicked())
					{
						ImGui::OpenPopup("Change Script");
					}
					if (ImGui::BeginPopup("Change Script"))
					{
						for (const auto& [newScriptName, script] : ScriptEngine::GetScripts())
						{
							if (ImGui::MenuItem(newScriptName.c_str()))
							{
								if (newScriptName != scriptName)
								{
									scriptComp.ScriptsNames.insert(newScriptName);
									deletes.emplace_back(scriptName);
								}
								ImGui::CloseCurrentPopup();
							}
						}
						ImGui::EndPopup();
					}
					ImGui::SameLine();
					if (ImGui::Button("-", { 20,20 }))
					{
						deletes.emplace_back(scriptName);
					}
				}
				for (auto& scriptname : deletes)
					scriptComp.ScriptsNames.erase(scriptname);
			}
			*/
			if (m_CurrentWorld->IsPlaying() == false)
			{
				for (const auto& scriptName : scriptComp.ScriptsNames)
				{
					const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;
					UI::ScopedStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 0,1.5 });
					bool open = ImGui::TreeNodeEx(scriptName.c_str(), treeNodeFlags, scriptName.c_str());
					if (!open)continue;
					Count<ScriptClass> entityClass = ScriptEngine::GetScriptClass(scriptName);
					const auto& fields = entityClass->GetFields();

					if (ScriptEngine::HasScriptFieldMap(entity) == false)
						ScriptEngine::CreateScriptFieldMap(entity);
					
					auto& entityFields = ScriptEngine::GetScriptFieldMap(entity);

					for (auto& [fieldName, field] : fields)
					{
						// field already contains script
						if (!entityFields.contains(scriptName))
							entityFields[scriptName] = {};

						if (entityFields[scriptName].contains(fieldName))
							continue;
						entityFields[scriptName][fieldName] = {};

						ScriptFieldInstance& scriptInstance = entityFields[scriptName][fieldName];
						switch (field.Type)
						{
							SET_FEILD_DEFAULT(Float, float);
							SET_FEILD_DEFAULT(Double, double);
							SET_FEILD_DEFAULT(Bool, bool);
							SET_FEILD_DEFAULT(Char, char);
							SET_FEILD_DEFAULT(Byte, int8_t);
							SET_FEILD_DEFAULT(Short, int16_t);
							SET_FEILD_DEFAULT(Int, int32_t);
							SET_FEILD_DEFAULT(Long, int64_t);
							SET_FEILD_DEFAULT(UByte, uint8_t);
							SET_FEILD_DEFAULT(UShort, uint16_t);
							SET_FEILD_DEFAULT(UInt, uint32_t);
							SET_FEILD_DEFAULT(ULong, uint64_t);
							SET_FEILD_DEFAULT(Vector2, Vector2);
							SET_FEILD_DEFAULT(Vector3, Vector);
							SET_FEILD_DEFAULT(Vector4, Vector4);
							SET_FEILD_DEFAULT(Entity, UUID);
							default:
								break;
						}
					}
					for (auto& [fieldName, field] : fields)
					{
						ScriptFieldInstance& scriptField = entityFields[scriptName].at(fieldName);
						switch (field.Type)
						{
							SET_FIELD_NUMERICAL_VALUE(Float, float, fieldName, ImGuiDataType_Float);
							SET_FIELD_NUMERICAL_VALUE(Double, double, fieldName, ImGuiDataType_Double);

							SET_FIELD_NUMERICAL_VALUE(Byte, int8_t, fieldName, ImGuiDataType_S8);
							SET_FIELD_NUMERICAL_VALUE(Short, int16_t, fieldName, ImGuiDataType_S16);
							SET_FIELD_NUMERICAL_VALUE(Int, int32_t, fieldName, ImGuiDataType_S32);
							SET_FIELD_NUMERICAL_VALUE(Long, int64_t, fieldName, ImGuiDataType_S64);

							SET_FIELD_NUMERICAL_VALUE(UByte, uint8_t, fieldName, ImGuiDataType_S8);
							SET_FIELD_NUMERICAL_VALUE(UShort, uint16_t, fieldName, ImGuiDataType_U16);
							SET_FIELD_NUMERICAL_VALUE(UInt, uint32_t, fieldName, ImGuiDataType_U32);
							SET_FIELD_NUMERICAL_VALUE(ULong, uint64_t, fieldName, ImGuiDataType_U64);

							default:
								break;
						}
					}
					ImGui::TreePop();
				}
			}
			else
			{
				if (!ScriptEngine::EntityHasScripts(entity))return;

				auto& scriptInstances = ScriptEngine::GetScriptInstnace(entity);
				for (auto& [scriptName, instance] : scriptInstances)
				{
					const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;
					bool open;
					{
						UI::ScopedStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 0,1.5 });
						open = ImGui::TreeNodeEx(scriptName.c_str(), treeNodeFlags, scriptName.c_str());
					}
					if (!open)continue;
					if (!instance)continue;

					const auto& fields = instance->GetScriptClass()->GetFields();
					for (const auto& [name, field] : fields)
					{
						switch (field.Type)
						{
							SET_FIELD_NUMERICAL_VALUE_RUNTIME(Float, float, name, ImGuiDataType_Float);
							SET_FIELD_NUMERICAL_VALUE_RUNTIME(Double, double, name, ImGuiDataType_Double);
								
							SET_FIELD_NUMERICAL_VALUE_RUNTIME(Byte, int8_t, name, ImGuiDataType_S8);
							SET_FIELD_NUMERICAL_VALUE_RUNTIME(Short, int16_t, name, ImGuiDataType_S16);
							SET_FIELD_NUMERICAL_VALUE_RUNTIME(Int, int32_t, name, ImGuiDataType_S32);
							SET_FIELD_NUMERICAL_VALUE_RUNTIME(Long, int64_t, name, ImGuiDataType_S64);
								
							SET_FIELD_NUMERICAL_VALUE_RUNTIME(UByte, uint8_t, name, ImGuiDataType_S8);
							SET_FIELD_NUMERICAL_VALUE_RUNTIME(UShort, uint16_t, name, ImGuiDataType_U16);
							SET_FIELD_NUMERICAL_VALUE_RUNTIME(UInt, uint32_t, name, ImGuiDataType_U32);
							SET_FIELD_NUMERICAL_VALUE_RUNTIME(ULong, uint64_t, name, ImGuiDataType_U64);
							default:
								break;
						}
					}
					ImGui::TreePop();
				}
			}
		});
	}

	void SceneHierachyPanel::DrawVectorControl(const std::string& UniqeLabel, Vector& Vec, float ResetValue, float columnWidth, float Speed) {
		ImGuiIO& io = ImGui::GetIO();
		auto boldFont = io.Fonts->Fonts[0];
		ImGui::PushID(UniqeLabel.c_str());// this id is for everything here so imgui does not assign something to the value that we have here
		ImGui::Columns(2); // distance between label and edits
		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text(UniqeLabel.c_str());
		ImGui::NextColumn();

		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0,0 });
		ImGui::GetStyle().FrameRounding = 0;

		float LineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f; // comes from IMGUI
		ImVec2 buttonSize = { LineHeight + 3.0f,LineHeight };

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 1.0f,0.0f,0.0f,1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 1.0f,0.5f,0.0f,1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 1.0f,0.0f,0.0f,1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("X", buttonSize)) {
			Vec.X = ResetValue;
		}
		ImGui::PopFont();

		ImGui::SameLine();
		ImGui::DragFloat("##x", &Vec.X, Speed, 0, 0, "%.3f"); // does not show ## as label
		ImGui::PopItemWidth();
		ImGui::SameLine();
		ImGui::PopStyleColor(3);

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.0f,.5f,0.0f,1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 1.0f,0.5f,0.0f,1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.0f,.5f,0.0f,1.0f });

		ImGui::PushFont(boldFont);
		if (ImGui::Button("Y", buttonSize)) {
			Vec.Y = ResetValue;
		}
		ImGui::PopFont();

		ImGui::SameLine();
		ImGui::DragFloat("##Y", &Vec.Y, Speed, 0, 0, "%.3f"); // does not show ## as label
		ImGui::PopItemWidth();
		ImGui::SameLine();
		ImGui::PopStyleColor(3);

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.0f,0.0f,1.0f,1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 1.0f,0.5f,0.0f,1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.0f,0.0f,1.0f,1.0f });

		ImGui::PushFont(boldFont);
		if (ImGui::Button("Z", buttonSize)) {
			Vec.Z = ResetValue;
		}
		ImGui::PopFont();

		ImGui::SameLine();
		ImGui::DragFloat("##Z", &Vec.Z, Speed, 0, 0, "%.3f"); // does not show ## as label
		ImGui::PopItemWidth();
		ImGui::PopStyleColor(3);

		ImGui::GetStyle().FrameRounding = 6;

		ImGui::PopStyleVar(1);
		ImGui::Columns(1);
		ImGui::PopID();
	}
	void SceneHierachyPanel::DrawVector4Control(const std::string& UniqeLabel, glm::vec4& Vec, float ResetValue, float columnWidth) {
		ImGuiIO& io = ImGui::GetIO();
		auto boldFont = io.Fonts->Fonts[0];
		ImGui::PushID(UniqeLabel.c_str());// this id is for everything here so imgui does not assign something to the value that we have here
		ImGui::Columns(2); // distance between label and edits
		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text(UniqeLabel.c_str());
		ImGui::NextColumn();
		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0,0 });
		float LineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f; // comes from IMGUI
		ImVec2 buttonSize = { LineHeight + 3.0f,LineHeight };

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 1.0f,0.0f,0.0f,1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 1.0f,0.5f,0.0f,1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 1.0f,0.0f,0.0f,1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("X", buttonSize)) {
			Vec.r = ResetValue;
		}
		ImGui::PopFont();
		ImGui::SameLine();
		ImGui::DragFloat("##x", &Vec.r, 0.1f, 0, 0, "%.3f"); // does not show ## as label
		ImGui::PopItemWidth();
		ImGui::SameLine();
		ImGui::PopStyleColor(3);

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.0f,.5f,0.0f,1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 1.0f,0.5f,0.0f,1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.0f,.5f,0.0f,1.0f });

		ImGui::PushFont(boldFont);
		if (ImGui::Button("Y", buttonSize)) {
			Vec.y = ResetValue;
		}
		ImGui::PopFont();

		ImGui::SameLine();
		ImGui::DragFloat("##Y", &Vec.y, 0.1f, 0, 0, "%.3f"); // does not show ## as label
		ImGui::PopItemWidth();
		ImGui::SameLine();
		ImGui::PopStyleColor(3);

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.0f,0.0f,1.0f,1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 1.0f,0.5f,0.0f,1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.0f,0.0f,1.0f,1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("Z", buttonSize)) {
			Vec.z = ResetValue;
		}
		ImGui::PopFont();

		ImGui::SameLine();
		ImGui::DragFloat("##Z", &Vec.z, 0.1f, 0, 0, "%.3f"); // does not show ## as label
		ImGui::PopItemWidth();
		ImGui::PopStyleColor(3);

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 1.0f,1.0f,1.0f,1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 1.0f,1.0f,1.0f,1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 1.0f,1.0f,1.0f,1.0f });

		ImGui::PushFont(boldFont);
		if (ImGui::Button("A", buttonSize)) {
			Vec.a = ResetValue;
		}
		ImGui::PopFont();

		ImGui::SameLine();
		ImGui::DragFloat("##A", &Vec.a, 0.1f, 0, 0, "%.3f"); // does not show ## as label
		ImGui::PopItemWidth();
		ImGui::PopStyleColor(3);
		ImGui::PopStyleVar();
		ImGui::Columns(1);
		ImGui::PopID();
	}
	void SceneHierachyPanel::DrawVectorControl(const std::string& UniqeLabel, VectorTemplate<bool>& Vec, bool resetValue, float columnWidth) {

		ImGuiIO& io = ImGui::GetIO();
		auto boldFont = io.Fonts->Fonts[0];
		ImGui::PushID(UniqeLabel.c_str());// this id is for everything here so imgui does not assign something to the value that we have here
		ImGui::Columns(2); // distance between label and edits
		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text(UniqeLabel.c_str());
		ImGui::NextColumn();

		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0,0 });
		ImGui::GetStyle().FrameRounding = 0;

		float LineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f; // comes from IMGUI
		ImVec2 buttonSize = { LineHeight + 3.0f,LineHeight };

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 1.0f,0.0f,0.0f,1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 1.0f,0.5f,0.0f,1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 1.0f,0.0f,0.0f,1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("X", buttonSize)) {
			Vec.X = resetValue;
		}
		ImGui::PopFont();

		ImGui::SameLine();
		ImGui::Checkbox("##x", &Vec.X); // does not show ## as label
		ImGui::PopItemWidth();
		ImGui::SameLine();
		ImGui::PopStyleColor(3);

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.0f,.5f,0.0f,1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 1.0f,0.5f,0.0f,1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.0f,.5f,0.0f,1.0f });

		ImGui::PushFont(boldFont);
		if (ImGui::Button("Y", buttonSize)) {
			Vec.Y = resetValue;
		}
		ImGui::PopFont();

		ImGui::SameLine();
		ImGui::Checkbox("##Y", &Vec.Y); // does not show ## as label
		ImGui::PopItemWidth();
		ImGui::SameLine();
		ImGui::PopStyleColor(3);

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.0f,0.0f,1.0f,1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 1.0f,0.5f,0.0f,1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.0f,0.0f,1.0f,1.0f });

		ImGui::PushFont(boldFont);
		if (ImGui::Button("Z", buttonSize)) {
			Vec.Z = resetValue;
		}
		ImGui::PopFont();

		ImGui::SameLine();
		ImGui::Checkbox("##Z", &Vec.Z); // does not show ## as label
		ImGui::PopItemWidth();
		ImGui::PopStyleColor(3);

		ImGui::GetStyle().FrameRounding = 6;

		ImGui::PopStyleVar(1);
		ImGui::Columns(1);
		ImGui::PopID();
	}
}
