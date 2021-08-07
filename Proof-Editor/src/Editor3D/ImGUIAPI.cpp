#include "ImGUIAPI.h"
#include <ImGui/imgui.h>
#include <imgui/imgui_internal.h>

namespace Proof
{
	namespace ExternalAPI
	{
		void ImGUIAPI::TextBar(const std::string& Name,const std::string& Text,float ColumnWidth,float TextWidth) {
			ImGui::GetStyle().FrameRounding = 0;
			ImGui::PushID(Name.c_str());
			ImGui::Columns(2); // distance between label and edits
			ImGui::SetColumnWidth(0,ColumnWidth);
			ImGui::Text(Name.c_str());
			ImGui::NextColumn();
			char buffer[1024];
			memset(buffer,0,sizeof(buffer));
			strcpy_s(buffer,sizeof(buffer),Text.c_str());
			ImGui::PushMultiItemsWidths(1,ImGui::CalcItemWidth());

			ImGui::SetNextItemWidth(TextWidth);
			if (ImGui::InputText("##N",buffer,sizeof(buffer),ImGuiInputTextFlags_ReadOnly));
			ImGui::PopItemWidth();
			ImGui::PopID();
			ImGui::Columns(1);
			ImGui::GetStyle().FrameRounding = 6;

		}
		void ImGUIAPI::AcceptPayLoad(const std::string& name,uint32_t& Variable) {
			if (ImGui::BeginDragDropTarget()) {
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(name.c_str())) {
					uint32_t Data = *(const uint32_t*)payload->Data;
					Variable = Data;
				}
				ImGui::EndDragDropTarget();
			}
		}
		void ImGUIAPI::SendPayLoad(uint32_t& Data,const std::string& DataName,uint32_t* TextureID,const char* name) {
			if (ImGui::BeginDragDropSource()) {
				ImGui::SetDragDropPayload(DataName.c_str(),&Data,sizeof(Data));
				if (TextureID != nullptr) {
					ImGui::Image((ImTextureID)*TextureID,{0,60});
				}
				if (name != nullptr) {
					ImGui::Text(name);
				}
				ImGui::EndDragDropSource();
			}
		}
	}
}