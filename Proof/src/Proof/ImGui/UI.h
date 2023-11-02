#pragma once
#include <Imgui/imgui.h>
#include "Proof/Core/SmartPointer.h"
#include "Proof/Renderer/Texture.h"
#include "Proof/Resources/EnumReflection.h"
#include "Proof/Math/Math.h"
#include "Proof/Asset/AssetTypes.h"
#include <glm/glm.hpp>
#include <unordered_set>
#define EG_HOVER_THRESHOLD 0.5f
namespace Proof::HeaderFileOnly {
	struct TextureUI {
		static ImTextureID GetTexture(Count<class Image> image);
	};

}
namespace Proof
{
	class FieldStorage;
	class EnumFieldStorage;
	class World;
}
namespace Proof::UI {

	inline void Image(Count<class Image> image, const ImVec2& size, const ImVec2& uv0 = ImVec2(0, 0), const ImVec2& uv1 = ImVec2(1, 1), const ImVec4& tint_col = ImVec4(1, 1, 1, 1), const ImVec4& border_col = ImVec4(0, 0, 0, 0))
	{
		ImGui::Image(HeaderFileOnly::TextureUI::GetTexture(image), size, uv0, uv1, tint_col, border_col);
	}
	inline void Image(Count<Texture2D> image, const ImVec2& size, const ImVec2& uv0 = ImVec2(0, 0), const ImVec2& uv1 = ImVec2(1, 1), const ImVec4& tint_col = ImVec4(1, 1, 1, 1), const ImVec4& border_col = ImVec4(0, 0, 0, 0))
	{
		ImGui::Image(HeaderFileOnly::TextureUI::GetTexture(image->GetImage()), size, uv0, uv1, tint_col, border_col);
	}

	inline bool ImageButton(const std::string& imageId, Count<Image2D> image, const ImVec2& size, const ImVec2& uv0 = ImVec2(0, 0), const ImVec2& uv1 = ImVec2(1, 1), const ImVec4& bg_col = ImVec4(0, 0, 0, 0), const ImVec4& tint_col = ImVec4(1, 1, 1, 1))
	{
		return ImGui::ImageButton(imageId.c_str(), HeaderFileOnly::TextureUI::GetTexture(image), size, uv0, uv1, bg_col, tint_col);
	}
	inline bool ImageButton(const std::string& imageId, Count<Texture2D> image, const ImVec2& size, const ImVec2& uv0 = ImVec2(0, 0), const ImVec2& uv1 = ImVec2(1, 1), const ImVec4& bg_col = ImVec4(0, 0, 0, 0), const ImVec4& tint_col = ImVec4(1, 1, 1, 1))
	{
		return ImGui::ImageButton(imageId.c_str(), HeaderFileOnly::TextureUI::GetTexture(image->GetImage()), size, uv0, uv1, bg_col, tint_col);
	}

	inline bool ImageButton(Count<Image2D> image, const ImVec2& size, const ImVec2& uv0 = ImVec2(0, 0), const ImVec2& uv1 = ImVec2(1, 1), int frame_padding = -1, const ImVec4& bg_col = ImVec4(0, 0, 0, 0), const ImVec4& tint_col = ImVec4(1, 1, 1, 1))
	{
		return ImGui::ImageButton(HeaderFileOnly::TextureUI::GetTexture(image), size, uv0, uv1, frame_padding,bg_col, tint_col);
	}
	inline bool ImageButton(Count<Texture2D> image, const ImVec2& size, const ImVec2& uv0 = ImVec2(0, 0), const ImVec2& uv1 = ImVec2(1, 1), int frame_padding = -1, const ImVec4& bg_col = ImVec4(0, 0, 0, 0), const ImVec4& tint_col = ImVec4(1, 1, 1, 1))
	{
		return ImageButton(image->GetImage(), size, uv0, uv1, frame_padding, bg_col, tint_col);
	}
	//Grid Name needs to be unique
	void BeginPropertyGrid(const std::string& gridName);
	void EndPropertyGrid();

	bool DrawVec3Control(const std::string& label, glm::vec3& values, const glm::vec3 resetValues = glm::vec3{ 0.f }, float columnWidth = 100.f);
	// returns the [is changed,seleection index, out of stirng]
	std::tuple<bool, int, std::string> Combo(const std::string& label, const std::vector<std::string>& options, const std::string& currentSelected, const std::vector<std::string>& tooltips = {}, const std::string& helpMessage ="");

	template<class TEnum>
	bool EnumCombo(const std::string& label, TEnum& value, const std::unordered_set<TEnum>& excludedValues = {},const std::vector<std::string>&tooltips = {}, const std::string & helpMessage = "")
	{
		auto [changed, outSelectionIndex, outSelectiongString] =
			UI::Combo(label, EnumReflection::GetNames<TEnum>(excludedValues), EnumReflection::EnumString(value),
				tooltips,helpMessage);

		if(changed)
			value = EnumReflection::StringEnum< TEnum>(outSelectiongString);

		return changed;
	}

	bool AttributeInputText(const std::string& label, std::string& value, const std::string& helpMessage = "");
	bool AttributeBool(const std::string& label, bool& value, const std::string& helpMessage = "");
	bool AttributeLabels(const std::string& label, const std::vector<std::string>& customLabels, bool* values, const std::string& helpMessage = "");

	bool AttributeTextBar(const std::string& label, const std::string& text);
	bool AttributeText(const std::string& text);

	// does the drag adn drop
	bool AttributeAssetTextBar(const std::string& label, AssetID& ID, AssetType type, bool includeRemove = true);
	bool AttributeAssetTextBar(const std::string& label, Count<class Asset> asset, AssetType type, bool includeRemove = true);
	bool AttributeEntity(const std::string& label,Count < class World > worldContext, UUID& entityID);

	bool AttributeSlider(const std::string& label, uint8_t& value, uint8_t min, uint8_t max, const std::string& helpMessage = "", ImGuiSliderFlags flags = ImGuiSliderFlags_AlwaysClamp, const char* format = "%d");
	bool AttributeSlider(const std::string& label, uint16_t& value, uint16_t min, uint16_t max, const std::string& helpMessage = "", ImGuiSliderFlags flags = ImGuiSliderFlags_AlwaysClamp, const char* format = "%d");
	bool AttributeSlider(const std::string& label, uint32_t& value, uint32_t min, uint32_t max, const std::string& helpMessage = "", ImGuiSliderFlags flags = ImGuiSliderFlags_AlwaysClamp, const char* format = "%d");
	bool AttributeSlider(const std::string& label, uint64_t& value, uint64_t min, uint64_t max, const std::string& helpMessage = "", ImGuiSliderFlags flags = ImGuiSliderFlags_AlwaysClamp, const char* format = "%d");

	// For signed data types
	bool AttributeSlider(const std::string& label, int8_t& value, int8_t min, int8_t max, const std::string& helpMessage = "", ImGuiSliderFlags flags = ImGuiSliderFlags_AlwaysClamp, const char* format = "%d");
	bool AttributeSlider(const std::string& label, int16_t& value, int16_t min, int16_t max, const std::string& helpMessage = "", ImGuiSliderFlags flags = ImGuiSliderFlags_AlwaysClamp, const char* format = "%d");
	bool AttributeSlider(const std::string& label, int32_t& value, int32_t min, int32_t max, const std::string& helpMessage = "", ImGuiSliderFlags flags = ImGuiSliderFlags_AlwaysClamp, const char* format = "%d");
	bool AttributeSlider(const std::string& label, int64_t& value, int64_t min, int64_t max, const std::string& helpMessage = "", ImGuiSliderFlags flags = ImGuiSliderFlags_AlwaysClamp, const char* format = "%d");

	// For floating-point data types
	bool AttributeSlider(const std::string& label, float& value, float min, float max, const std::string& helpMessage = "", ImGuiSliderFlags flags = ImGuiSliderFlags_AlwaysClamp, const char* format = "%.3f");
	bool AttributeSlider(const std::string& label, double& value, double min, double max, const std::string& helpMessage = "", ImGuiSliderFlags flags = ImGuiSliderFlags_AlwaysClamp, const char* format = "%.3f");

	bool AttributeSlider(const std::string& label, glm::vec2& value, float min, float max, const std::string& helpMessage = "", ImGuiSliderFlags flags = ImGuiSliderFlags_AlwaysClamp, const char* format = "%.3f");
	bool AttributeSlider(const std::string& label, glm::vec3& value, float min, float max, const std::string& helpMessage = "", ImGuiSliderFlags flags = ImGuiSliderFlags_AlwaysClamp, const char* format = "%.3f");
	bool AttributeSlider(const std::string& label, glm::vec4& value, float min, float max, const std::string& helpMessage = "",ImGuiSliderFlags flags = ImGuiSliderFlags_AlwaysClamp, const char* format = "%.3f")	;

	bool AttributeDrag(const std::string& label, int8_t& value, float speed = 1, int8_t min = Math::GetMinType<int8_t>(), int8_t max = Math::GetMaxType<int8_t>(), const std::string& helpMessage = "", ImGuiSliderFlags flags = ImGuiSliderFlags_AlwaysClamp, const char* format = "%d");
	bool AttributeDrag(const std::string& label, int16_t& value, float speed = 1, int16_t min = Math::GetMinType<int16_t>(), int16_t max = Math::GetMaxType<int16_t>(), const std::string& helpMessage = "", ImGuiSliderFlags flags = ImGuiSliderFlags_AlwaysClamp, const char* format = "%d");
	bool AttributeDrag(const std::string& label, int32_t& value, float speed = 1, int32_t min = Math::GetMinType<int32_t>(), int32_t max = Math::GetMaxType<int32_t>(), const std::string& helpMessage = "", ImGuiSliderFlags flags = ImGuiSliderFlags_AlwaysClamp, const char* format = "%d");
	bool AttributeDrag(const std::string& label, int64_t& value, float speed = 1, int64_t min = Math::GetMinType<int64_t>(), int64_t max = Math::GetMaxType<int64_t>(), const std::string& helpMessage = "", ImGuiSliderFlags flags = ImGuiSliderFlags_AlwaysClamp, const char* format = "%d");

	bool AttributeDrag(const std::string& label, uint8_t& value, float speed = 1, uint8_t min = Math::GetMinType<uint8_t>(), uint8_t max = Math::GetMaxType<uint8_t>(), const std::string& helpMessage = "", ImGuiSliderFlags flags = ImGuiSliderFlags_AlwaysClamp, const char* format = "%d");
	bool AttributeDrag(const std::string& label, uint16_t& value, float speed = 1, uint16_t min = Math::GetMinType<uint16_t>(), uint16_t max = Math::GetMaxType<uint16_t>(), const std::string& helpMessage = "", ImGuiSliderFlags flags = ImGuiSliderFlags_AlwaysClamp, const char* format = "%d");
	bool AttributeDrag(const std::string& label, uint32_t& value, float speed = 1, uint32_t min = Math::GetMinType<uint32_t>(), uint32_t max = Math::GetMaxType<uint32_t>(), const std::string& helpMessage = "", ImGuiSliderFlags flags = ImGuiSliderFlags_AlwaysClamp, const char* format = "%d");
	bool AttributeDrag(const std::string& label, uint64_t& value, float speed = 1, uint64_t min = Math::GetMinType<uint64_t>(), uint64_t max = Math::GetMaxType<uint64_t>(), const std::string& helpMessage = "", ImGuiSliderFlags flags = ImGuiSliderFlags_AlwaysClamp, const char* format = "%d");

	bool AttributeDrag(const std::string& label, float& value, float speed = 1.f, float min = Math::GetMinType<float>(), float max = Math::GetMaxType<float>(), const std::string& helpMessage = "", ImGuiSliderFlags flags = ImGuiSliderFlags_AlwaysClamp, const char* format = "%.3f");
	bool AttributeDrag(const std::string& label, double& value, float speed = 1.f, double min = Math::GetMinType<double>(), double max = Math::GetMaxType<double>(), const std::string& helpMessage = "",ImGuiSliderFlags flags = ImGuiSliderFlags_AlwaysClamp, const char* format = "%.3f");

	bool AttributeDrag(const std::string& label, glm::vec2& value, float speed = 1.f, float min = Math::GetMinType<float>(), float max = Math::GetMaxType<float>(), const std::string& helpMessage = "", ImGuiSliderFlags flags = ImGuiSliderFlags_AlwaysClamp, const char* format = "%.3f");
	bool AttributeDrag(const std::string& label, glm::vec3& value, float speed = 1.f, float min = Math::GetMinType<float>(), float max = Math::GetMaxType<float>(), const std::string& helpMessage = "", ImGuiSliderFlags flags = ImGuiSliderFlags_AlwaysClamp, const char* format = "%.3f");
	bool AttributeDrag(const std::string& label, glm::vec4& value, float speed = 1.f, float min = Math::GetMinType<float>(), float max = Math::GetMaxType<float>(), const std::string& helpMessage = "",ImGuiSliderFlags flags = ImGuiSliderFlags_AlwaysClamp, const char* format = "%.3f");

	bool AttributeColor(const std::string& label, glm::vec3& value, const std::string& helpMessage = "");
	bool AttributeColor(const std::string& label, glm::vec4& value, const std::string& helpMessage = "");

	bool AttributeButton(const std::string& label, const std::string& buttonText, const ImVec2& size = ImVec2(0, 0));

	bool AttributeInputTextMultiline(const std::string& label, std::string& value, ImGuiInputTextFlags flags);
	void PushItemDisabled();
	void PopItemDisabled();

	void Tooltip(const std::string& tooltip, float treshHold = EG_HOVER_THRESHOLD);
	void HelpMarker(const std::string& text);
	bool DrawFieldValue(Count < World > worldContext, const std::string& fieldName, Count<FieldStorage>& storage);
	bool DrawFieldValue(Count < World > worldContext, const std::string& fieldName,Count<EnumFieldStorage>& storage);

	struct ScopedStyleColor 
	{
		ScopedStyleColor() = default;
		ScopedStyleColor(ImGuiCol idx, ImVec4 color, bool predicate = true)
			: m_Set(predicate)
		{
			if (predicate)
				ImGui::PushStyleColor(idx, color);
		}

		ScopedStyleColor(ImGuiCol idx, ImU32 color, bool predicate = true)
			: m_Set(predicate)
		{
			if (predicate)
				ImGui::PushStyleColor(idx, color);
		}

		~ScopedStyleColor()
		{
			if (m_Set)
				ImGui::PopStyleColor();
		}
	private:
		bool m_Set = false;
	};

	struct ScopedStyleVar 
	{
		ScopedStyleVar() = default;
		ScopedStyleVar(ImGuiStyleVar idx, float val, bool predicate = true)
			: m_Set(predicate)
		{
			if (predicate)
				ImGui::PushStyleVar(idx, val);
		}

		ScopedStyleVar(ImGuiStyleVar idx, const ImVec2& val, bool predicate = true)
			: m_Set(predicate)
		{
			if (predicate)
				ImGui::PushStyleVar(idx, val);
		}

		~ScopedStyleVar()
		{
			if (m_Set)
				ImGui::PopStyleVar();
		}
	private:
		bool m_Set = false;
	};

	struct ScopedID {
		ScopedID() = default;
		ScopedID(std::string str_id, bool predicate = true)
			: m_Set(predicate)
		{
			if (predicate)
				ImGui::PushID(str_id.c_str());
		}
		ScopedID(const char* str_id, bool predicate = true)
			: m_Set(predicate)
		{
			if (predicate)
				ImGui::PushID(str_id);
		}

		ScopedID(const char* str_id_begin, const char* str_id_end, bool predicate = true)
			: m_Set(predicate)
		{
			if (predicate)
				ImGui::PushID(str_id_begin, str_id_end);
		}
		ScopedID(int int_id, bool predicate = true)
			: m_Set(predicate)
		{
			if (predicate)
				ImGui::PushID(int_id);
		}

		ScopedID(const void* ptr_id,bool predicate = true)
			: m_Set(predicate)
		{
			if (predicate)
				ImGui::PushID(ptr_id);
		}
		~ScopedID()
		{
			if (m_Set)
				ImGui::PopID();
		}
	private:
		bool m_Set = false;
	};
}
