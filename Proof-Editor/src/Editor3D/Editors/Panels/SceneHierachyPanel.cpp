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
#include "../../ImGUIAPI.h"
#include "Proof/Scene/ExampleSccripts.h"
#include "Proof/Scene/Material.h"
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
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include "Proof/Scene/Prefab.h"
#include "Proof/Math/Random.h"
//include those before stdlig.h

#include "misc/cpp/imgui_stdlib.h"
#include "Proof/ProofCore.h"
#include "Proof/Scene/Mesh.h"

namespace Proof
{
	#define SET_FEILD_DEFAULT(FieldType, Type)           \
			case ScriptFieldType::FieldType:          \
				scriptInstance.SetValue<Type>(entityClass->GetFieldDefaultValue<Type>(fieldName));  \
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

	#define Set_FIELD_ENUM_NUMERICAL_VALUE(FieldType,Type) \
			case ScriptFieldType::FieldType:          \
			{																													\
				UI::ScopedID id(fmt::format("{}{}", field.Name.c_str(), sizeof(Type)).c_str());									\
					std::string currentSeelct = "";																				\
					Type currentSelectValue = 0;																				\
					const auto& enumDatas = ScriptEngine::GetEnumClasses().at(enumTypeName).second;								\
					Type value = scriptField.GetValue<Type>();																	\
					for (const auto& enumInfo : enumDatas)																		\
					{																											\
						if (enumInfo.GetValue<Type>() == value)																	\
						{																										\
							currentSeelct = enumInfo.Name;																		\
							currentSelectValue = enumInfo.GetValue<Type>();														\
							break;																								\
						}																										\
					}																											\
					ImGui::Text(fieldName.c_str());																				\
						ImGui::SameLine();																						\
						if (ImGui::BeginCombo("##CurrentEnumVal", currentSeelct.c_str()))										\
						{																										\
							for (size_t i = 0; i < enumDatas.size(); i++)														\
							{																									\
								bool isSelected = (currentSeelct == enumDatas[i].Name);											\
								if (ImGui::Selectable(enumDatas[i].Name.c_str(), isSelected))									\
								{																								\
									currentSelectValue = enumDatas[i].GetValue<Type>();											\
								}																								\
									if (isSelected)																				\
									{																							\
										ImGui::SetItemDefaultFocus();															\
									}																							\
							}																									\
								ImGui::EndCombo();																				\
						}																										\
						scriptField.SetValue<Type>(currentSelectValue);\
						break;																									\
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

	#define Set_FIELD_ENUM_NUMERICAL_VALUE_RUNTIME(FieldType, Type)                    \
    case ScriptFieldType::FieldType:                                              \
    {                                                                             \
        UI::ScopedID id(fmt::format("{}{}", field.Name.c_str(), sizeof(Type)).c_str()); \
        std::string currentSeelct = "";                                           \
        Type currentSelectValue = 0;                                              \
        const auto& enumDatas = ScriptEngine::GetEnumClasses().at(enumTypeName).second; \
        Type value = instance->GetFieldValue<Type>(name);                         \
        for (const auto& enumInfo : enumDatas)                                     \
        {                                                                         \
            if (enumInfo.GetValue<Type>() == value)                                \
            {                                                                     \
                currentSeelct = enumInfo.Name;                                     \
                currentSelectValue = enumInfo.GetValue<Type>();                     \
                break;                                                            \
            }                                                                     \
        }                                                                         \
        ImGui::Text(field.Name.c_str());                                          \
        ImGui::SameLine();                                                        \
        if (ImGui::BeginCombo("##CurrentEnumVal", currentSeelct.c_str()))          \
        {                                                                         \
            for (size_t i = 0; i < enumDatas.size(); i++)                          \
            {                                                                     \
                bool isSelected = (currentSeelct == enumDatas[i].Name);            \
                if (ImGui::Selectable(enumDatas[i].Name.c_str(), isSelected))      \
                {                                                                 \
                    currentSelectValue = enumDatas[i].GetValue<Type>();             \
                }                                                                 \
                if (isSelected)                                                   \
                {                                                                 \
                    ImGui::SetItemDefaultFocus();                                   \
                }                                                                 \
            }                                                                     \
            ImGui::EndCombo();                                                    \
        }                                                                         \
        instance->SetFieldValue<Type>(name, currentSelectValue);                   \
        break;                                                                    \
    }
	#define DEFAULT_MESH_SET(Type) \
		if (ImGui::MenuItem(#Type))\
		{\
			newEntity = m_ActiveWorld->CreateEntity(#Type);\
			newEntity.AddComponent<MeshComponent>().SetMesh(AssetManager::GetDefaultAsset(DefaultRuntimeAssets::Type)->GetID());\
		}
	template<class T>
	static void AddComponentGui(Entity entity, const std::string& name) {
		if (ImGui::MenuItem(name.c_str()))
		{
			entity.AddComponent<T>();
			ImGui::CloseCurrentPopup();
		}
	};
	SceneHierachyPanel::SceneHierachyPanel()
	{
		
	}
	void SceneHierachyPanel::OnImGuiRender(const char* dsiplayName, bool& isOpen) {

		if (isOpen == false)
			return;
		PF_PROFILE_FUNC();
		UI::ScopedID customID(GetCustomPushID().Get());
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0,0 });
		if (ImGui::Begin(dsiplayName, &isOpen));
		{
			/*
			auto &a =m_ActiveWorld->m_Registry.view<>();
			for (auto& g : a) {

			}
			*/
			if (!m_SelectedEntity)m_SelectedEntity = {};
			ImGui::PushStyleColor(ImGuiCol_ChildBg, { 0,0,0,1 });
			ImGui::BeginChild("Child Herieachy", { ImGui::GetContentRegionAvail().x,ImGui::GetWindowHeight() / 2 });
			if (ImGui::BeginPopupContextWindow(0)) { // right click adn open a new entitiy
				CreateEntityMenu();
				ImGui::EndPopup();
			}
			
			{
				m_WindowHoveredorFocus = ImGui::IsWindowHovered() || ImGui::IsWindowFocused();
				m_ActiveWorld->m_Registry.each([&](auto entityID)
				{
					Entity entity = { entityID,m_ActiveWorld.Get() };
					if (entity.HasParent() == false)
						DrawEntityNode(entity);
				});

				if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered() && ImGui::IsAnyItemHovered() == false) {
					m_SelectedEntity = {};
				}
			
			}
			ImGui::EndChild();
			if (ImGui::BeginDragDropTarget()) {
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("EntityNewOwner")) {

					Entity Data = *(const Entity*)payload->Data;
					Data.Unparent();
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

	bool  SceneHierachyPanel::CreateEntityMenu(Entity owner) {
		uint64_t selectedPreviousEntityID = m_SelectedEntity.GetUUID();// we are doing this inncase we created a child entity
		Entity newEntity;
		if (ImGui::MenuItem("Entity"))
			newEntity = m_ActiveWorld->CreateEntity();
		if (ImGui::BeginMenu("Light")) {
			if (ImGui::MenuItem("Point ")) {
				newEntity = m_ActiveWorld->CreateEntity("Point Light");
				newEntity.AddComponent<PointLightComponent>();
			}
			if (ImGui::MenuItem("Spot")) {
				newEntity = m_ActiveWorld->CreateEntity("Spot Light");
				newEntity.AddComponent<SpotLightComponent>();
			}
			if (ImGui::MenuItem("Directional")) {
				newEntity = m_ActiveWorld->CreateEntity("Directional Light");
				newEntity.AddComponent<DirectionalLightComponent>();
			}
			if (ImGui::MenuItem("SkyLight"))
			{
				newEntity = m_ActiveWorld->CreateEntity("Sky Light");
				newEntity.AddComponent<SkyLightComponent>();
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Mesh")) {
			DEFAULT_MESH_SET(Cube);
			DEFAULT_MESH_SET(Sphere);
			DEFAULT_MESH_SET(Capsule);
			DEFAULT_MESH_SET(Cylinder);
			DEFAULT_MESH_SET(Cone);
			DEFAULT_MESH_SET(Torus);
			DEFAULT_MESH_SET(Plane);
			
			if (ImGui::MenuItem("Empty Mesh"))
			{
				newEntity = m_ActiveWorld->CreateEntity("Mesh");
				newEntity.AddComponent<MeshComponent>();
			}

			ImGui::EndMenu();
		}
		if (ImGui::MenuItem("Camera")) {
			newEntity = m_ActiveWorld->CreateEntity("Camera");
			newEntity.AddComponent<CameraComponent>();
		}
		if (owner && newEntity.GetUUID() != 0) {
			owner.AddChild(newEntity);
		}
		if (newEntity.GetUUID() != 0)
			return true;
		return false;
	}
	void SceneHierachyPanel::DrawEntityNode(Entity entity) {
		auto& tc = entity.GetComponent<TagComponent>().Tag;
		//PF_ENGINE_TRACE("Name:{} X:{} Y:{} Z:{}", tc, entity.GetComponent<TransformComponent>().GetRotationEuler().x,
		//	entity.GetComponent<TransformComponent>().GetRotationEuler().y, entity.GetComponent<TransformComponent>().GetRotationEuler().z);

		ImGui::PushID(entity.GetUUID());
		ImGuiTreeNodeFlags flags = ((m_SelectedEntity == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;

		if (entity.GetComponent<HierarchyComponent>().Children.empty()) {
			flags |= ImGuiTreeNodeFlags_Leaf;//makes the tree not use an arrow
		}

		flags |= ImGuiTreeNodeFlags_SpanFullWidth;
		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity.GetUUID(), flags, tc.c_str());
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
				m_ActiveWorld->DeleteEntity(m_SelectedEntity, true);

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
			//Editore3D::Get()->m_EditorCamera.SetPosition( m_ActiveWorld->GetWorldSpaceLocation(m_SelectedEntity));
		}
		if (m_SelectedEntity && ImGui::IsKeyPressed((ImGuiKey)KeyBoardKey::F)) {
			//Editore3D::Get()->m_EditorCamera.SetPosition (m_ActiveWorld->GetWorldSpaceLocation(m_SelectedEntity));
		}

		if (opened) {
			for (const UUID& I : entity.GetComponent<HierarchyComponent>().Children) {
				;
				DrawEntityNode(m_ActiveWorld->GetEntity(I));
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
		T& component = entity.GetComponent<T>();
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
		auto& Tag = entity.GetComponent<TagComponent>();
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

			AddComponentGui<SkyLightComponent>(entity, "Sky Light");
			AddComponentGui<DirectionalLightComponent>(entity, "Directional Light");
			AddComponentGui<PointLightComponent>(entity, "Point Light");
			AddComponentGui<SpotLightComponent>(entity, "Spot Light");
			AddComponentGui<CameraComponent>(entity, "Camera");

			AddComponentGui<CubeColliderComponent>(entity, "Cube Collider");
			AddComponentGui<SphereColliderComponent>(entity, "Sphere Collider");
			AddComponentGui<CapsuleColliderComponent>(entity, "Capsule Collider");
			AddComponentGui<MeshColliderComponent>(entity, "Mesh Collider");
			AddComponentGui<RigidBodyComponent>(entity, "Rigid Body");

			AddComponentGui<TextComponent>(entity, "Text");

			AddComponentGui<ScriptComponent>(entity, "Scripts");
			AddComponentGui<PlayerInputComponent>(entity, "Player Input");
			AddComponentGui<PlayerHUDComponent>(entity, "Player HUD");

			AddComponentGui<ParticleSystemComponent>(entity, "Particle System");

			AddComponentGui<AudioComponent>(entity, "Audio");
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
			glm::vec3 rotationdeg = glm::degrees(transformComp.GetRotationEuler());
			DrawVectorControl("Rotation", rotationdeg);
			transformComp.SetRotationEuler(glm::radians(rotationdeg));
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

			ExternalAPI::ImGUIAPI::CheckBox("Visible", &meshComp.Visible);
		});
		DrawComponents<SpriteComponent>({ "Sprite" }, entity, [](SpriteComponent& spriteComp) {
			if (spriteComp.Texture != nullptr)
				UI::Image(spriteComp.Texture, {30,30});
			else 
				UI::Image(Renderer::GetWhiteTexture(), {30,30});
			if (ImGui::BeginPopupContextItem("RemoveTexture")) {
				ImGui::EndPopup();
			}
			if (ImGui::BeginPopup("RemoveTexture")) {
				if (ImGui::MenuItem("Remove Texture")) {
					spriteComp.Texture = nullptr;
				}
				ImGui::EndPopup();
			}

			if (ImGui::BeginDragDropTarget()) {
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(EnumReflection::EnumString(AssetType::Texture).c_str())) {
					uint64_t Data = *(const uint64_t*)payload->Data;
					if (AssetManager::HasAsset(Data))
					{
						spriteComp.Texture =AssetManager::GetAsset<Texture2D>(Data);
					}
				}
				ImGui::EndDragDropTarget();
			}
			ImGui::SameLine();
			ImGui::ColorEdit4("##Colour", glm::value_ptr(spriteComp.Colour));
		});
		DrawComponents<NativeScriptComponent>("Native Script", entity, [](NativeScriptComponent& NativeScriptComp) {
			ExternalAPI::ImGUIAPI::TextBar("Sript", NativeScriptComp.GetScriptName());
		});
		DrawComponents<SkyLightComponent>("Sky Light", entity, [](SkyLightComponent& skylight) {
			if (skylight.DynamicSky == false)
			{

				bool hasImage = false;
				if (AssetManager::HasAsset(skylight.Image) && skylight.Environment != nullptr)
					hasImage = true;

				if (hasImage)
				{
					ExternalAPI::ImGUIAPI::TextBar("HDR Map", AssetManager::GetAssetInfo(skylight.Image).GetName());
				}
				else
					ExternalAPI::ImGUIAPI::TextBar("HDR Map");
				if (ImGui::BeginPopupContextItem("Remove HDR"))
				{
					ImGui::EndPopup();
				}
				if (ImGui::BeginPopup("Remove HDR"))
				{
					if (ImGui::MenuItem("Remove HDR"))
					{
						skylight.RemoveImage();
					}
					ImGui::EndPopup();
				}

				if (ImGui::BeginDragDropTarget())
				{
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(EnumReflection::EnumString(AssetType::Texture).c_str()))
					{
						uint64_t Data = *(const uint64_t*)payload->Data;
						if (AssetManager::HasAsset(Data))
						{
							skylight.LoadMap(Data);
						}
					}
					ImGui::EndDragDropTarget();
				}
			}
			if(skylight.Environment)

				ImGui::SliderFloat("SkyBoxLoad", &skylight.SkyBoxLoad, 0, skylight.Environment->PrefilterMap->GetMipLevelCount());
			else
				ImGui::SliderFloat("SkyBoxLoad", &skylight.SkyBoxLoad, 0, 11);
			ImGui::DragFloat("Intensity", &skylight.Intensity,0.25,0,1000,"%.3f",ImGuiSliderFlags_AlwaysClamp);
			ImGui::DragFloat("Rotation", &skylight.MapRotation, 0.25);
			if(ExternalAPI::ImGUIAPI::CheckBox("DynamicSky", &skylight.DynamicSky));
			if (skylight.DynamicSky)
			{
				ImGui::DragFloat("Turbidity", &skylight.Turbidity, 0.01,1.8f,Math::GetMaxType<float>(),"%.3f", ImGuiSliderFlags_AlwaysClamp);
				ImGui::DragFloat("Azimuth", &skylight.Azimuth, 0.01);
				ImGui::DragFloat("Inclination", &skylight.Inclination, 0.01);
			}
			
			ImGui::ColorEdit3("TintColor", skylight.ColorTint.GetValue_Ptr());
		});
		DrawComponents<DirectionalLightComponent>("Directonal Light", entity, [](DirectionalLightComponent& drl) {
			ImGui::ColorEdit3("Ambient", drl.Color.GetValue_Ptr());
			ImGui::DragFloat("Intensity", &drl.Intensity, 0.01, 0.0f, 100);

			ImGui::Checkbox("CastShadows", &drl.CastShadow);
			if (drl.CastShadow)
			{
				ImGui::SliderFloat("ShadowStrength", &drl.ShadowStrength, 0, 1, "%.3f", ImGuiSliderFlags_AlwaysClamp);
				ImGui::Checkbox("CastSoftShadows", &drl.CastSoftShadow);

				if (drl.CastSoftShadow)
				{
					ImGui::SliderFloat("ShadowSoftness", &drl.ShadowSoftness, 0, 1, "%.3f", ImGuiSliderFlags_AlwaysClamp);
				}
			}
		});

		DrawComponents<PointLightComponent>("Point Light", entity, [](PointLightComponent& pl) {
			
			ImGui::ColorEdit3("Color", glm::value_ptr(pl.Color));
			ImGui::DragFloat("Intensity", &pl.Intensity, 0.01, 0.0f, Math::GetMaxType<float>(), "%.3f",ImGuiSliderFlags_AlwaysClamp);
			ImGui::DragFloat("MinRadius", &pl.MinRadius, 0.01, 0.0f, pl.Radius,"%.3f", ImGuiSliderFlags_AlwaysClamp);
			ImGui::DragFloat("Radius", &pl.Radius, 0.01, 0.0f,Math::GetMaxType<float>(), "%.3f",ImGuiSliderFlags_AlwaysClamp);
			ImGui::DragFloat("Falloff", &pl.Falloff, 0.005,0,1.0,"%.3f", ImGuiSliderFlags_AlwaysClamp);
			//ImGui::DragFloat("Constant", &pl.Constant, 0.001);
			//ImGui::DragFloat("Linear", &pl.Linear, 0.001);
			//ImGui::DragFloat("Quadratic", &pl.Quadratic, 0.001);
		});

		DrawComponents<SpotLightComponent>("Spot Light", entity, [](SpotLightComponent& sl) {

			ImGui::ColorEdit3("Color", glm::value_ptr(sl.Color));
			ImGui::DragFloat("Intensity", &sl.Intensity, 0.01, 0.0f, Math::GetMaxType<float>(), "%.3f", ImGuiSliderFlags_AlwaysClamp);
			ImGui::DragFloat("AngleAttenuation", &sl.AngleAttenuation, 0.25, 0.0f, Math::GetMaxType<float>(), "%.3f");
			ImGui::DragFloat("Falloff", &sl.Falloff, 0.005, 0, Math::GetMaxType<float>(), "%.3f");
			ImGui::DragFloat("Range", &sl.Range, 0.25, 0, Math::GetMaxType<float>(), "%.3f");
			ImGui::DragFloat("Angle", &sl.Angle, 0.25, 0, 180, "%.3f", ImGuiSliderFlags_AlwaysClamp);
			//ImGui::ColorEdit3("Ambient", sl.Color.GetValue_Ptr());
			//ImGui::DragFloat("Intensity", &sl.Intensity, 0.01, 0.0f, 100);
			//
			//ImGui::DragFloat("Radius", &sl.Radius, 0.01, 0.0f, 100);
			//
			//ImGui::NewLine();
			//ImGui::DragFloat("Constant", &sl.Constant, 0.001);
			//ImGui::DragFloat("Linear", &sl.Linear, 0.001);
			//ImGui::DragFloat("Quadratic", &sl.Quadratic, 0.001);
			//ImGui::DragFloat("CutOff", &sl.CutOff, 0.001);
			//ImGui::DragFloat("Outer-Cutoff", &sl.OuterCutOff, 0.001);
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
			ExternalAPI::ImGUIAPI::EnumCombo("Type", cameraComp.Projection);
			ExternalAPI::ImGUIAPI::CheckBox("Local Rotation", &cameraComp.UseLocalRotation);
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
			DrawVectorControl("Center", cubeCollider.Center);
			DrawVectorControl("Size", cubeCollider.Size, 1.0f);

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
			DrawVectorControl("Center", sphereCollider.Center);

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
			DrawVectorControl("Offset Location", capsuleCollider.Center);

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

		DrawComponents<ParticleSystemComponent>("Particle System",entity, [&](ParticleSystemComponent& particleSystem) {
			const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;
			UI::ScopedStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 0,1.5 });
			bool open = ImGui::TreeNodeEx("PartcileTable", treeNodeFlags, "Particle Table");
			if (!open)return;
			ImGui::SameLine();
			if (ImGui::Button("+"))
			{
				particleSystem.ParticleHandlerTable->SetHandler(particleSystem.ParticleHandlerTable->GetHandlerSize(), nullptr);
			}
			for (auto& [index, particleHandler] : particleSystem.ParticleHandlerTable->GetHandlers())
			{
				UI::ScopedID scope(&index);
				std::string name;
				if (particleHandler !=nullptr &&AssetManager::HasAsset(particleHandler->GetParticleSystem()))
				{
					name = AssetManager::GetAssetInfo(particleHandler->GetParticleSystem()).GetName();
				}
				else
				{
					name = "null";
				}
				ExternalAPI::ImGUIAPI::TextBar(fmt::format("Index {}", index), name);
				if (particleHandler != nullptr)
				{
					ImGui::SameLine();
					ImGui::Checkbox("Visible", &particleHandler->Visible);
				}
				if (ImGui::BeginDragDropTarget())
				{
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(EnumReflection::EnumString<AssetType>(AssetType::ParticleSystem).c_str()))
					{
						uint64_t Data = *(const uint64_t*)payload->Data;
						if (AssetManager::HasAsset(Data))
						{
							auto par = AssetManager::GetAsset<ParticleSystem>(Data);
							Count<ParticleHandler> handler = Count<ParticleHandler>::Create(par);
							particleSystem.ParticleHandlerTable->SetHandler(index, handler);
						}
					}
					ImGui::EndDragDropTarget();
				}
			}
			ImGui::TreePop();
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
			if (m_ActiveWorld->IsPlaying() == false)
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
							SET_FEILD_DEFAULT(Int8_t, int8_t);
							SET_FEILD_DEFAULT(Int16_t, int16_t);
							SET_FEILD_DEFAULT(Int32_t, int32_t);
							SET_FEILD_DEFAULT(Int64_t, int64_t);
							SET_FEILD_DEFAULT(Uint8_t, uint8_t);
							SET_FEILD_DEFAULT(Uint16_t, uint16_t);
							SET_FEILD_DEFAULT(Uint32_t, uint32_t);
							SET_FEILD_DEFAULT(Uint64_t, uint64_t);
							SET_FEILD_DEFAULT(Vector2, Vector2);
							SET_FEILD_DEFAULT(Vector3, Vector);
							SET_FEILD_DEFAULT(Vector4, Vector4);

							case ScriptFieldType::Enum:
								{
									scriptInstance.SetValueRaw(entityClass->GetFieldDefaultValueRaw(fieldName).data()); // use the largest type to set the enum as 0
									break;
								}
							// these typs are clses so its weird getting ther data
							case ScriptFieldType::Entity:
							case ScriptFieldType::Prefab:
							case ScriptFieldType::Texture:
								scriptInstance.SetValue<uint64_t>(0);
								break;
							default:
								//PF_CORE_ASSERT(false);
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

							SET_FIELD_NUMERICAL_VALUE(Int8_t, int8_t, fieldName, ImGuiDataType_S8);
							SET_FIELD_NUMERICAL_VALUE(Int16_t, int16_t, fieldName, ImGuiDataType_S16);
							SET_FIELD_NUMERICAL_VALUE(Int32_t, int32_t, fieldName, ImGuiDataType_S32);
							SET_FIELD_NUMERICAL_VALUE(Int64_t, int64_t, fieldName, ImGuiDataType_S64);

							SET_FIELD_NUMERICAL_VALUE(Uint8_t, uint8_t, fieldName, ImGuiDataType_S8);
							SET_FIELD_NUMERICAL_VALUE(Uint16_t, uint16_t, fieldName, ImGuiDataType_U16);
							SET_FIELD_NUMERICAL_VALUE(Uint32_t, uint32_t, fieldName, ImGuiDataType_U32);
							SET_FIELD_NUMERICAL_VALUE(Uint64_t, uint64_t, fieldName, ImGuiDataType_U64);
							case ScriptFieldType::Enum:
								{
									const std::string enumTypeName = ScriptEngine::GetFieldEnumName(field);
									if(!ScriptEngine::GetEnumClasses().contains(enumTypeName))
										break;

									switch (ScriptEngine::GetEnumClasses().at(enumTypeName).first)
									{
										Set_FIELD_ENUM_NUMERICAL_VALUE(Int8_t, int8_t);
										Set_FIELD_ENUM_NUMERICAL_VALUE(Int16_t, int16_t);
										Set_FIELD_ENUM_NUMERICAL_VALUE(Int32_t, int32_t);
										Set_FIELD_ENUM_NUMERICAL_VALUE(Int64_t, int64_t);

										Set_FIELD_ENUM_NUMERICAL_VALUE(Uint8_t, uint8_t);
										Set_FIELD_ENUM_NUMERICAL_VALUE(Uint16_t, uint16_t);
										Set_FIELD_ENUM_NUMERICAL_VALUE(Uint32_t, uint32_t);
										Set_FIELD_ENUM_NUMERICAL_VALUE(Uint64_t, uint64_t);
										default:
											break;
									}
									break;
								}
							case ScriptFieldType::Texture:
								{
									ImGui::Text(fieldName.c_str());
									ImGui::SameLine();
									if (AssetManager::HasAsset(scriptField.GetValue<uint64_t>()))
									{
										auto texture = AssetManager::GetAsset<Texture2D>(scriptField.GetValue<uint64_t>());
										UI::Image(texture->GetImage(), { 30,30 });
										//ImGui::Image((ImTextureID)Renderer::GetWhiteTexture()->GetImage().SourceImage, { 30,30 });
									}
									else
									{
										UI::Image(Renderer::GetWhiteTexture(), { 30,30 });
									}
									if (ImGui::BeginDragDropTarget())
									{
										if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(EnumReflection::EnumString(AssetType::Texture).c_str()))
										{
											uint64_t Data = *(const uint64_t*)payload->Data;
											if (AssetManager::HasAsset(Data))
											{
												scriptField.SetValue(Data); 
											}
										}
										ImGui::EndDragDropTarget();
									}
									break;
								}
							case ScriptFieldType::Bool:
								{
									bool var = scriptField.GetValue<bool>();
									ExternalAPI::ImGUIAPI::CheckBox(fieldName, &var);
									scriptField.SetValue<bool>(var);
									break;
								}
							case ScriptFieldType::Prefab:
								{
									if (AssetManager::HasAsset(scriptField.GetValue<uint64_t>()))
									{
										auto assetInfo =AssetManager::GetAssetInfo(scriptField.GetValue<uint64_t>());
										ExternalAPI::ImGUIAPI::TextBar(field.Name, assetInfo.GetName());

									}
									else
									{
										scriptField.SetValue<uint64_t>(0);

										ExternalAPI::ImGUIAPI::TextBar(field.Name, "null (Prefab)");
									}
									if (ImGui::BeginDragDropTarget())
									{
										if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(EnumReflection::EnumString(AssetType::Prefab).c_str()))
										{
											UUID prefabId = *(UUID*)payload->Data;
											if (AssetManager::HasAsset(prefabId))
												scriptField.SetValue<uint64_t>(prefabId);
										}
										ImGui::EndDragDropTarget();
									}
									break;
								}
							case ScriptFieldType::Entity:
								{
									if (m_ActiveWorld->HasEntity(scriptField.GetValue<uint64_t>()))
									{
										Entity ent = m_ActiveWorld->GetEntity(scriptField.GetValue<uint64_t>());
										ExternalAPI::ImGUIAPI::TextBar(field.Name, ent.GetName());
										scriptField.SetValue<uint64_t>(ent.GetUUID().Get());
									}
									else
									{
										scriptField.SetValue<uint64_t>(0);
										ExternalAPI::ImGUIAPI::TextBar(field.Name, "null (Entity)");
									}
									if (ImGui::BeginDragDropTarget())
									{
										if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("SceneEntity"))
										{
											Entity ent = *(Entity*)payload->Data;
											scriptField.SetValue<uint64_t>(ent.GetUUID());
										}
										ImGui::EndDragDropTarget();
									}
									break;
								}
							default:
								PF_CORE_ASSERT(false);
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
								
							SET_FIELD_NUMERICAL_VALUE_RUNTIME(Int8_t, int8_t, name, ImGuiDataType_S8);
							SET_FIELD_NUMERICAL_VALUE_RUNTIME(Int16_t, int16_t, name, ImGuiDataType_S16);
							SET_FIELD_NUMERICAL_VALUE_RUNTIME(Int32_t, int32_t, name, ImGuiDataType_S32);
							SET_FIELD_NUMERICAL_VALUE_RUNTIME(Int64_t, int64_t, name, ImGuiDataType_S64);
								
							SET_FIELD_NUMERICAL_VALUE_RUNTIME(Uint8_t, uint8_t, name, ImGuiDataType_S8);
							SET_FIELD_NUMERICAL_VALUE_RUNTIME(Uint16_t, uint16_t, name, ImGuiDataType_U16);
							SET_FIELD_NUMERICAL_VALUE_RUNTIME(Uint32_t, uint32_t, name, ImGuiDataType_U32);
							SET_FIELD_NUMERICAL_VALUE_RUNTIME(Uint64_t, uint64_t, name, ImGuiDataType_U64);

							case ScriptFieldType::Enum:
								{
									const std::string enumTypeName = ScriptEngine::GetFieldEnumName(field);
									if (!ScriptEngine::GetEnumClasses().contains(enumTypeName))
										break;

									switch (ScriptEngine::GetEnumClasses().at(enumTypeName).first)
									{
										Set_FIELD_ENUM_NUMERICAL_VALUE_RUNTIME(Int8_t, int8_t);
										Set_FIELD_ENUM_NUMERICAL_VALUE_RUNTIME(Int16_t, int16_t);
										Set_FIELD_ENUM_NUMERICAL_VALUE_RUNTIME(Int32_t, int32_t);
										Set_FIELD_ENUM_NUMERICAL_VALUE_RUNTIME(Int64_t, int64_t);

										Set_FIELD_ENUM_NUMERICAL_VALUE_RUNTIME(Uint8_t, uint8_t);
										Set_FIELD_ENUM_NUMERICAL_VALUE_RUNTIME(Uint16_t, uint16_t);
										Set_FIELD_ENUM_NUMERICAL_VALUE_RUNTIME(Uint32_t, uint32_t);
										Set_FIELD_ENUM_NUMERICAL_VALUE_RUNTIME(Uint64_t, uint64_t);
										default:
											break;
									}
									
									break;
								}
							case ScriptFieldType::Texture:
								{
									ImGui::Text(field.Name.c_str());
									ImGui::SameLine();
									if (AssetManager::HasAsset(instance->GetFieldValue<uint64_t>(name)))
									{
										auto texture = AssetManager::GetAsset<Texture2D>(instance->GetFieldValue<uint64_t>(name));
										UI::Image(texture, { 30,30 });
										//ImGui::Image((ImTextureID)texture->GetImage().SourceImage, { 30,30 });
									}
									else
									{
										UI::Image(Renderer::GetWhiteTexture(), { 30,30 });
									}
									if (ImGui::BeginDragDropTarget())
									{
										if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(EnumReflection::EnumString(AssetType::Texture).c_str()))
										{
											uint64_t Data = *(const uint64_t*)payload->Data;
											if (AssetManager::HasAsset(Data))
											{
												instance->SetFieldValue(name,Data);
											}
										}
										ImGui::EndDragDropTarget();
									}
									break;
								}
							case ScriptFieldType::Prefab:
								{
									if (AssetManager::HasAsset(instance->GetFieldValue<uint64_t>(name)))
									{
										auto assetInfo = AssetManager::GetAssetInfo(instance->GetFieldValue<uint64_t>(name));
										ExternalAPI::ImGUIAPI::TextBar(field.Name, assetInfo.GetName());

									}
									else
									{
										instance->SetFieldValue<uint64_t>(name,0);

										ExternalAPI::ImGUIAPI::TextBar(field.Name, "null (Prefab)");
									}
									if (ImGui::BeginDragDropTarget())
									{
										if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(EnumReflection::EnumString(AssetType::Prefab).c_str()))
										{
											UUID prefabId = *(UUID*)payload->Data;
											if (AssetManager::HasAsset(prefabId))
												instance->SetFieldValue<uint64_t>(name, prefabId);
										}
										ImGui::EndDragDropTarget();
									}
									break;
								}
							case ScriptFieldType::Entity:
								{
									if (m_ActiveWorld->HasEntity(instance->GetFieldValue<uint64_t>(name)))
									{
										Entity ent = m_ActiveWorld->GetEntity(instance->GetFieldValue<uint64_t>(name));
										ExternalAPI::ImGUIAPI::TextBar(field.Name, ent.GetName());

									}
									else
									{
										instance->SetFieldValue<uint64_t>(name,0);
										ExternalAPI::ImGUIAPI::TextBar(field.Name, "null (Entity)");
									}
									if (ImGui::BeginDragDropTarget())
									{

										if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("SceneEntity"))
										{
											UUID entityID = *(UUID*)payload->Data;
											instance->SetFieldValue<uint64_t>(name, entityID);
										}
										ImGui::EndDragDropTarget();
									}
									break;
								}
							default:
								break;
						}
					}
					ImGui::TreePop();
				}
			}
		});

		DrawComponents<TextComponent>("Text Component", entity, [](TextComponent& textComponent) {
			//ImGui::InputTextMultiline("Text", (char*)textComponent.Text.c_str(), textComponent.Text.capacity() + 1);
			UI::AttributeInputTextMultiline("ither", textComponent.Text,0);
			ImGui::ColorEdit4("##Colour", glm::value_ptr(textComponent.Colour));

			ImGui::DragFloat("Kernng", &textComponent.Kerning, 0.025);
			ImGui::DragFloat("Line Spacing", &textComponent.LineSpacing, 0.025);
		});

		DrawComponents<PlayerInputComponent>("Player Input", entity, [](PlayerInputComponent& player) {
			ExternalAPI::ImGUIAPI::EnumCombo("Player", player.InputPlayer);
			if (AssetManager::HasAsset(player.Player))
			{
				auto assetInfo = AssetManager::GetAssetInfo(player.Player);
				ExternalAPI::ImGUIAPI::TextBar("PlayerPrefab", assetInfo.GetName());
			}
			else
			{
				ExternalAPI::ImGUIAPI::TextBar("PlayerPrefab", "null (Prefab)");
			}
			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(EnumReflection::EnumString(AssetType::Prefab).c_str()))
				{
					UUID prefabId = *(UUID*)payload->Data;
					if (AssetManager::HasAsset(prefabId))
						player.Player = AssetManager::GetAsset<Prefab>(prefabId);
				}
				ImGui::EndDragDropTarget();
			}
		});


		DrawComponents<PlayerHUDComponent>("Player HUD", entity, [](PlayerHUDComponent& playerHud){
			const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;
			UI::ScopedStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 0,1.5 });
			bool open = ImGui::TreeNodeEx("HudCadfasd", treeNodeFlags, "HUd Table");
			if (!open)return;
			ImGui::SameLine();
			if(ImGui::Button("+")) {
				playerHud.HudTable->SetUI(playerHud.HudTable->GetPanelSize(), nullptr);
			}
			for (auto& [index, hud] : playerHud.HudTable->GetPanels())
			{
				UI::ScopedID scope(&index);
				std::string name = hud != nullptr ? hud->Name : "null";
				ExternalAPI::ImGUIAPI::TextBar(fmt::format("Index {}", index), name);
				if (hud != nullptr)
				{
					ImGui::SameLine();
					ImGui::Checkbox("Visible", &hud->Visible);
				}
				if (ImGui::BeginDragDropTarget())
				{
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(EnumReflection::EnumString<AssetType>(AssetType::UIPanel).c_str()))
					{
						uint64_t Data = *(const uint64_t*)payload->Data;
						if (AssetManager::HasAsset(Data))
						{
							auto ui = AssetManager::GetAsset<UIPanel>(Data);
							playerHud.HudTable->SetUI(index, ui);
						}
					}
					ImGui::EndDragDropTarget();
				}
			}
			ImGui::TreePop();
		});


		DrawComponents<AudioComponent>("Audio", entity, [](AudioComponent& audio) 
		{
			if (AssetManager::HasAsset(audio.AudioAsset))
			{

				ExternalAPI::ImGUIAPI::TextBar("Audio", AssetManager::GetAssetInfo(audio.AudioAsset).GetName());
			}
			else
			{
				ExternalAPI::ImGUIAPI::TextBar("Audio", "null");
			}

			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(EnumReflection::EnumString<AssetType>(AssetType::Audio).c_str()))
				{
					uint64_t Data = *(const uint64_t*)payload->Data;
					if (AssetManager::HasAsset(Data))
					{
						audio.AudioAsset = Data;
					}
				}
				ImGui::EndDragDropTarget();
			}

			UI::AttributeSlider("VolumeMultiplier", audio.VolumeMultiplier, 0,1);
			UI::AttributeSlider("PitchMultiplier", audio.PitchMultiplier,0,24);

			ExternalAPI::ImGUIAPI::CheckBox("Looping", &audio.Looping);
			ExternalAPI::ImGUIAPI::CheckBox("Play On Awake", &audio.PlayOnAwake);
			ExternalAPI::ImGUIAPI::CheckBox("Specialization", &audio.SpatializationEnabled);

			if (audio.SpatializationEnabled == false)return;

			if (auto [changed,newValue] = UI::EnumCombo("Attenuation Model", EnumReflection::GetNames<AttenuationModel>(), EnumReflection::EnumString(audio.AttenuationModel)); changed)
			{
				audio.AttenuationModel = EnumReflection::StringEnum<AttenuationModel>(newValue);
			}

			UI::AttributeDrag("Min Gain", audio.MinGain, 0.025f, 0, 1);
			UI::AttributeDrag("Max Gain", audio.MaxGain, 0.025f, 0, 1);

			UI::AttributeDrag("Min Distance", audio.MinDistance, 0.025f, 0, audio.MaxDistance);
			UI::AttributeDrag("Max Distance", audio.MaxDistance, 0.025f, audio.MinDistance, Math::GetMaxType<float>());

			{
				float degrees= glm::degrees(audio.ConeInnerAngleInRadians);
				if (UI::AttributeSlider("Cone Inner Angle", degrees,0, 360))
					audio.ConeInnerAngleInRadians = glm::radians(degrees);

				degrees = glm::degrees(audio.ConeOuterAngleInRadians);
				if (UI::AttributeSlider("Cone Outer Angle", degrees, 0, 360))
					audio.ConeOuterAngleInRadians = glm::radians(degrees);

				UI::AttributeSlider("ConeOuterGain", audio.ConeOuterGain, 0, 1);
			}

			UI::AttributeSlider("Doppler Factor", audio.DopplerFactor, 0, 1);
			UI::AttributeSlider("RollOff", audio.Rolloff,0, 1);
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
	void SceneHierachyPanel::DrawVectorControl(const std::string& UniqeLabel, glm::vec3& Vec, float ResetValue, float columnWidth, float Speed)
	{
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
		if (ImGui::Button("X", buttonSize))
		{
			Vec.x = ResetValue;
		}
		ImGui::PopFont();

		ImGui::SameLine();
		ImGui::DragFloat("##x", &Vec.x, Speed, 0, 0, "%.3f"); // does not show ## as label
		ImGui::PopItemWidth();
		ImGui::SameLine();
		ImGui::PopStyleColor(3);

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.0f,.5f,0.0f,1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 1.0f,0.5f,0.0f,1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.0f,.5f,0.0f,1.0f });

		ImGui::PushFont(boldFont);
		if (ImGui::Button("Y", buttonSize))
		{
			Vec.y = ResetValue;
		}
		ImGui::PopFont();

		ImGui::SameLine();
		ImGui::DragFloat("##Y", &Vec.y, Speed, 0, 0, "%.3f"); // does not show ## as label
		ImGui::PopItemWidth();
		ImGui::SameLine();
		ImGui::PopStyleColor(3);

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.0f,0.0f,1.0f,1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 1.0f,0.5f,0.0f,1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.0f,0.0f,1.0f,1.0f });

		ImGui::PushFont(boldFont);
		if (ImGui::Button("Z", buttonSize))
		{
			Vec.z = ResetValue;
		}
		ImGui::PopFont();

		ImGui::SameLine();
		ImGui::DragFloat("##Z", &Vec.z, Speed, 0, 0, "%.3f"); // does not show ## as label
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
	void SceneHierachyPanel::DrawVector2Control(const std::string& UniqeLabel, glm::vec2& Vec, float ResetValue, float columnWidth)
	{
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
		if (ImGui::Button("X", buttonSize))
		{
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
		if (ImGui::Button("Y", buttonSize))
		{
			Vec.y = ResetValue;
		}
		ImGui::PopFont();

		ImGui::SameLine();
		ImGui::DragFloat("##Y", &Vec.y, 0.1f, 0, 0, "%.3f"); // does not show ## as label
		ImGui::PopItemWidth();
		ImGui::SameLine();
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
