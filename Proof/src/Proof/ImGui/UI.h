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

	}

	bool AttributeInputText(const std::string& label, std::string& value, const std::string& helpMessage = "");
	bool AttributeBool(const std::string& label, bool& value, const std::string& helpMessage = "");
	bool AttributeLabels(const std::string& label, const std::vector<std::string>& customLabels, bool* values, const std::string& helpMessage = "");

	bool AttributeTextBar(const std::string& label, const std::string& text);
	bool AttributeText(const std::string& text);

	// does the drag adn drop
	bool AttributeAssetTextBar(const std::string& label, AssetID& ID, AssetType type, bool includeRemove = true);
	bool AttributeAssetTextBar(const std::string& label, Count<class Asset> asset, AssetType type, bool includeRemove = true);

	bool AttributeSlider(const std::string& label, int& value, int min = Math::GetMinType<int>(), int max = Math::GetMaxType<int>(), const std::string& helpMessage = "", ImGuiSliderFlags flags = ImGuiSliderFlags_AlwaysClamp, const char* format = "%.3f");
	bool AttributeSlider(const std::string& label, float& value, float min = Math::GetMinType<float>(), float max = Math::GetMaxType<float>(), const std::string& helpMessage ="", ImGuiSliderFlags flags = ImGuiSliderFlags_AlwaysClamp, const char* format = "%.3f");
	bool AttributeSlider(const std::string& label, glm::vec2& value, float min = Math::GetMinType<float>(), float max = Math::GetMaxType<float>(), const std::string& helpMessage = "", ImGuiSliderFlags flags = ImGuiSliderFlags_AlwaysClamp, const char* format = "%.3f");
	bool AttributeSlider(const std::string& label, glm::vec3& value, float min = Math::GetMinType<float>(), float max = Math::GetMaxType<float>(), const std::string& helpMessage = "", ImGuiSliderFlags flags = ImGuiSliderFlags_AlwaysClamp, const char* format = "%.3f");
	bool AttributeSlider(const std::string& label, glm::vec4& value, float min = Math::GetMinType<float>(), float max = Math::GetMaxType<float>(), const std::string& helpMessage = "",ImGuiSliderFlags flags = ImGuiSliderFlags_AlwaysClamp, const char* format = "%.3f")	;

	bool AttributeDrag(const std::string& label, uint32_t& value, float speed = 1, uint32_t min = Math::GetMinType<uint32_t>(), uint32_t max = Math::GetMaxType<uint32_t>(), const std::string& helpMessage = "", ImGuiSliderFlags flags = ImGuiSliderFlags_AlwaysClamp, const char* format = "%d");
	bool AttributeDrag(const std::string& label, float& value, float speed = 1.f, float min = Math::GetMinType<float>(), float max = Math::GetMaxType<float>(), const std::string& helpMessage = "",ImGuiSliderFlags flags = ImGuiSliderFlags_AlwaysClamp, const char* format = "%.3f");
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
