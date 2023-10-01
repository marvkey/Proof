#pragma once
#include <Imgui/imgui.h>
#include "Proof/Core/SmartPointer.h"
#include "Proof/Renderer/Texture.h"
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

	// returns the [changed,eum selected]
	inline std::pair<bool, std::string> EnumCombo(const std::string& name, const std::vector<std::string>& names, const std::string& currentSelected)
	{
		ImGui::Text(name.c_str());
		std::string id= fmt::format("##{}", name);

		std::string returnValue = currentSelected;
		bool isChanged = false;
		if (ImGui::BeginCombo(id.c_str(), currentSelected.c_str()))
		{
			for (auto& containerName : names)
			{
				bool isSelected = (currentSelected == containerName);
				if (ImGui::Selectable(containerName.c_str(), isSelected))
				{
					returnValue = containerName;
					isChanged = true;
				}
				if (isSelected)
				{
					ImGui::SetItemDefaultFocus();
				}
			}
			ImGui::EndCombo();
		}
		return { isChanged, returnValue };
	}

	inline bool AttributeSlider(const std::string& label,float& value, float min = 0, float max = 0, ImGuiSliderFlags flags = ImGuiSliderFlags_AlwaysClamp, const char* format = "%.3f")
	{
		if(min == 0 && max ==0)
			flags = flags & ~ImGuiSliderFlags_AlwaysClamp;

		bool modfified = false;
		std::string id = fmt::format("##{}", label);
		ImGui::Text(label.c_str());
		ImGui::SameLine();
		modfified = ImGui::SliderFloat(id.c_str(), &value, min, max,format, flags);
		return modfified;
	}

	inline bool AttributeDrag(const std::string& label, uint32_t& value, float speed = 1, uint32_t min = 0, uint32_t max = 0, ImGuiSliderFlags flags = ImGuiSliderFlags_AlwaysClamp, const char* format = "%d")
	{
		if (min == 0 && max == 0)
			flags = flags & ~ImGuiSliderFlags_AlwaysClamp;

		bool modfified = false;
		std::string id = fmt::format("##{}", label);
		ImGui::Text(label.c_str());
		ImGui::SameLine();
		modfified = ImGui::DragScalar(id.c_str(),ImGuiDataType_U32, &value, speed, &min, &max, format, flags);
		return modfified;
	}

	inline bool AttributeDrag(const std::string& label, float& value, float speed =1.f , float min = 0, float max = 0, ImGuiSliderFlags flags = ImGuiSliderFlags_AlwaysClamp, const char* format = "%.3f")
	{
		if (min == 0 && max == 0 )
			flags = flags & ~ImGuiSliderFlags_AlwaysClamp;

		bool modfified = false;
		std::string id = fmt::format("##{}", label);
		ImGui::Text(label.c_str());
		ImGui::SameLine();
		modfified = ImGui::DragFloat(id.c_str(), &value, speed, min, max, format, flags);
		return modfified;
	}

	bool AttributeInputTextMultiline(const std::string& label, std::string& value, ImGuiInputTextFlags flags);
	struct ScopedStyleColor {
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

	struct ScopedStyleVar {
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
