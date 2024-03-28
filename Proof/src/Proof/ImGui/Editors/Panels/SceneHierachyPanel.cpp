#include "Proofprch.h"
#include "SceneHierachyPanel.h"
#include "Proof/ImGui/ImGuiLayer.h"
#include "Proof/Scene/Entity.h"
#include "Proof/Scene/Component.h"
#include <imgui.h>
#include <imgui_internal.h>
#include "Proof/Scene/EntitiyComponentSystem/ECS.h"
#include "Proofprch.h"
#include "Proof/Asset/Asset.h"
#include "Proof/Physics/MeshCollider.h"
#include "Proof/Physics/PhysicsEngine.h"
#include "Proof/Project/Project.h"
//#include "ContentBrowserPanel.h"
#include <vector>
#include "Proof/Scene/ExampleSccripts.h"
#include "Proof/Scene/Material.h"
#include "Proof/Scene/Script.h"
#include "Proof/Input/KeyCodes.h"
#include "imgui_internal.h"
#include "Proof/Scene/Component.h"
#include "Proof/Asset/AssetManager.h"
#include <string.h>
#include<magic_enum.hpp>
#include "Proof/Resources/EnumReflection.h"
#include "Proof/Scripting/ScriptEngine.h"
#include "Proof/Imgui/UI.h"
#include "Proof/Imgui/UIHandlers.h"
#include "Proof/Renderer/Renderer.h"
#include "Proof/Scene/Prefab.h"
#include "Proof/Math/Random.h"
#include "Proof/Scripting/ScriptFile.h"
#include "Proof/Utils/StringUtils.h"
#include "Proof/Scripting/ScriptField.h"
#include "Proof/ImGui/SelectionManager.h"
//include those before stdlig.h

#include "misc/cpp/imgui_stdlib.h"
#include "Proof/ProofCore.h"
#include "Proof/Scene/Mesh.h"
#include "Proof/Scripting/ScriptWorld.h"
#include "Proof/ImGui/UiUtilities.h"
#include "Proof/ImGui/UIWidgets.h"

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
			newEntity.AddComponent<MeshColliderComponent>().ColliderID = AssetManager::GetDefaultAsset(DefaultRuntimeAssets::Type)->GetID();\
			newEntity.AddComponent<RigidBodyComponent>();\
		}
	template<class T>
	static void AddComponentGui(Entity entity, const std::string& name) {
		if (ImGui::MenuItem(name.c_str()))
		{
			entity.AddComponent<T>();
			ImGui::CloseCurrentPopup();
		}
	};

	template<class T,class UIFunction>
	static void AddComponentGuiButton(Entity entity, const std::string& name, UIFunction function) {
		if (ImGui::MenuItem(name.c_str()))
		{
			entity.AddComponent<T>();
			function(entity, entity.GetComponent<T>());
			ImGui::CloseCurrentPopup();
		}
	};
	SceneHierachyPanel::SceneHierachyPanel(bool IsWorld , UUID prefabID)
		:
		m_IsWorld(IsWorld), m_PrefabID(prefabID)
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

			m_IsFocused = ImGui::IsWindowFocused();
			m_IsHovered = ImGui::IsWindowHovered();
			/*
			auto &a =m_ActiveWorld->m_Registry.view<>();
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

				for (auto [id,entity] : m_ActiveWorld->GetEntities())
				{
					if (entity.HasParent() == false)
						DrawEntityNode(entity);
				}

				if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered() && ImGui::IsAnyItemHovered() == false) 
				{
					if (m_IsWorld)
						SelectionManager::DeselectAll();
					else
						AssetSelectionManager::DeselectAll(AssetSelectionContext::Prefab, m_PrefabID);
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
					if (m_IsWorld)
					{

						if (SelectionManager::GetSelectionCount(SelectionContext::Scene) > 0)
						{
							auto entity = m_ActiveWorld->GetEntity(SelectionManager::GetSelections(SelectionContext::Scene).front());
							DrawComponent(entity);
						}
					}
					else
					{
						if (AssetSelectionManager::HasSelections(AssetSelectionContext::Prefab, m_PrefabID))
						{
							auto entity = m_ActiveWorld->GetEntity(AssetSelectionManager::GetSelections(AssetSelectionContext::Prefab, m_PrefabID).front());
							DrawComponent(entity);
						}

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
				newEntity.GetComponent<TransformComponent>().SetRotationEuler(glm::radians(glm::vec3{ 90.0f, 0.0f, 0.0f }));
			}
			if (ImGui::MenuItem("Directional")) {
				newEntity = m_ActiveWorld->CreateEntity("Directional Light");
				newEntity.AddComponent<DirectionalLightComponent>();
				newEntity.GetComponent<TransformComponent>().SetRotationEuler(glm::radians(glm::vec3{ 80.0f, 10.0f, 0.0f }));

			}
			if (ImGui::MenuItem("SkyLight"))
			{
				newEntity = m_ActiveWorld->CreateEntity("Sky Light");
				newEntity.AddComponent<SkyLightComponent>();
				//https://github.com/TKscoot/Ivy/blob/master/projects/Ivy/source/scene/renderpasses/skymodels/HosekWilkieSkyModel.cpp#L66
				newEntity.GetComponent<TransformComponent>().SetRotationEuler(glm::radians(glm::vec3(2, 1,0)));
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Mesh")) 
		{
			if (ImGui::MenuItem("Cube"))
			{
				newEntity = m_ActiveWorld->CreateEntity("Cube"); 
				newEntity.AddComponent<MeshComponent>().SetMesh(AssetManager::GetDefaultAsset(DefaultRuntimeAssets::Cube)->GetID());
				newEntity.AddComponent<BoxColliderComponent>();
				newEntity.AddComponent<RigidBodyComponent>();
			}

			if (ImGui::MenuItem("Sphere"))
			{
				newEntity = m_ActiveWorld->CreateEntity("Sphere");
				newEntity.AddComponent<MeshComponent>().SetMesh(AssetManager::GetDefaultAsset(DefaultRuntimeAssets::Sphere)->GetID());
				newEntity.AddComponent<SphereColliderComponent>();
				newEntity.AddComponent<RigidBodyComponent>();
			}

			if (ImGui::MenuItem("Capsule"))
			{
				newEntity = m_ActiveWorld->CreateEntity("Capsule");
				newEntity.AddComponent<MeshComponent>().SetMesh(AssetManager::GetDefaultAsset(DefaultRuntimeAssets::Capsule)->GetID());
				newEntity.AddComponent<CapsuleColliderComponent>();
				newEntity.AddComponent<RigidBodyComponent>();
			}
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
		ImGuiTreeNodeFlags flags;	
		if(m_IsWorld)
			flags= ((SelectionManager::IsSelected(SelectionContext::Scene, entity.GetUUID()) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow);
		else
			flags = ((AssetSelectionManager::IsSelected(AssetSelectionContext::Prefab,m_PrefabID, entity.GetUUID()) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow);
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
		if ( ImGui::IsItemClicked() && ImGui::IsKeyDown((ImGuiKey)KeyBoardKey::E) ==false) 
		{
			if (m_IsWorld)
			{

				SelectionManager::DeselectAll();
				SelectionManager::Select(SelectionContext::Scene, entity.GetUUID());
			}
			else
			{
				AssetSelectionManager::DeselectAll(AssetSelectionContext::Prefab, m_PrefabID);
				AssetSelectionManager::Select(AssetSelectionContext::Prefab, m_PrefabID, entity.GetUUID());
			}
		}
		//if (ImGui::BeginPopupContextItem()) {
		if(ImGui::BeginPopupContextItem("Entity Settings")) {
			ImGui::EndPopup();
		}
		if (ImGui::BeginPopup("Entity Settings")) 
		{

			if (ImGui::BeginMenu("Child Entity")) {
				bool temp = CreateEntityMenu(entity);
				// not setting to opne because 
				// if it is already opened what if we
				// do not actually create and entity we are closing
				// for no reason

				ImGui::EndMenu();
			}
			if (!m_IsWorld)
			{
				Count<Prefab> prefab = AssetManager::GetAsset<Prefab>(m_PrefabID);
				if (prefab->GetBaseEntity() != entity)
				{
					if (ImGui::MenuItem("Delete"))
					{
						m_ActiveWorld->DeleteEntity(entity, true);

						if (m_IsWorld)
						{
							SelectionManager::DeselectAll();
						}
						else
						{
							AssetSelectionManager::DeselectAll(AssetSelectionContext::Prefab, m_PrefabID);
						}
						if (opened) {
							ImGui::EndPopup();
							ImGui::TreePop();
							ImGui::PopID();
							return;
						}
					}
				}
			}
			else
			{

				if (ImGui::MenuItem("Delete"))
				{
					m_ActiveWorld->DeleteEntity(entity, true);

					if (m_IsWorld)
					{
						SelectionManager::DeselectAll();
					}
					else
					{
						AssetSelectionManager::DeselectAll(AssetSelectionContext::Prefab, m_PrefabID);
					}
					if (opened) {
						ImGui::EndPopup();
						ImGui::TreePop();
						ImGui::PopID();
						return;
					}
				}
			}

			if (m_ActiveWorld->HasEntity(entity.GetUUID()))
			{
				if (ImGui::MenuItem("Duplicate"))
				{
					auto newEntity = m_ActiveWorld->CreateEntity(entity);
					if (m_IsWorld)
					{
						SelectionManager::DeselectAll();
						SelectionManager::Select(SelectionContext::Scene, newEntity.GetUUID());
					}
					else
					{
						AssetSelectionManager::DeselectAll(AssetSelectionContext::Prefab, m_PrefabID);
						AssetSelectionManager::Select(AssetSelectionContext::Prefab, m_PrefabID,newEntity.GetUUID());
					}
				}
			}
			ImGui::EndPopup();
		}
		//if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0) && m_SelectedEntity) {
		//	//Editore3D::Get()->m_EditorCamera.SetPosition( m_ActiveWorld->GetWorldSpaceLocation(m_SelectedEntity));
		//}
		//if (m_SelectedEntity && ImGui::IsKeyPressed((ImGuiKey)KeyBoardKey::F)) {
		//	//Editore3D::Get()->m_EditorCamera.SetPosition (m_ActiveWorld->GetWorldSpaceLocation(m_SelectedEntity));
		//}

		if (opened) {
			for (const UUID& I : entity.GetComponent<HierarchyComponent>().Children) {
				;
				DrawEntityNode(m_ActiveWorld->GetEntity(I));
			}
			ImGui::TreePop();
		}
		ImGui::PopID();
	}
	static bool DynamicMeshUseSlider = false;
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
	struct ConvertRuntimeMaterialToDiskMaterial
	{
		std::string SavePath = "Materials/";

	}ConvertDiskMaterial;
	static void ConvertDiskMaterialFunc(AssetID id)
	{
		if (!AssetManager::HasAsset(id))
			return;
		if (AssetManager::IsDefaultAsset(id))
			return;

		const auto assetInfo = AssetManager::GetAssetInfo(id);

		if (assetInfo.RuntimeAsset == false)
			return;

		if(ImGui::MenuItem("ConvertToDiskMaterial"))
		{
			UI::ShowMessageBox("ConvertMaterialToDisk", [id, assetInfo]()
				{


					ImGui::Text(Project::GetActive()->GetProjectDirectory().filename().string().c_str());

					if (ConvertDiskMaterial.SavePath == "Materials/")
						ConvertDiskMaterial.SavePath += assetInfo.GetName();

					UI::AttributeInputText("MaterialName", ConvertDiskMaterial.SavePath);
					if (ImGui::Button("Create"))
					{
						std::filesystem::path savedPath = Project::GetActive()->GetAssetDirectory() / ConvertDiskMaterial.SavePath;
						savedPath += Utils::GetAssetExtensionString(AssetType::Material);
						if (!FileSystem::Exists(savedPath.parent_path()))
							FileSystem::CreateDirectory(savedPath.parent_path());

						savedPath = FileSystem::GenerateUniqueFileName(savedPath);
						AssetManager::ConvertRuntimeToDiskAsset(id, savedPath);

						ConvertDiskMaterial = {};
						ImGui::CloseCurrentPopup();

					}
					ImGui::SameLine();

					if (ImGui::Button("Cancel"))
					{
						ConvertDiskMaterial = {};
						ImGui::CloseCurrentPopup();
					}
				});
		}

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
			AddComponentGui<DynamicMeshComponent>(entity, "DynamicMesh");
			AddComponentGui<SpriteComponent>(entity, "Sprite");
			AddComponentGui<NativeScriptComponent>(entity, "Native Script");

			AddComponentGui<SkyLightComponent>(entity, "Sky Light");
			AddComponentGui<DirectionalLightComponent>(entity, "Directional Light");
			AddComponentGui<PointLightComponent>(entity, "Point Light");
			AddComponentGui<SpotLightComponent>(entity, "Spot Light");
			AddComponentGui<CameraComponent>(entity, "Camera");

			AddComponentGui<BoxColliderComponent>(entity, "Box Collider");
			AddComponentGui<SphereColliderComponent>(entity, "Sphere Collider");
			AddComponentGui<CapsuleColliderComponent>(entity, "Capsule Collider");
			AddComponentGuiButton<MeshColliderComponent>(entity, "Mesh Collider", [](Entity entity, MeshColliderComponent& meshColliderComp)
				{
					PhysicsEngine::GetOrCreateColliderAsset(entity, meshColliderComp);
				});

			AddComponentGui<RigidBodyComponent>(entity, "Rigid Body");
			AddComponentGui<CharacterControllerComponent>(entity, "Character Controller");

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
		DrawComponents<MeshComponent>("Mesh", entity, [](MeshComponent& meshComp) 
		{
			UI::BeginPropertyGrid();

			UI::PropertyAssetReferenceSettings assetRefSettings;
			assetRefSettings.AssetMemoryTypes = UI::UIMemoryAssetTypes::Default;
			AssetID id = meshComp.m_MeshID;
			if (UI::AttributeAssetReference("Mesh", AssetType::Mesh, id, assetRefSettings))
			{
				if (id == 0)
					meshComp.RemoveMesh();
				else
					meshComp.SetMesh(id);
			}
			UI::AttributeBool("Visible", meshComp.Visible);

			ImGui::Separator();
			UI::EndPropertyGrid();

			if (UI::AttributeTreeNode("MaterialTable"))
			{
				UI::BeginPropertyGrid();

				for (auto& [index, material] : meshComp.MaterialTable->GetMaterials())
				{
					AssetID materialID = material->GetID();
					UI::PropertyAssetReferenceSettings assetSettings;
					assetSettings.OnRightClick = ConvertDiskMaterialFunc;
					if (UI::AttributeAssetReference(fmt::format("Index {}", index), AssetType::Material, materialID, assetSettings))
					{
						if (materialID == 0)
						{
							auto mesh = meshComp.GetMesh();
							if (mesh)
							{
								materialID = mesh->GetMeshSource()->GetMaterials()->GetMaterial(index)->GetID();
							}
						}
						meshComp.MaterialTable->SetMaterial(index, AssetManager::GetAsset<Material>(materialID));
					}
				}
				UI::EndPropertyGrid();

				UI::EndTreeNode();
			}
		});

		DrawComponents<DynamicMeshComponent>("Dynamic Mesh", entity, [](DynamicMeshComponent& meshComp)
		{

			const uint32_t currentIndexMaterial = meshComp.GetSubMeshMaterialIndex();
			auto mesh = meshComp.GetMesh();
		
			AssetID id = meshComp.m_MeshID;
			UI::BeginPropertyGrid();

			if (UI::AttributeAssetReference("Dynamic Mesh", AssetType::DynamicMesh, id))
			{
				if (id == 0)
					meshComp.RemoveMesh();
				else
					meshComp.SetMesh(id);
			}
			if (mesh)
			{
				UI::AttributeBool("SubMeshIndexUseSlider", DynamicMeshUseSlider);
				if (DynamicMeshUseSlider)
				{
					auto submeshIndex = meshComp.GetSubMeshIndex();
					if (UI::AttributeDrag("SubMesh Index", submeshIndex, 1,0, (uint32_t)mesh->GetMeshSource()->GetSubMeshes().size() - 1))
					{
						meshComp.SetSubMeshIndex(submeshIndex);
					}
				}
				else
				{
					const SubMesh& currentSubMesh = mesh->GetMeshSource()->GetSubMeshes().at( meshComp.GetSubMeshIndex());

					std::vector<std::string> subMeshes;
					subMeshes.resize(mesh->GetSubMeshes().size());

					for (auto& index : mesh->GetSubMeshes())
					{
						subMeshes[index] = mesh->GetMeshSource()->GetSubMeshes().at(index).Name;
					}
					auto [changed, currentSelectIndex,currentSelect] = UI::Combo("SubMesh", subMeshes, currentSubMesh.Name);
					if (changed)
					{
						auto it = std::find(subMeshes.begin(), subMeshes.end(), currentSelect);
						if (it != subMeshes.end())
							meshComp.SetSubMeshIndex(std::distance(subMeshes.begin(), it)); // index
					}
				}

			}
			UI::AttributeBool("Visible", meshComp.Visible);
			ImGui::Separator();
			UI::EndPropertyGrid();


			if (UI::AttributeTreeNode("MaterialTable"))
			{
				UI::BeginPropertyGrid();

				for (auto& [index, material] : meshComp.MaterialTable->GetMaterials())
				{
					AssetID materialID = material->GetID();
					UI::ScopedStyleColor addfs(ImGuiCol_Text, ImVec4(0.0f, .8f, 0.0f, 1.0f), index == currentIndexMaterial);

					UI::PropertyAssetReferenceSettings assetSettings;
					assetSettings.OnRightClick = ConvertDiskMaterialFunc;
					if (UI::AttributeAssetReference(fmt::format("Index {}", index), AssetType::Material, materialID, assetSettings))
					{
						if (materialID == 0)
						{
							auto mesh = meshComp.GetMesh();
							if (mesh)
							{
								materialID = mesh->GetMeshSource()->GetMaterials()->GetMaterial(index)->GetID();
							}
						}
						meshComp.MaterialTable->SetMaterial(index, AssetManager::GetAsset<Material>(materialID));
					}
				}
				UI::EndPropertyGrid();

				UI::EndTreeNode();
			}

		});
		DrawComponents<SpriteComponent>({ "Sprite" }, entity, [](SpriteComponent& spriteComp) {

			UI::BeginPropertyGrid();

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
			UI::AttributeColor("##Colour", spriteComp.Colour);

			UI::EndPropertyGrid();

		});
		DrawComponents<NativeScriptComponent>("Native Script", entity, [](NativeScriptComponent& NativeScriptComp) {
			UI::AttributeTextBar("Sript", NativeScriptComp.GetScriptName());
		});
		DrawComponents<SkyLightComponent>("Sky Light", entity, [](SkyLightComponent& skylight) 
		{

			UI::BeginPropertyGrid();

			Count<Environment> environment = skylight.Environment;

			auto state = environment->GetEnvironmentState();
			if (UI::EnumCombo("EnvironmentState", state))
			{
				switch (state)
				{
				case Proof::EnvironmentState::HosekWilkie:
					environment->Update(environment->GetHosekWilkieDataSkyData());
					break;
				case Proof::EnvironmentState::PreethamSky:
					environment->Update(environment->GetPreethamSkyData());
					break;
				case Proof::EnvironmentState::EnvironmentTexture:
					environment->Update(environment->GetTextureData());
					break;
				default:
					break;
				}
			}
			switch (environment->GetEnvironmentState())
			{
			case EnvironmentState::HosekWilkie:
				{
					auto hosek = environment->GetHosekWilkieDataSkyData();
					UI::EndPropertyGrid();
					UI::AttributeText("Edit Rotation of Transform to change sun Position");
					UI::BeginPropertyGrid();

					UI::AttributeSlider("Turbidity", hosek.Turbidity,2.f, 30.f);
					UI::AttributeSlider("GroundReflectance", hosek.GroundReflectance,0, 1);
					environment->Update(hosek);

				}
			break;
			case EnvironmentState::EnvironmentTexture:
				{
					auto environmentTexture = environment->GetTextureData();
					UI::AttributeAssetReference("HDR Map", AssetType::Texture, environmentTexture.Image);
					UI::AttributeSlider("SkyBoxLoad", skylight.SkyBoxLoad, 0, skylight.Environment->GetPrefilterMap()->GetMipLevelCount());
					environment->Update(environmentTexture);
				}
				break;
			case EnvironmentState::PreethamSky:
				{
					UI::EndPropertyGrid();
					UI::AttributeText("Edit Rotation of Transform to change sun Position");
					UI::BeginPropertyGrid();

					auto pretham = environment->GetPreethamSkyData();
					UI::AttributeSlider("Turbidity", pretham.Turbidity,0, 30);
					environment->Update(pretham);
				}
				break;
			default:
				break;
			}
			UI::AttributeDrag("Intensity", skylight.Intensity, 0.25, 0, 1000);
			UI::AttributeDrag("Rotation", skylight.MapRotation, 0.25);
			UI::AttributeColor("TintColor", skylight.ColorTint);

			UI::EndPropertyGrid();


		});
		DrawComponents<DirectionalLightComponent>("Directonal Light", entity, [](DirectionalLightComponent& drl) {

			UI::BeginPropertyGrid();

			UI::AttributeColor("Ambient", drl.Color);
			UI::AttributeDrag("Intensity", drl.Intensity, 0.01, 0.0f, 500);

			UI::AttributeBool("CastShadows", drl.CastShadow);
			if (drl.CastShadow)
			{
				UI::AttributeSlider("ShadowStrength", drl.ShadowStrength,0,1);
				UI::AttributeBool("CastSoftShadows", drl.CastSoftShadow);

				if (drl.CastSoftShadow)
				{
					UI::AttributeSlider("ShadowSoftness", drl.ShadowSoftness, 0, 1);
				}
			}

			UI::EndPropertyGrid();

		});

		DrawComponents<PointLightComponent>("Point Light", entity, [](PointLightComponent& pl) 
		{
			UI::BeginPropertyGrid();

			UI::AttributeColor("Color", pl.Color);
			UI::AttributeDrag("Intensity", pl.Intensity, 0.01, 0.0f, Math::GetMaxType<float>());
			UI::AttributeDrag("MinRadius", pl.MinRadius, 0.01, 0.0f, pl.Radius);
			UI::AttributeDrag("Radius", pl.Radius, 0.01, 0.0f);
			UI::AttributeDrag("Falloff", pl.Falloff, 0.005,0);
			UI::AttributeBool("Cast Shadows", pl.CastsShadows);

			if (pl.CastsShadows)
			{
				UI::AttributeSlider("ShadowStrength", pl.ShadowStrength, 0, 1);
				UI::AttributeBool("CastSoftShadows", pl.SoftShadows);

				if (pl.SoftShadows)
				{
					UI::AttributeSlider("ShadowSoftness", pl.ShadowSoftness, 0, 1);
				}
			}
			UI::EndPropertyGrid();

		});

		DrawComponents<SpotLightComponent>("Spot Light", entity, [](SpotLightComponent& sl) {
			UI::BeginPropertyGrid();

			UI::AttributeColor("Color", sl.Color);
			UI::AttributeDrag("Intensity", sl.Intensity, 0.01, 0.0f);
			UI::AttributeDrag("Range", sl.Range, 0.25, 0);
			UI::AttributeDrag("Angle", sl.Angle, 0.25, 0, 180);
			UI::AttributeDrag("AngleAttenuation", sl.AngleAttenuation, 0.25, 0.0f);
			UI::AttributeDrag("Falloff", sl.Falloff, 0.005, 0);
			if (sl.CastsShadows)
			{
				UI::AttributeSlider("ShadowStrength", sl.ShadowStrength, 0, 1);
				UI::AttributeBool("CastSoftShadows", sl.SoftShadows);

				if (sl.SoftShadows)
				{
					UI::AttributeSlider("ShadowSoftness", sl.ShadowSoftness, 0, 1);
				}
			}

			UI::EndPropertyGrid();

		});

		DrawComponents<CameraComponent>("Camera", entity, [](CameraComponent& cameraComp) {
			UI::BeginPropertyGrid();

			UI::AttributeSlider("Field ov view", cameraComp.FovDeg, 0, 360);
			UI::AttributeSlider("Near plane", cameraComp.NearPlane, -1, 1);
			UI::AttributeSlider("Far plane", cameraComp.FarPlane, 0, 10000);

			if (ImGui::IsItemHovered()) {
				ImGui::BeginTooltip();
				ImGui::Text("Setting to 0 means you can see any object no matter how far away it is");
				ImGui::EndTooltip();
			}
			UI::EnumCombo< ProjectionType>("ProjectionType", cameraComp.Projection,{ ProjectionType::None });

			UI::AttributeBool("UseLocalRotation", cameraComp.UseLocalRotation);
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
			UI::EndPropertyGrid();

		}, "if nothing visible set roation of z axis to 1");

		DrawComponents<BoxColliderComponent>("Box Collider", entity, [](BoxColliderComponent& cubeCollider) {
			UI::BeginPropertyGrid();

			UI::AttributeBool("IsTrigger", cubeCollider.IsTrigger);
			UI::DrawVec3Control("Size", cubeCollider.Size, glm::vec3{1});
			UI::DrawVec3Control("Center", cubeCollider.Center);

			UI::AttributeAssetReference("Material",AssetType::PhysicsMaterial, cubeCollider.m_PhysicsMaterialPointerID);

			UI::EndPropertyGrid();

		});
		DrawComponents<SphereColliderComponent>("Sphere Collider", entity, [](SphereColliderComponent& sphereCollider) {
			UI::BeginPropertyGrid();

			UI::AttributeBool("IsTrigger",sphereCollider.IsTrigger);
			UI::AttributeDrag("Radius", sphereCollider.Radius, 0.5);
			UI::DrawVec3Control("Center", sphereCollider.Center);

			UI::AttributeAssetReference("Material", AssetType::PhysicsMaterial, sphereCollider.m_PhysicsMaterialPointerID);
			UI::EndPropertyGrid();

		});
		DrawComponents<CapsuleColliderComponent>("Capsule Collider", entity, [](CapsuleColliderComponent& capsuleCollider) {
			UI::BeginPropertyGrid();

			UI::AttributeBool("IsTrigger", capsuleCollider.IsTrigger);
			UI::AttributeDrag("Radius", capsuleCollider.Radius, 0.5);
			UI::AttributeDrag("Height", capsuleCollider.Height, 0.5);
			UI::EnumCombo("Direction", capsuleCollider.Direction);
			UI::DrawVec3Control("Center", capsuleCollider.Center);
			UI::AttributeAssetReference("Material", AssetType::PhysicsMaterial, capsuleCollider.m_PhysicsMaterialPointerID);

			UI::EndPropertyGrid();
		});
		DrawComponents<MeshColliderComponent>("Mesh Collider", entity, [&](MeshColliderComponent& meshCollider) {
			UI::BeginPropertyGrid();

			if (UI::AttributeAssetReference("MeshCollider", AssetType::MeshCollider, meshCollider.ColliderID))
			{
				if (meshCollider.ColliderID == 0)
				{
					PhysicsEngine::GetOrCreateColliderAsset(entity, meshCollider);
				}

				if (entity.HasComponent<DynamicMeshComponent>())
					meshCollider.SubMeshIndex = entity.GetComponent<DynamicMeshComponent>().GetSubMeshIndex();
			}
			UI::AttributeBool("IsTrigger", meshCollider.IsTrigger);

			Count<MeshCollider> colliderAsset = nullptr;
			bool isPhysicalAsset = false;

			if (AssetManager::HasAsset(meshCollider.ColliderID))
			{
				isPhysicalAsset = !AssetManager::GetAssetInfo(meshCollider.ColliderID).RuntimeAsset;
				colliderAsset = AssetManager::GetAsset<MeshCollider>(meshCollider.ColliderID);
			}

			UI::PushItemDisabled(colliderAsset && isPhysicalAsset);
			UI::AttributeBool("Use Shared Shape", meshCollider.UseSharedShape);
			UI::SetTooltip("Allows this collider to share its collider data. (Default: False)");
			UI::PopItemDisabled();

			UI::AttributeAssetReference("Material", AssetType::PhysicsMaterial, meshCollider.m_PhysicsMaterialPointerID);
			UI::SetTooltip("Overrides the material provided by the collider asset if an explicit asset has been set");


			UI::EndPropertyGrid();
		});
		DrawComponents<RigidBodyComponent>("RigidBody", entity, [&](RigidBodyComponent& rigidBody) {
			static uint32_t checked = 0;
			if (entity.GetName() == "bouncyBall" && checked == 0)
			{
				checked = 1;
				PF_CORE_ASSERT(false);
			}
			UI::BeginPropertyGrid();

			UI::EnumCombo("RigidType", rigidBody.RigidBodyType);

			{
				if (!PhysicsLayerManager::IsLayerValid(rigidBody.PhysicsLayerID))
					rigidBody.PhysicsLayerID = 0;

				const PhysicsLayer& layer = PhysicsLayerManager::GetLayer(rigidBody.PhysicsLayerID);

				auto [changed, outSelectionIndex, outSelectionString] = UI::Combo("Layer", PhysicsLayerManager::GetLayersNames(), layer.Name);
				if(changed)
				{
					if (PhysicsLayerManager::IsLayerValid(outSelectionString))
					{
						rigidBody.PhysicsLayerID = PhysicsLayerManager::GetLayer(outSelectionString).LayerID;
					}
				}
			}
			if (rigidBody.RigidBodyType == RigidBodyType::Dynamic)
			{
				UI::EnumCombo("CollisionDetection", rigidBody.CollisionDetection);
				UI::AttributeDrag("Mass", rigidBody.Mass, 0.5, 0);
				UI::AttributeDrag("LinearDrag", rigidBody.LinearDrag, 0.5, 0);
				UI::AttributeDrag("AngularDrag", rigidBody.AngularDrag, 0.5);

				UI::AttributeBool("Gravity", rigidBody.Gravity);
				UI::AttributeBool("Kinematic", rigidBody.Kinematic);

				DrawVectorControl("Freeze Location", rigidBody.FreezeLocation, false);
				DrawVectorControl("Freeze Rotation", rigidBody.FreezeRotation, false);
			}
			UI::EndPropertyGrid();
		});


		DrawComponents<CharacterControllerComponent>("CharacterController", entity, [](CharacterControllerComponent& controller) {

			UI::BeginPropertyGrid();

			{
				float slopdeg = glm::degrees(controller.SlopeLimitRadians);
				if (UI::AttributeDrag("SlopeLimitDeg", slopdeg,0.5,0))
					controller.SlopeLimitRadians = glm::radians(slopdeg);
			}

			UI::AttributeDrag("SkinOffset", controller.SkinOffset, 0.1,0.001);
			UI::AttributeBool("GravityEnabled", controller.GravityEnabled);
			UI::AttributeDrag("GravityScale", controller.GravityScale);
			UI::AttributeDrag("MinMoveDistance", controller.MinMoveDistance, 0.1,0);
			{
				bool disabled = std::cos(controller.SlopeLimitRadians) < 0.0f;
				UI::PushItemDisabled(disabled);
				UI::EnumCombo("WalkableMode", controller.WalkableMode, {}, 
					{
					"if character lands on a slope it cannot walk it would prevent climbing",
					"if character lands on a slope it cannot walk it would prevent climbing, the character will slide down"
					},
					"only valid if cos(SlopeLimitDeg) is greater than 0");
				UI::PopItemDisabled();
			}
			UI::AttributeAssetReference("PhysicsMaterial", AssetType::PhysicsMaterial, controller.PhysicsMaterialID);

			ImGui::Separator();

			UI::EnumCombo("ColliderType", controller.ColliderType);
			UI::AttributeDrag("Center", controller.Center);

			if (controller.ColliderType == CharacterControllerType::Capsule)
			{
				UI::AttributeDrag("Radius", controller.Radius);
				UI::AttributeDrag("Height", controller.Height);
			}
			else
			{
				UI::AttributeDrag("Size", controller.Size);
			}
			UI::EndPropertyGrid();
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
				UI::AttributeTextBar(fmt::format("Index {}", index), name);
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
			#if 1

			auto scriptWorld = m_ActiveWorld->GetScriptWorld();
			if (UI::AttributeButton("","Add Script"))
			{
				ImGui::OpenPopup("Open Scripts");
			}
			std::string scriptName;
			if (UI::Widgets::SearchScriptsPopup("Open Scripts", scriptName))
			{
				if (scriptWorld->IsEntityScriptInstantiated(entity))
				{
					scriptWorld->ScriptEntityPushScript(entity, scriptName);
				}
				else
				{
					scriptComp.ScriptMetadates.emplace_back(ScriptComponentsClassesData{ scriptName });
					scriptWorld->InstantiateScriptEntity(entity);
				}
			}

			if (!scriptWorld->IsEntityScriptInstantiated(entity))
				return;

			auto& classFields = *scriptWorld->GetEntityClassesContainer(entity);

			for (auto& [className, classMetaData] : classFields.GetClassesMetaData())
			{
				const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;
				UI::ScopedStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 0,1.5 });
				bool open = ImGui::TreeNodeEx(classMetaData.className.c_str(), treeNodeFlags, classMetaData.className.c_str());
				if (!open)continue;
				ImGui::SameLine();
				if (ImGui::Button("-"))
				{
					scriptWorld->ScriptEntityDeleteScript(entity, classMetaData.className);
					ImGui::TreePop();
					continue;
				}
				UI::BeginPropertyGrid();

				//usign this becuase it stores the field in order form top to bottom
				ManagedClass* managedClass = ScriptRegistry::GetManagedClassByName(classMetaData.className);
				if (managedClass == nullptr)
					continue;
				for (const auto& fieldName : managedClass->Fields)
				{
					if (!classMetaData.Fields.contains(fieldName))
						continue;

					Count<FieldStorageBase> field = classMetaData.Fields.at(fieldName);

					std::string fieldName = field->GetFieldInfo()->DisplayName.empty() ? Utils::String::SubStr(field->GetFieldInfo()->Name, field->GetFieldInfo()->Name.find(':') + 1) : field->GetFieldInfo()->DisplayName;

					if (field->GetFieldInfo()->IsArray())
					{
						Count<ArrayFieldStorage> storage = field.As<ArrayFieldStorage>();
						UI::DrawFieldValue(m_ActiveWorld, fieldName, storage);

					}
					else if (field->GetFieldInfo()->IsEnum())
					{
						Count<EnumFieldStorage> storage = field.As<EnumFieldStorage>();
						UI::DrawFieldValue(m_ActiveWorld, fieldName, storage);
					}
					else
					{
						Count<FieldStorage> storage = field.As<FieldStorage>();
						UI::DrawFieldValue(m_ActiveWorld, fieldName, storage);
					}
				}
				#if 0
				for (const auto& [fieldName,field] : classMetaData.Fields)
				{
					std::string fieldName = field->GetFieldInfo()->DisplayName.empty() ? Utils::String::SubStr(field->GetFieldInfo()->Name, field->GetFieldInfo()->Name.find(':') + 1) : field->GetFieldInfo()->DisplayName;

					if (field->GetFieldInfo()->IsArray())
					{

					
					}
					else if (field->GetFieldInfo()->IsEnum())
					{
						Count<EnumFieldStorage> storage = field.As<EnumFieldStorage>();
						UI::DrawFieldValue(m_ActiveWorld, fieldName, storage);
					}
					else
					{
						Count<FieldStorage> storage = field.As<FieldStorage>();
						UI::DrawFieldValue(m_ActiveWorld, fieldName, storage);
					}
				}
				#endif
				UI::EndPropertyGrid();
				ImGui::TreePop();
			}


			#endif
			# if 0
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
									UI::AttributeBool(fieldName, var);
									scriptField.SetValue<bool>(var);
									break;
								}
							case ScriptFieldType::Prefab:
								{
									if (AssetManager::HasAsset(scriptField.GetValue<uint64_t>()))
									{
										auto assetInfo =AssetManager::GetAssetInfo(scriptField.GetValue<uint64_t>());
										UI::AttributeTextBar(field.Name, assetInfo.GetName());

									}
									else
									{
										scriptField.SetValue<uint64_t>(0);

										UI::AttributeTextBar(field.Name, "null (Prefab)");
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
										UI::AttributeTextBar(field.Name, ent.GetName());
										scriptField.SetValue<uint64_t>(ent.GetUUID().Get());
									}
									else
									{
										scriptField.SetValue<uint64_t>(0);
										UI::AttributeTextBar(field.Name, "null (Entity)");
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
										UI::AttributeTextBar(field.Name, assetInfo.GetName());

									}
									else
									{
										instance->SetFieldValue<uint64_t>(name,0);

										UI::AttributeTextBar(field.Name, "null (Prefab)");
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
										UI::AttributeTextBar(field.Name, ent.GetName());

									}
									else
									{
										instance->SetFieldValue<uint64_t>(name,0);
										UI::AttributeTextBar(field.Name, "null (Entity)");
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
			#endif
		});

		DrawComponents<TextComponent>("Text Component", entity, [](TextComponent& textComponent) {
			//ImGui::InputTextMultiline("Text", (char*)textComponent.Text.c_str(), textComponent.Text.capacity() + 1);
			UI::AttributeInputTextMultiline("", textComponent.Text,0);
			UI::BeginPropertyGrid();
			UI::AttributeColor("Colour", textComponent.Colour);

			UI::AttributeDrag("Kernng", textComponent.Kerning, 0.025);
			UI::AttributeDrag("Line Spacing", textComponent.LineSpacing, 0.025);
			UI::EndPropertyGrid();
		});

		DrawComponents<PlayerInputComponent>("Player Input", entity, [](PlayerInputComponent& player) {
			UI::BeginPropertyGrid();

			UI::EnumCombo("Player", player.InputPlayer);
			//UI::AttributeAssetReference("Player",AssetType::Prefab, player.Player);
			UI::EndPropertyGrid();
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
				UI::AttributeTextBar(fmt::format("Index {}", index), name);
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
			UI::BeginPropertyGrid();

			UI::AttributeAssetReference("Audio",AssetType::Audio, audio.AudioAsset);

			UI::AttributeSlider("VolumeMultiplier", audio.VolumeMultiplier, 0, 1);
			UI::AttributeSlider("PitchMultiplier", audio.PitchMultiplier, 0, 24);

			UI::AttributeBool("Looping", audio.Looping);
			UI::AttributeBool("Play On Awake", audio.PlayOnAwake);
			UI::AttributeBool("Specialization", audio.SpatializationEnabled);

			if (audio.SpatializationEnabled == false)
			{

				UI::EnumCombo("Attenuation Model", audio.AttenuationModel);

				UI::AttributeDrag("Min Gain", audio.MinGain, 0.025f, 0, 1);
				UI::AttributeDrag("Max Gain", audio.MaxGain, 0.025f, 0, 1);

				UI::AttributeDrag("Min Distance", audio.MinDistance, 0.025f, 0, audio.MaxDistance);
				UI::AttributeDrag("Max Distance", audio.MaxDistance, 0.025f, audio.MinDistance, Math::GetMaxType<float>());

				{
					float degrees = glm::degrees(audio.ConeInnerAngleInRadians);
					if (UI::AttributeSlider("Cone Inner Angle", degrees, 0, 360))
						audio.ConeInnerAngleInRadians = glm::radians(degrees);

					degrees = glm::degrees(audio.ConeOuterAngleInRadians);
					if (UI::AttributeSlider("Cone Outer Angle", degrees, 0, 360))
						audio.ConeOuterAngleInRadians = glm::radians(degrees);

					UI::AttributeSlider("ConeOuterGain", audio.ConeOuterGain, 0, 1);
				}

				UI::AttributeSlider("Doppler Factor", audio.DopplerFactor, 0, 1);
				UI::AttributeSlider("RollOff", audio.Rolloff, 0, 1);
			}

			UI::EndPropertyGrid();
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
