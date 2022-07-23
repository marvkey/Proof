#pragma once
#include "Proof/Core/Core.h"
#include <ImGui/imgui.h>
#include <imgui/imgui_internal.h>
namespace Proof
{
	namespace ExternalAPI
	{
		class Proof_API ImGUIAPI {
		public:
			static void TextBar(const std::string& Name,const std::string& Text ="null",float ColumnWidth=210.f);
			static void AcceptPayLoad(const std::string& name, uint32_t& Variable);
			static void SendPayLoad(uint32_t& Data,const std::string&DataName,uint32_t* TextureID=nullptr,const char* name=nullptr);
			static void InputText(const std::string& label,std::string& Variable,int ImGuiTextFlags=0);
			static void InputTextBar(const std::string& Name,std::string& Variable,int MaxNumberChar = 1000,int ImGuiTextFlags = 0,bool changeVariable = true,float ColumnWidth = 210.f);
			static bool CheckBox(const std::string& name, bool* variable);
			static void SetKeyboardFocusOff();
			
			template<typename E>
			static bool EnumCombo(const std::string& name, E& enumVar) {
				ImGui::PushID(name.c_str());
				void* val = &enumVar;
				int* valNum = (int*)val;
				auto enumNames = EnumReflection::GetNames<E>();
				std::vector<const char*> charVec(enumNames.size(), nullptr);
				for (int i = 0; i < enumNames.size(); i++) {
					charVec[i] = enumNames[i].c_str();
				}
				ImGui::Text(name.c_str());
				ImGui::SameLine();
				bool temp =ImGui::Combo("##", valNum, charVec.data(), charVec.size(), charVec.size());
				ImGui::PopID();
				return temp;
			}
		};
	}
}

