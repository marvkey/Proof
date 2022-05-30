#include "ImGUIAPI.h"
#include <ImGui/imgui.h>
#include <imgui/imgui_internal.h>
#include <vector>
#include<iostream>
#include <array>
#
namespace Proof
{
	namespace ExternalAPI
	{
		void ImGUIAPI::TextBar(const std::string& Name,const std::string& Text,float ColumnWidth) {
			ImGui::GetStyle().FrameRounding = 3.5;
			ImGui::PushID(Name.c_str());
			ImGui::Columns(2); 
			ImGui::SetColumnWidth(0,ColumnWidth);
			ImGui::Text(Name.c_str());
			ImGui::NextColumn();

			char buffer[1024];
			memset(buffer,0,sizeof(buffer));
			strcpy_s(buffer,sizeof(buffer),Text.c_str());
			if (ImGui::InputText("##N",buffer,sizeof(buffer),ImGuiInputTextFlags_ReadOnly));
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
		void ImGUIAPI::InputText(const std::string& label,std::string& Variable,int ImGuiTextFlags ) {
			std::vector<char> buffer;

			memset(buffer.data(), 0, buffer.size() * sizeof(char));
			strcpy_s(buffer.data(),buffer.size() * sizeof(char),Variable.c_str());
			if (ImGui::InputText(label.c_str(),buffer.data(), buffer.size())) {
					Variable = buffer.data();
			}
		}
		void ImGUIAPI::InputTextBar(const std::string& Name,std::string& Variable,int MaxNumberChar,int ImGuiTextFlags,bool changeVariable,float ColumnWidth) {
			ImGui::GetStyle().FrameRounding = 3.5;
			ImGui::PushID(Name.c_str());
			ImGui::Columns(2);
			ImGui::SetColumnWidth(0,ColumnWidth);
			ImGui::Text(Name.c_str());
			ImGui::NextColumn();
			std::vector<char> buffer;

			memset(&buffer[0],0,buffer.size() * sizeof(char));
			strcpy_s(&buffer[0],buffer.size() * sizeof(char),Variable.c_str());
			if (ImGui::InputText(Name.c_str(),&buffer[0],buffer.size() * sizeof(char))) {
				if (changeVariable == true && Variable.size() >= MaxNumberChar)
					Variable = buffer[0];
			}
			ImGui::PopID();
			ImGui::Columns(1);
			ImGui::GetStyle().FrameRounding = 6;
		}
		bool ImGUIAPI::CheckBox(const std::string& name,bool* variable) {
			ImGui::GetStyle().FrameRounding = 3.5;
			ImGui::PushID(name.c_str());
			ImGui::Text(name.c_str());
			ImGui::SameLine();
			bool val =ImGui::Checkbox("##N",variable);
			ImGui::GetStyle().FrameRounding = 6;
			ImGui::PopID();
			return val;
		}

		void ImGUIAPI::SetKeyboardFocusOff()
		{
			ImGuiContext& g = *GImGui;
			ImGuiWindow* window = g.CurrentWindow;
			g.FocusRequestNextWindow = nullptr;
			g.FocusRequestNextCounterRegular = window->DC.FocusCounterRegular;
			g.FocusRequestNextCounterTabStop = INT_MAX;
		}

		static void InputText(const std::string& Name,const std::string& Text,float ColumnWidth){
		
			
		}
	}
}