#include "Proofprch.h"
#include "SceneHierachyPanel.h"
#include "Proof/ImGui/ImGuiLayer.h"
#include "Proof/Scene/Entity.h"
#include "Proof/Scene/Component.h"
#include <imgui.h>

#include "Proof/Scene/EntitiyComponentSystem/ECS.h"
#include "Proofprch.h"
#include "Proof/Asset/Asset.h"
#include "Proof/Physics/MeshCollider.h"
#include "Proof/Physics/PhysicsEngine.h"
#include "Proof/Project/Project.h"
#include "Proof/Renderer/Texture.h"
#include "Proof/Physics/PhysicsMaterial.h"
//#include "ContentBrowserPanel.h"
#include <vector>
#include "Proof/Scene/WaterSystem/WaterSystem.h"
#include "Proof/Scene/ExampleSccripts.h"
#include "Proof/Scene/Material.h"
#include "Proof/Scene/Script.h"
#include "Proof/Input/KeyCodes.h"
#include "Proof/Scene/Component.h"
#include "Proof/Asset/AssetManager.h"
#include <string.h>
#include<magic_enum.hpp>
#include "Proof/Resources/EnumReflection.h"
#include "Proof/Input/ElevatedInputSystem/ElevatedPlayer.h"
#include "Proof/Input/ElevatedInputSystem/InputBindingContext.h"
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
#include "Proof/Scene/WaterSystem/Water.h"
#include "Proof/Scene/WaterSystem/GerstnerWave.h"
#include "Proof/Scene/WaterSystem/FFTWave/FFTWave.h"
#include "Proof/Scene/WaterSystem/FFTWave/FFTWaveRealistic.h"
//include those before stdlig.h
#include "Proof/Renderer/UIRenderer/UIPanel.h"
#include "Proof/Renderer/ParticleSystem.h"
#include "Proof/Scene/TerrainRenderer/TerrainRenderer.h"
#include <ImCurveEdit.h>
//#include "misc/cpp/imgui_stdlib.h"
#include "Proof/Scene/Mesh.h"
#include "Proof/Scripting/ScriptWorld.h"
#include "Proof/ImGui/UiUtilities.h"
#include "Proof/ImGui/UIWidgets.h"
#include "Proof/ImGui/UIGenericEditors.h"
#define IM_VEC2_CLASS_EXTRA
#include <imgui_internal.h>
#include <math.h>
enum class CurveEditorFlags
{
	NO_TANGENTS = 1 << 0,
	SHOW_GRID = 1 << 1,
	RESET = 1 << 2
};
static const float NODE_SLOT_RADIUS = 4.0f;
struct ImVec2;

namespace ImGui
{
	int CurveEditor(const char* label
		, float* values
		, int points_count
		, const ImVec2& editor_size
		, ImU32 flags
		, int* new_count)
	{
		enum class StorageValues : ImGuiID
		{
			FROM_X = 100,
			FROM_Y,
			WIDTH,
			HEIGHT,
			IS_PANNING,
			POINT_START_X,
			POINT_START_Y
		};


		const float HEIGHT = 100;
		static ImVec2 start_pan;


		ImGuiContext& g = *GImGui;
		const ImGuiStyle& style = g.Style;
		ImVec2 size = editor_size;
		size.x = size.x < 0 ? CalcItemWidth() + (style.FramePadding.x * 2) : size.x;
		size.y = size.y < 0 ? HEIGHT : size.y;


		ImGuiWindow* parent_window = GetCurrentWindow();
		ImGuiID id = parent_window->GetID(label);
		if (!BeginChildFrame(id, size, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse))
		{
			EndChild();
			return -1;
		}


		int hovered_idx = -1;
		if (new_count) *new_count = points_count;


		ImGuiWindow* window = GetCurrentWindow();
		if (window->SkipItems)
		{
			EndChild();
			return -1;
		}

		ImVec2 points_min(FLT_MAX, FLT_MAX);
		ImVec2 points_max(-FLT_MAX, -FLT_MAX);
		for (int point_idx = 0; point_idx < points_count; ++point_idx)
		{
			ImVec2 point;
			if (flags & (int)CurveEditorFlags::NO_TANGENTS)
			{
				point = ((ImVec2*)values)[point_idx];
			}
			else
			{
				point = ((ImVec2*)values)[1 + point_idx * 3];
			}
			points_max = ImMax(points_max, point);
			points_min = ImMin(points_min, point);
		}
		points_max.y = ImMax(points_max.y, points_min.y + 0.0001f);


		float from_x = window->StateStorage.GetFloat((ImGuiID)StorageValues::FROM_X, points_min.x);
		float from_y = window->StateStorage.GetFloat((ImGuiID)StorageValues::FROM_Y, points_min.y);
		float width = window->StateStorage.GetFloat((ImGuiID)StorageValues::WIDTH, points_max.x - points_min.x);
		float height = window->StateStorage.GetFloat((ImGuiID)StorageValues::HEIGHT, points_max.y - points_min.y);
		window->StateStorage.SetFloat((ImGuiID)StorageValues::FROM_X, from_x);
		window->StateStorage.SetFloat((ImGuiID)StorageValues::FROM_Y, from_y);
		window->StateStorage.SetFloat((ImGuiID)StorageValues::WIDTH, width);
		window->StateStorage.SetFloat((ImGuiID)StorageValues::HEIGHT, height);


		ImVec2 beg_pos = GetCursorScreenPos();


		const ImRect inner_bb = window->InnerRect;
		const ImRect frame_bb(inner_bb.Min - style.FramePadding, inner_bb.Max + style.FramePadding);


		auto transform = [&](const ImVec2& pos) -> ImVec2
			{
				float x = (pos.x - from_x) / width;
				float y = (pos.y - from_y) / height;


				return ImVec2(
					inner_bb.Min.x * (1 - x) + inner_bb.Max.x * x,
					inner_bb.Min.y * y + inner_bb.Max.y * (1 - y)
				);
			};


		auto invTransform = [&](const ImVec2& pos) -> ImVec2
			{
				float x = (pos.x - inner_bb.Min.x) / (inner_bb.Max.x - inner_bb.Min.x);
				float y = (inner_bb.Max.y - pos.y) / (inner_bb.Max.y - inner_bb.Min.y);


				return ImVec2(
					from_x + width * x,
					from_y + height * y
				);
			};


		if (flags & (int)CurveEditorFlags::SHOW_GRID)
		{
			int exp;
			frexp(width / 5, &exp);
			float step_x = (float)ldexp(1.0, exp);
			int cell_cols = int(width / step_x);


			float x = step_x * int(from_x / step_x);
			for (int i = -1; i < cell_cols + 2; ++i)
			{
				ImVec2 a = transform({ x + i * step_x, from_y });
				ImVec2 b = transform({ x + i * step_x, from_y + height });
				window->DrawList->AddLine(a, b, 0x55000000);
				char buf[64];
				if (exp > 0)
				{
					ImFormatString(buf, sizeof(buf), " %d", int(x + i * step_x));
				}
				else
				{
					ImFormatString(buf, sizeof(buf), " %f", x + i * step_x);
				}
				window->DrawList->AddText(b, 0x55000000, buf);
			}


			frexp(height / 5, &exp);
			float step_y = (float)ldexp(1.0, exp);
			int cell_rows = int(height / step_y);


			float y = step_y * int(from_y / step_y);
			for (int i = -1; i < cell_rows + 2; ++i)
			{
				ImVec2 a = transform({ from_x, y + i * step_y });
				ImVec2 b = transform({ from_x + width, y + i * step_y });
				window->DrawList->AddLine(a, b, 0x55000000);
				char buf[64];
				if (exp > 0)
				{
					ImFormatString(buf, sizeof(buf), " %d", int(y + i * step_y));
				}
				else
				{
					ImFormatString(buf, sizeof(buf), " %f", y + i * step_y);
				}
				window->DrawList->AddText(a, 0x55000000, buf);
			}
		}


		if (ImGui::GetIO().MouseWheel != 0 && ImGui::IsItemHovered())
		{
			float scale = powf(2, ImGui::GetIO().MouseWheel);
			width *= scale;
			height *= scale;
			window->StateStorage.SetFloat((ImGuiID)StorageValues::WIDTH, width);
			window->StateStorage.SetFloat((ImGuiID)StorageValues::HEIGHT, height);
		}
		if (ImGui::IsMouseReleased(1))
		{
			window->StateStorage.SetBool((ImGuiID)StorageValues::IS_PANNING, false);
		}
		if (window->StateStorage.GetBool((ImGuiID)StorageValues::IS_PANNING, false))
		{
			ImVec2 drag_offset = ImGui::GetMouseDragDelta(1);
			from_x = start_pan.x;
			from_y = start_pan.y;
			from_x -= drag_offset.x * width / (inner_bb.Max.x - inner_bb.Min.x);
			from_y += drag_offset.y * height / (inner_bb.Max.y - inner_bb.Min.y);
			window->StateStorage.SetFloat((ImGuiID)StorageValues::FROM_X, from_x);
			window->StateStorage.SetFloat((ImGuiID)StorageValues::FROM_Y, from_y);
		}
		else if (ImGui::IsMouseDragging(1) && ImGui::IsItemHovered())
		{
			window->StateStorage.SetBool((ImGuiID)StorageValues::IS_PANNING, true);
			start_pan.x = from_x;
			start_pan.y = from_y;
		}


		int changed_idx = -1;
		for (int point_idx = points_count - 2; point_idx >= 0; --point_idx)
		{
			ImVec2* points;
			if (flags & (int)CurveEditorFlags::NO_TANGENTS)
			{
				points = ((ImVec2*)values) + point_idx;
			}
			else
			{
				points = ((ImVec2*)values) + 1 + point_idx * 3;
			}

			ImVec2 p_prev = points[0];
			ImVec2 tangent_last;
			ImVec2 tangent;
			ImVec2 p;
			if (flags & (int)CurveEditorFlags::NO_TANGENTS)
			{
				p = points[1];
			}
			else
			{
				tangent_last = points[1];
				tangent = points[2];
				p = points[3];
			}


			auto handlePoint = [&](ImVec2& p, int idx) -> bool
				{
					static const float SIZE = 3;


					ImVec2 cursor_pos = GetCursorScreenPos();
					ImVec2 pos = transform(p);


					SetCursorScreenPos(pos - ImVec2(SIZE, SIZE));
					PushID(idx);
					InvisibleButton("", ImVec2(2 * NODE_SLOT_RADIUS, 2 * NODE_SLOT_RADIUS));


					ImU32 col = IsItemActive() || IsItemHovered() ? GetColorU32(ImGuiCol_PlotLinesHovered) : GetColorU32(ImGuiCol_PlotLines);


					window->DrawList->AddLine(pos + ImVec2(-SIZE, 0), pos + ImVec2(0, SIZE), col);
					window->DrawList->AddLine(pos + ImVec2(SIZE, 0), pos + ImVec2(0, SIZE), col);
					window->DrawList->AddLine(pos + ImVec2(SIZE, 0), pos + ImVec2(0, -SIZE), col);
					window->DrawList->AddLine(pos + ImVec2(-SIZE, 0), pos + ImVec2(0, -SIZE), col);


					if (IsItemHovered()) hovered_idx = point_idx + idx;


					bool changed = false;
					if (IsItemActive() && IsMouseClicked(0))
					{
						window->StateStorage.SetFloat((ImGuiID)StorageValues::POINT_START_X, pos.x);
						window->StateStorage.SetFloat((ImGuiID)StorageValues::POINT_START_Y, pos.y);
					}


					if (IsItemHovered() || IsItemActive() && IsMouseDragging(0))
					{
						char tmp[64];
						ImFormatString(tmp, sizeof(tmp), "%0.2f, %0.2f", p.x, p.y);
						window->DrawList->AddText({ pos.x, pos.y - GetTextLineHeight() }, 0xff000000, tmp);
					}


					if (IsItemActive() && IsMouseDragging(0))
					{
						pos.x = window->StateStorage.GetFloat((ImGuiID)StorageValues::POINT_START_X, pos.x);
						pos.y = window->StateStorage.GetFloat((ImGuiID)StorageValues::POINT_START_Y, pos.y);
						pos += ImGui::GetMouseDragDelta();
						ImVec2 v = invTransform(pos);


						p = v;
						changed = true;
					}
					PopID();


					SetCursorScreenPos(cursor_pos);
					return changed;
				};


			auto handleTangent = [&](ImVec2& t, const ImVec2& p, int idx) -> bool
				{
					static const float SIZE = 2;
					static const float LENGTH = 18;


					auto normalized = [](const ImVec2& v) -> ImVec2
						{
							float len = 1.0f / sqrtf(v.x * v.x + v.y * v.y);
							return ImVec2(v.x * len, v.y * len);
						};


					ImVec2 cursor_pos = GetCursorScreenPos();
					ImVec2 pos = transform(p);
					ImVec2 tang = pos + normalized(ImVec2(t.x, -t.y)) * LENGTH;


					SetCursorScreenPos(tang - ImVec2(SIZE, SIZE));
					PushID(-idx);
					InvisibleButton("", ImVec2(2 * NODE_SLOT_RADIUS, 2 * NODE_SLOT_RADIUS));


					window->DrawList->AddLine(pos, tang, GetColorU32(ImGuiCol_PlotLines));


					ImU32 col = IsItemHovered() ? GetColorU32(ImGuiCol_PlotLinesHovered) : GetColorU32(ImGuiCol_PlotLines);


					window->DrawList->AddLine(tang + ImVec2(-SIZE, SIZE), tang + ImVec2(SIZE, SIZE), col);
					window->DrawList->AddLine(tang + ImVec2(SIZE, SIZE), tang + ImVec2(SIZE, -SIZE), col);
					window->DrawList->AddLine(tang + ImVec2(SIZE, -SIZE), tang + ImVec2(-SIZE, -SIZE), col);
					window->DrawList->AddLine(tang + ImVec2(-SIZE, -SIZE), tang + ImVec2(-SIZE, SIZE), col);


					bool changed = false;
					if (IsItemActive() && IsMouseDragging(0))
					{
						tang = GetIO().MousePos - pos;
						tang = normalized(tang);
						tang.y *= -1;


						t = tang;
						changed = true;
					}
					PopID();


					SetCursorScreenPos(cursor_pos);
					return changed;
				};


			PushID(point_idx);
			if ((flags & (int)CurveEditorFlags::NO_TANGENTS) == 0)
			{
				window->DrawList->AddBezierCurve(
					transform(p_prev),
					transform(p_prev + tangent_last),
					transform(p + tangent),
					transform(p),
					GetColorU32(ImGuiCol_PlotLines),
					1.0f,
					20);
				if (handleTangent(tangent_last, p_prev, 0))
				{
					points[1] = ImClamp(tangent_last, ImVec2(0, -1), ImVec2(1, 1));
					changed_idx = point_idx;
				}
				if (handleTangent(tangent, p, 1))
				{
					points[2] = ImClamp(tangent, ImVec2(-1, -1), ImVec2(0, 1));
					changed_idx = point_idx + 1;
				}
				if (handlePoint(p, 1))
				{
					if (p.x <= p_prev.x) p.x = p_prev.x + 0.001f;
					if (point_idx < points_count - 2 && p.x >= points[6].x)
					{
						p.x = points[6].x - 0.001f;
					}
					points[3] = p;
					changed_idx = point_idx + 1;
				}


			}
			else
			{
				window->DrawList->AddLine(transform(p_prev), transform(p), GetColorU32(ImGuiCol_PlotLines), 1.0f);
				if (handlePoint(p, 1))
				{
					if (p.x <= p_prev.x) p.x = p_prev.x + 0.001f;
					if (point_idx < points_count - 2 && p.x >= points[2].x)
					{
						p.x = points[2].x - 0.001f;
					}
					points[1] = p;
					changed_idx = point_idx + 1;
				}
			}
			if (point_idx == 0)
			{
				if (handlePoint(p_prev, 0))
				{
					if (p.x <= p_prev.x) p_prev.x = p.x - 0.001f;
					points[0] = p_prev;
					changed_idx = point_idx;
				}
			}
			PopID();
		}


		SetCursorScreenPos(inner_bb.Min);


		InvisibleButton("bg", inner_bb.Max - inner_bb.Min);


		if (ImGui::IsItemActive() && ImGui::IsMouseDoubleClicked(0) && new_count)
		{
			ImVec2 mp = ImGui::GetMousePos();
			ImVec2 new_p = invTransform(mp);
			ImVec2* points = (ImVec2*)values;


			if ((flags & (int)CurveEditorFlags::NO_TANGENTS) == 0)
			{
				points[points_count * 3 + 0] = ImVec2(-0.2f, 0);
				points[points_count * 3 + 1] = new_p;
				points[points_count * 3 + 2] = ImVec2(0.2f, 0);;
				++*new_count;


				auto compare = [](const void* a, const void* b) -> int
					{
						float fa = (((const ImVec2*)a) + 1)->x;
						float fb = (((const ImVec2*)b) + 1)->x;
						return fa < fb ? -1 : (fa > fb) ? 1 : 0;
					};


				qsort(values, points_count + 1, sizeof(ImVec2) * 3, compare);


			}
			else
			{
				points[points_count] = new_p;
				++*new_count;


				auto compare = [](const void* a, const void* b) -> int
					{
						float fa = ((const ImVec2*)a)->x;
						float fb = ((const ImVec2*)b)->x;
						return fa < fb ? -1 : (fa > fb) ? 1 : 0;
					};


				qsort(values, points_count + 1, sizeof(ImVec2), compare);
			}
		}


		if (hovered_idx >= 0 && ImGui::IsMouseDoubleClicked(0) && new_count && points_count > 2)
		{
			ImVec2* points = (ImVec2*)values;
			--*new_count;
			if ((flags & (int)CurveEditorFlags::NO_TANGENTS) == 0)
			{
				for (int j = hovered_idx * 3; j < points_count * 3 - 3; j += 3)
				{
					points[j + 0] = points[j + 3];
					points[j + 1] = points[j + 4];
					points[j + 2] = points[j + 5];
				}
			}
			else
			{
				for (int j = hovered_idx; j < points_count - 1; ++j)
				{
					points[j] = points[j + 1];
				}
			}
		}


		EndChildFrame();
		RenderText(ImVec2(frame_bb.Max.x + style.ItemInnerSpacing.x, inner_bb.Min.y), label);
		return changed_idx;
	}
}
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
			newEntity.AddComponent<MeshColliderComponent>().ColliderKey = AssetManager::GetDefaultAsset(DefaultRuntimeAssets::Type)->GetID();\
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

	template<class T, class UIFunction>
	static void AddComponentGuiButton(Entity entity, const std::string& name, UIFunction function) {
		if (ImGui::MenuItem(name.c_str()))
		{
			entity.AddComponent<T>();
			function(entity, entity.GetComponent<T>());
			ImGui::CloseCurrentPopup();
		}
	};
	SceneHierachyPanel::SceneHierachyPanel(bool IsWorld, UUID prefabID)
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

				for (auto [id, entity] : m_ActiveWorld->GetEntities())
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
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("SceneEntity"))
				{

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
				newEntity.GetComponent<TransformComponent>().SetRotationEuler(glm::radians(glm::vec3(2, 1, 0)));
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

		if (ImGui::MenuItem("PlayerStart")) {
			newEntity = m_ActiveWorld->CreateEntity("PlayerStart");
			newEntity.AddComponent<PlayerStartComponent>();
		}
		if (owner && newEntity.GetUUID() != 0) {
			owner.AddChild(newEntity);
		}
		if (newEntity.GetUUID() != 0)
			return true;
		return false;
	}
	bool SceneHierachyPanel::CreateEntityMenu()
	{
		return CreateEntityMenu({});
	}
	void SceneHierachyPanel::DrawEntityNode(Entity entity) {
		auto& tc = entity.GetComponent<TagComponent>().Tag;
		//PF_ENGINE_TRACE("Name:{} X:{} Y:{} Z:{}", tc, entity.GetComponent<TransformComponent>().GetRotationEuler().x,
		//	entity.GetComponent<TransformComponent>().GetRotationEuler().y, entity.GetComponent<TransformComponent>().GetRotationEuler().z);

		ImGui::PushID(entity.GetUUID());
		ImGuiTreeNodeFlags flags;
		if (m_IsWorld)
			flags = ((SelectionManager::IsSelected(SelectionContext::Scene, entity.GetUUID()) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow);
		else
			flags = ((AssetSelectionManager::IsSelected(AssetSelectionContext::Prefab, m_PrefabID, entity.GetUUID()) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow);
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
		if (ImGui::IsItemClicked() && ImGui::IsKeyDown((ImGuiKey)KeyBoardKey::E) == false)
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
		if (ImGui::BeginPopupContextItem("Entity Settings")) {
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
				if (entity.HasChildren())
				{
					if (ImGui::MenuItem("Delete Keep Children"))
					{
						m_ActiveWorld->DeleteEntity(entity, false);

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
				if (entity.HasChildren())
				{
					if (ImGui::MenuItem("Delete Keep Children"))
					{
						m_ActiveWorld->DeleteEntity(entity, false);

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
						AssetSelectionManager::Select(AssetSelectionContext::Prefab, m_PrefabID, newEntity.GetUUID());
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
				if (m_ActiveWorld->HasEntity(I))
					DrawEntityNode(m_ActiveWorld->GetEntity(I));
				else
					PF_INFO("Entity {} no child {}", entity.GetName(), I);
			}
			ImGui::TreePop();
		}
		ImGui::PopID();
	}
	static bool DynamicMeshUseSlider = false;
	template<typename T, typename UIFunction>
	void SceneHierachyPanel::DrawComponents(const std::string& name, Entity& entity, UIFunction Uifunction, const std::string& toolTip) 
	{
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

		if (ImGui::MenuItem("ConvertToDiskMaterial"))
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
						//AssetManager::ConvertRuntimeToDiskAsset(id, savedPath);

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
			AddComponentGui<TerrainComponent>(entity, "Terrain");
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
			AddComponentGui<PlayerStartComponent>(entity, "Player start");
			AddComponentGui<PlayerHUDComponent>(entity, "Player HUD");

			AddComponentGui<ParticleSystemComponent>(entity, "Particle System");

			AddComponentGui<AudioComponent>(entity, "Audio");
			AddComponentGui<WaterComponent>(entity, "Water");
			AddComponentGui<BuoyancyComponent>(entity, "Buoyancy");
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

				auto mesh = meshComp.GetMesh();
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

				if (mesh)
				{
					UI::AttributeDrawMaterialTable(meshComp.MaterialTable, mesh->GetMaterialTable());
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
						if (UI::AttributeDrag("SubMesh Index", submeshIndex, 1, 0, (uint32_t)mesh->GetMeshSource()->GetSubMeshes().size() - 1))
						{
							meshComp.SetSubMeshIndex(submeshIndex);
						}
					}
					else
					{
						const SubMesh& currentSubMesh = mesh->GetMeshSource()->GetSubMeshes().at(meshComp.GetSubMeshIndex());

						std::vector<std::string> subMeshes;
						subMeshes.resize(mesh->GetSubMeshes().size());

						for (auto& index : mesh->GetSubMeshes())
						{
							subMeshes[index] = mesh->GetMeshSource()->GetSubMeshes().at(index).Name;
						}
						auto [changed, currentSelectIndex, currentSelect] = UI::Combo("SubMesh", subMeshes, currentSubMesh.Name);
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

				if (mesh)
				{
					UI::AttributeDrawMaterialTable(meshComp.MaterialTable, mesh->GetMaterialTable());
				}
			});

		DrawComponents<TerrainComponent>("Terrain", entity, [](TerrainComponent& meshComp)
			{

				bool modified = false;
				UI::PushModified(modified);
				UI::AttributeDrag("Mesh Scale", meshComp.Terrain->TerrainScale, 0.1f);
				UI::AttributeSlider("LevelOfDetail", meshComp.Terrain->LevelOfDetail);
				UI::BeginPropertyGrid();
				auto& settings = meshComp.Terrain->NoiseParams;

				UI::AttributeDrag("Offset", settings.Offset,0.25);
				UI::AttributeDrag("Scale", settings.Scale, 0.1f);
				UI::AttributeSlider("Octaves", settings.Octaves);
				UI::AttributeDrag("Persistence", settings.Persistence, 0.01f);
				UI::AttributeDrag("Lacunarity", settings.Lacunarity, 0.01f);
				UI::EndPropertyGrid();


				UI::PopModified();
				if(modified)
					meshComp.Terrain->RegenerateTerrainMesh();

				UI::InterpolationCurveEditor("Interpolation", meshComp.Terrain->Curve);

				if (ImGui::Button("Rengenerate"))
				{
					meshComp.Terrain->RegenerateTerrainMesh();
				
				}
				UI::ViewDebugImage("Color Texure", meshComp.Terrain->GetColorTexture());
				UI::ViewDebugImage("Noise Texure", meshComp.Terrain->GetNoiseTexture()->GetImage());

				if (ImGui::Button("change tex"))
				{
					if (meshComp.Terrain->GetTerrainMesh()->GetMaterialTable()->GetMaterial(0)->GetAlbedoMap() == meshComp.Terrain->GetColorTexture())
						meshComp.Terrain->GetTerrainMesh()->GetMaterialTable()->GetMaterial(0)->SetAlbedoMap(Renderer::GetWhiteTexture());
					else
						meshComp.Terrain->GetTerrainMesh()->GetMaterialTable()->GetMaterial(0)->SetAlbedoMap(meshComp.Terrain->GetColorTexture());
				}

				

			});
		DrawComponents<SpriteComponent>({ "Sprite" }, entity, [](SpriteComponent& spriteComp) {

			UI::BeginPropertyGrid();

			if(spriteComp.Texture.IsValid())
				UI::Image(spriteComp.Texture.GetAsset<Texture2D>(), {30,30});
			else
				UI::Image(Renderer::GetWhiteTexture(), { 30,30 });

			UI::AttributeColor("Tint Color", spriteComp.Colour);

			UI::AttributeAssetKeyReference("Texture", spriteComp.Texture);
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

						UI::AttributeSlider("Turbidity", hosek.Turbidity, 2.f, 30.f);
						UI::AttributeSlider("GroundReflectance", hosek.GroundReflectance, 0, 1);
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
						UI::AttributeSlider("Turbidity", pretham.Turbidity, 0, 30);
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
				UI::AttributeSlider("ShadowStrength", drl.ShadowStrength, 0, 1);
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
				UI::AttributeDrag("Falloff", pl.Falloff, 0.005, 0);
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
			UI::EnumCombo< ProjectionType>("ProjectionType", cameraComp.Projection, { ProjectionType::None });

			UI::AttributeBool("UseLocalRotation", cameraComp.UseLocalRotation);
			UI::AttributeBool("ActiveForRendering", cameraComp.ActiveForRendering);
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
			UI::DrawVec3Control("Size", cubeCollider.Size, glm::vec3{ 1 });
			UI::DrawVec3Control("Center", cubeCollider.Center);

			UI::AttributeAssetKeyReference("Material",cubeCollider.PhysicsMaterialKey);

			UI::EndPropertyGrid();

			});
		DrawComponents<SphereColliderComponent>("Sphere Collider", entity, [](SphereColliderComponent& sphereCollider) {
			UI::BeginPropertyGrid();

			UI::AttributeBool("IsTrigger", sphereCollider.IsTrigger);
			UI::AttributeDrag("Radius", sphereCollider.Radius, 0.5);
			UI::DrawVec3Control("Center", sphereCollider.Center);

			UI::AttributeAssetKeyReference("Material", sphereCollider.PhysicsMaterialKey);
			UI::EndPropertyGrid();

			});
		DrawComponents<CapsuleColliderComponent>("Capsule Collider", entity, [](CapsuleColliderComponent& capsuleCollider) {
			UI::BeginPropertyGrid();

			UI::AttributeBool("IsTrigger", capsuleCollider.IsTrigger);
			UI::AttributeDrag("Radius", capsuleCollider.Radius, 0.5);
			UI::AttributeDrag("Height", capsuleCollider.Height, 0.5);
			UI::EnumCombo("Direction", capsuleCollider.Direction);
			UI::DrawVec3Control("Center", capsuleCollider.Center);
			UI::AttributeAssetKeyReference("Material", capsuleCollider.PhysicsMaterialKey);

			UI::EndPropertyGrid();
			});
		DrawComponents<MeshColliderComponent>("Mesh Collider", entity, [&](MeshColliderComponent& meshCollider) {
			UI::BeginPropertyGrid();

			if (UI::AttributeAssetKeyReference("MeshCollider", meshCollider.ColliderKey))
			{
				if (meshCollider.ColliderKey.GetAssetID() == 0)
				{
					PhysicsEngine::GetOrCreateColliderAsset(entity, meshCollider);
				}

				if (entity.HasComponent<DynamicMeshComponent>())
					meshCollider.SubMeshIndex = entity.GetComponent<DynamicMeshComponent>().GetSubMeshIndex();
			}
			UI::AttributeBool("IsTrigger", meshCollider.IsTrigger);

			Count<MeshCollider> colliderAsset = nullptr;
			bool isPhysicalAsset = false;

			if (AssetManager::HasAsset(meshCollider.ColliderKey.GetAssetID()))
			{
				isPhysicalAsset = !AssetManager::GetAssetInfo(meshCollider.ColliderKey.GetAssetID()).RuntimeAsset;
				colliderAsset = AssetManager::GetAsset<MeshCollider>(meshCollider.ColliderKey.GetAssetID());
			}

			UI::PushItemDisabled(colliderAsset && isPhysicalAsset);
			UI::AttributeBool("Use Shared Shape", meshCollider.UseSharedShape);
			UI::SetTooltip("Allows this collider to share its collider data. (Default: False)");
			UI::PopItemDisabled();

			UI::AttributeAssetKeyReference("Material", meshCollider.PhysicsMaterialKey);
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
				if (changed)
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
				if (UI::AttributeDrag("SlopeLimitDeg", slopdeg, 0.5, 0))
					controller.SlopeLimitRadians = glm::radians(slopdeg);
			}

			UI::AttributeDrag("SkinOffset", controller.SkinOffset, 0.1, 0.001);
			UI::AttributeBool("GravityEnabled", controller.GravityEnabled);
			UI::AttributeDrag("GravityScale", controller.GravityScale);
			UI::AttributeDrag("MinMoveDistance", controller.MinMoveDistance, 0.1, 0);
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
			UI::AttributeAssetKeyReference("Material", controller.PhysicsMaterialKey);
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

		DrawComponents<ParticleSystemComponent>("Particle System", entity, [&](ParticleSystemComponent& particleSystem) {
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
				if (particleHandler != nullptr && AssetManager::HasAsset(particleHandler->GetParticleSystem()))
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
			auto scriptWorld = m_ActiveWorld->GetScriptWorld();
			if (UI::AttributeButton("", "Add Script"))
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

			auto& classFields = *scriptWorld->GetEntityClassesContainer(entity, true);

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
				for (const auto& [fieldName, field] : classMetaData.Fields)
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


	
			});
			DrawComponents< PlayerStartComponent>("Player Start component", entity, [](PlayerStartComponent& playerStartComponent)
				{
					UI::BeginPropertyGrid();

					UI::AttributeBool("Dont spawn", playerStartComponent.DontSpawn);
					if(!playerStartComponent.DontSpawn)
						UI::AttributeAssetKeyReference("Player", playerStartComponent.Player);
					UI::EnumCombo("Input Player", playerStartComponent.InputPlayer);

					UI::EndPropertyGrid();
				});
		DrawComponents<TextComponent>("Text Component", entity, [](TextComponent& textComponent) {
			//ImGui::InputTextMultiline("Text", (char*)textComponent.Text.c_str(), textComponent.Text.capacity() + 1);
			UI::AttributeInputTextMultiline("", textComponent.Text, 0);
			UI::BeginPropertyGrid();
			UI::AttributeColor("Colour", textComponent.Colour);

			UI::AttributeDrag("Kernng", textComponent.Kerning, 0.025);
			UI::AttributeDrag("Line Spacing", textComponent.LineSpacing, 0.025);
			UI::AttributeBool("Visible", textComponent.Visible);
			UI::AttributeBool("UseLocalRotation", textComponent.UseLocalRotation);
			UI::AttributeBool("RenderInViewSpace", textComponent.RenderInViewSpace);
			UI::EndPropertyGrid();
			});

		DrawComponents<BuoyancyComponent>("Buoyancy Component", entity, [&](BuoyancyComponent& buoyancyComponent) 
			{
				{

					UI::BeginPropertyGrid();

					UI::AttributeDrag("Density", buoyancyComponent.Density,0.01	);
					ImGui::SameLine();
					UI::AttributeText("Yo wagawan");
					UI::AttributeSlider("VoxelRelativeSize", buoyancyComponent.VoxelRelativeSize);
					UI::AttributeDrag("DragInWater", buoyancyComponent.DragInWater,0.01);
					UI::AttributeDrag("AngularDragInWater", buoyancyComponent.AngularDragInWater, 0.01);
					UI::AttributeSlider("DisplacementFactor", buoyancyComponent.WaveDisplacementFactor,"How Much the water can chagne the object location");
					UI::AttributeBool("Always stay upright",buoyancyComponent.AlwaysStayUpright,"If this objeect is intended to always stay upright, it should not tip, set to true, help save some performance");

					UI::EndPropertyGrid();
				}

				UI::Separator();

				if (UI::AttributeButton("", "AddFloater"))
				{
					auto floater = m_ActiveWorld->CreateChildEntity(entity, "Floater");
					buoyancyComponent.Floaters.emplace_back(BuoyancyComponent::EntityFloater{ floater.GetUUID(),{} });

					if (m_IsWorld)
						SelectionManager::Select(SelectionContext::Scene, floater.GetUUID());
					else
						AssetSelectionManager::Select(AssetSelectionContext::Prefab, m_PrefabID, floater.GetUUID());
				}
				uint32_t i = 0;
				for (auto& [entityId, floater] : buoyancyComponent.Floaters)
				{
					UI::PushID();

					UI::BeginPropertyGrid();

					UI::AttributeEntity("FloaterEntity", m_ActiveWorld, entityId);
					UI::EndPropertyGrid();
					UI::ShiftCursorX(2.f);

					if (UI::AttributeTreeNode(fmt::format("Attributes ##{}",i), true, 3, 3))
					{
						ImGui::PushID(fmt::format("Floater {}", i).c_str());
						UI::BeginPropertyGrid();

						UI::AttributeDrag("SubmersionDepth", floater.SubmersionDepth, 0.01, 0);
						UI::AttributeDrag("Drag", floater.Drag, 0.25, 0);
						UI::AttributeDrag("Angular Drag", floater.AngularDrag, 0.25);
						UI::AttributeDrag("Buoyancy Strength", floater.BuoyancyStrength, 0.5, 0);

						UI::EndPropertyGrid();
						ImGui::PopID();

						UI::EndTreeNode();
					}
					UI::PopID();
					i++;
				}
			

			});

		DrawComponents<PlayerInputComponent>("Player Input", entity, [](PlayerInputComponent& player) {
			UI::BeginPropertyGrid();

			//UI::EnumCombo("Player", player.InputPlayer);
			//UI::AttributeAssetReference("Player",AssetType::Prefab, player.Player);
			UI::EndPropertyGrid();


			{
				AssetID addInputPopUp = 0;
				if (UI::Widgets::AssetSearchPopup("AddInputContextPopup", AssetType::InputBindingContext, addInputPopUp, UI::UIMemoryAssetTypes::None))
					player.Player->AddInputBinding(AssetManager::GetAsset<InputBindingContext>(addInputPopUp));
			}

			if (UI::AttributeButton("", "AddInputMapping"))
				ImGui::OpenPopup("AddInputContextPopup");
			for (uint32_t i = 0; i < player.Player->GetInputBindingContextList().size(); i++)
			{

				UI::PushID();

				if (UI::AttributeTreeNode(fmt::format("Index {}", i).c_str(), true, 6, 2.0f))
				{
					const InputBindingContextInstance* elevatedInstance = player.Player->GetInputBindingContextInstance(player.Player->GetInputBindingContextList().at(i).InputBindingContext);
					bool active = elevatedInstance->Active;
					if (UI::AttributeBool("Active", active))
						player.Player->SetInputBindingActive(elevatedInstance->InputBindingContext, active);
					AssetID Id = elevatedInstance->InputBindingContext->GetID();
					if (UI::AttributeAssetReference("InputBindingContext", AssetType::InputBindingContext, Id))
					{
						int index = player.Player->GetInputBindingPriority(elevatedInstance->InputBindingContext);
						player.Player->RemoveInputBindingByPriority(index);
						player.Player->AddInputBinding(AssetManager::GetAsset<InputBindingContext>(Id), index);
					}
					UI::EndTreeNode();
				}
				UI::PopID();
			}
			});
	#if 1
		DrawComponents<PlayerHUDComponent>("Player HUD", entity, [](PlayerHUDComponent& playerHud)
			{
				UI::AttributeDrawUITable(playerHud.HudTable);
			});
	#endif
		DrawComponents<WaterComponent>("Water Component", entity, [](WaterComponent& waterComponent)
			{
			#if 1
				auto water = waterComponent.Water;

				//physics
				{
					UI::BeginPropertyGrid();
					UI::AttributeDrag("Density", water->Density);
					UI::EndPropertyGrid();
				}
				UI::Separator();
				WaveType type = water->GetWaveType();
				if (UI::EnumCombo("WaveType", type))
				{
					water->SetWave(type);
					return;
				}

				//WaterSystem::WaterDataInfo& waterData = waterComponent.WaterSystem->WaterData;

				if (waterComponent.Water->GetWaveType() == WaveType::GerstnerWave)
				{
					UI::BeginPropertyGrid();

					GerstnerWave::GerstnerWaveInfo& gerstnerData = waterComponent.Water->GetWave().As<GerstnerWave>()->GerstnerData;
					UI::AttributeDrag("Wave Count", gerstnerData.WaveCount, 0.2f, 0, gerstnerData.MaxGerstnerWavesCount);
					ImGui::Separator();

					UI::AttributeColor("Color", gerstnerData.Color);
					UI::AttributeDrag("Speed", gerstnerData.Speed, 0.25f);
					UI::AttributeDrag("Direction", gerstnerData.WaveDirection, 0.01f);
					UI::AttributeSlider("Distribution", gerstnerData.WaveDistribution, 0, 10);
					UI::AttributeSlider("Spread", gerstnerData.WaveSpread, 0, 1);

					ImGui::Separator();

					UI::AttributeDrag("Min Max Wavelength", gerstnerData.MinMaxWavelength, 0.25, 0.0001, 10000);
					UI::AttributeSlider("WavelengthFallOff", gerstnerData.WavelengthFalloff, 0, 1);
					ImGui::Separator();

					UI::AttributeDrag("Min Max Amplitude", gerstnerData.MinMaxAmplitude, 0.25, 0.0001, 10);
					UI::AttributeSlider("AmplitudeFallOff", gerstnerData.AmplitudeFallOff, 0, 1);
					ImGui::Separator();

					UI::AttributeDrag("Min Max Steepness", gerstnerData.MinMaxSteepness, 0.01, 0, 1);
					UI::AttributeSlider("SteepnessFallOff", gerstnerData.SteepnessFallOff, 0, 1);
					ImGui::Separator();


					UI::AttributeBool("RandomizeSeed", gerstnerData.RandomSeed);

					if (!gerstnerData.RandomSeed)
						UI::AttributeDrag("Seed", gerstnerData.Seed);

					ImGui::Separator();
					UI::AttributeSlider("PlaneSize", gerstnerData.PlaneSize, 1, 1000);
					if (UI::AttributeButton("", "RecomputePlaneSize"))
						waterComponent.Water->GetWave().As<GerstnerWave>()->RegeneratePlane();
					ImGui::Separator();

					UI::AttributeBool("VisualizeWaveHeight", waterComponent.Water->GetWave().As<GerstnerWave>()->VisualizeWaveHeight);
					UI::EndPropertyGrid();

				}

				if (water->GetWaveType() == WaveType::FastFourierTransformWave)
				{
					FFTWave::FFTWaveInfo& waveInfo = water->GetWave().As<FFTWave>()->WaveInfo;
					Count< FFTWave> fftWave = water->GetWave().As<FFTWave>();

					UI::BeginPropertyGrid();
					UI::AttributeColor("Water Colour", waveInfo.WaterColor);
					UI::AttributeColor("Foam Colour", waveInfo.FoamColor);
					UI::AttributeDrag("Normal Strength", waveInfo.NormalStrength,0.01);
					UI::AttributeDrag("Roughness", waveInfo.Roughness, 0.01);
					UI::AttributeSlider("Num Cascades", waveInfo.NumCascades);
					ImGui::Separator();

					UI::AttributeDrag("DisplacementScale", waveInfo.DisplacementScale,0.01);
					UI::AttributeDrag("NormalScale", waveInfo.NormalScale, 0.01);
					UI::AttributeDrag("WindSpeedScale", waveInfo.WindSpeedScale, 0.01);
					UI::AttributeDrag("SwellScale", waveInfo.SwellScale, 0.01);
					UI::AttributeDrag("DetailScale", waveInfo.DetailScale, 0.01);
					UI::AttributeDrag("SpreadScale", waveInfo.SpreadScale, 0.01);
					
					ImGui::Separator();

					UI::AttributeDrag("WhitecapScale", waveInfo.WhitecapScale, 0.01);
					UI::AttributeDrag("FoamGrowthScale", waveInfo.FoamGrowthScale, 0.01);
					UI::AttributeDrag("FoamDecayScale", waveInfo.FoamDecayScale, 0.01);
					UI::AttributeDrag("FoamTimeScale", waveInfo.FoamTimeScale, 0.01);


					ImGui::Separator();

					UI::AttributeBool("View WaveHeight", waveInfo.ViewWaveHeight);

					UI::EndPropertyGrid();

					ImGui::Separator();
					for (auto cascade : fftWave->GetCascades())
					{
						UI::ScopedID scopeID(fmt::format("FFt waterCascade Settings {}", cascade->GetCascadeIndex()).c_str());
						if (UI::AttributeTreeNode(fmt::format("Cascade {}",cascade->GetCascadeIndex()), false, 3, 3))
						{
							auto& settings = cascade->Settings;
							UI::BeginPropertyGrid();

							UI::AttributeDrag("TileLength", settings.TileLength);
							UI::AttributeSlider("DisplacementScale", settings.DisplacementScale);
							UI::AttributeSlider("NormalScale", settings.NormalScale);
							ImGui::Separator();

							UI::AttributeDrag("Wind Speed", settings.WindSpeed);
							UI::AttributeDrag("WindDirection", settings.WindDirection, 0.25);
							UI::AttributeSlider("FetLength", settings.FetchLength);
							UI::AttributeSlider("Swell", settings.Swell);
							UI::AttributeSlider("Detail", settings.Detail);
							UI::AttributeSlider("Spread", settings.Spread);

							ImGui::Separator();
							UI::AttributeDrag("FoamAmount", settings.FoamAmount, 0.01);
							UI::AttributeDrag("WhiteCap", settings.Whitecap, 0.01,"How Steep wave before foam builds up");
							UI::AttributeDrag("FoamTimeScale", settings.FoamTimeScale, 0.01,"How fast foam builds up");

							UI::EndPropertyGrid();

							if (UI::AttributeTreeNode("Debugs", false, 3, 3))
							{
								UI::ViewDebugImage("Spectrum", cascade->SpectrumMap);
								UI::ViewDebugImage("Displacement", cascade->DisplacementMap);
								UI::ViewDebugImage("Normal", cascade->NormalMap);

								UI::EndTreeNode();
							}


							UI::EndTreeNode();
						}
					}

				}
#endif


				if (water->GetWaveType() == WaveType::RealisticFastFourierTransformWave)
				{
					Count<FFTWaveRealistic> fftWave = water->GetWave().As<FFTWaveRealistic>();
					UI::BeginPropertyGrid();
					UI::EndPropertyGrid();
#if 1

					UI::ViewDebugImage("Noise Texure", fftWave->GetNoiseTexture());
					//UI::ViewDebugImage("Precompute dataMap", fftWave->GetPrcomuteData());

					{

						//UI::Image(fftWave->GetPrcomuteData().As<Image>(), ImVec2{ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().x}, {0, 1}, {1, 0});
					}

					for (auto cascade : fftWave->GetCascades())
					{
						UI::ScopedID scopeID(fmt::format("FFt waterCascade Settings {}", cascade->GetCascadeIndex()).c_str());
						if (UI::AttributeTreeNode(fmt::format("Cascade {}", cascade->GetCascadeIndex()), false, 3, 3))
						{
							if (UI::AttributeTreeNode("Debugs", false, 3, 3))
							{
								UI::AttributeText("Initial Spectrum");

								UI::ViewDebugImage("Initial Spectrum Map", cascade->GetInitialSpectrumMap());
								UI::ViewDebugImage("cascade Buffer ma", cascade->GetInitialSpectrumBufferMap());
								UI::ViewDebugImage("Waves data", cascade->GetWavesData());

								UI::Separator();
								UI::AttributeText("Cascade Settings Map");

								UI::ViewDebugImage("Displacement Map", cascade->GetDisplacementMap());
								UI::ViewDebugImage("Turbulence Map", cascade->GetTurbulenceMap());
								UI::ViewDebugImage("Turbulence2 Map", cascade->GetTurbulence2Map());
								UI::ViewDebugImage("Cascade Buffer map", cascade->GetCascadeBufferMap());

								UI::Separator();

								UI::AttributeText("Derivatives");

								UI::ViewDebugImage("Derivatives Map", cascade->GetDerivativesMap());
								UI::ViewDebugImage("DxDz", cascade->GetDxDz());
								UI::ViewDebugImage("DyDxz", cascade->GetDyDxz());
								UI::ViewDebugImage("DyxDyz", cascade->GetDyxDyz());
								UI::ViewDebugImage("DxxDzz", cascade->GetDxxDzz());

								UI::EndTreeNode();
							}
							UI::EndTreeNode();

						}
					}
#endif
				}
				

			});
		DrawComponents<AudioComponent>("Audio", entity, [](AudioComponent& audio)
			{
				UI::BeginPropertyGrid();

				UI::AttributeAssetReference("Audio", AssetType::Audio, audio.AudioAsset);

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
