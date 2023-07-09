#pragma once
#include <Imgui/imgui.h>
#include "Proof/Core/SmartPointer.h"
#include "Proof/Renderer/Texture.h"
namespace Proof::HeaderFileOnly {
	struct TextureUI {
		static ImTextureID GetTexture(Count<Image2D> image);
	};

}
namespace Proof::UI {

	inline void Image(Count<Image2D> image, const ImVec2& size, const ImVec2& uv0 = ImVec2(0, 0), const ImVec2& uv1 = ImVec2(1, 1), const ImVec4& tint_col = ImVec4(1, 1, 1, 1), const ImVec4& border_col = ImVec4(0, 0, 0, 0))
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
