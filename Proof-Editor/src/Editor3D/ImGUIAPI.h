#pragma once
#include "Proof/Core/Core.h"
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
			static void CheckBox(const std::string& name, bool* variable);
			static void SetKeyboardFocusOff();
			
		};
	}
}

