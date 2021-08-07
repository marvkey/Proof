#pragma once
#include "Proof/Core/Core.h"
namespace Proof
{
	namespace ExternalAPI
	{
		class Proof_API ImGUIAPI {
		public:
			static void TextBar(const std::string& Name,const std::string& Text ="null",float ColumnWidth=100.f,float TextWidth=100.f);
			static void AcceptPayLoad(const std::string& name, uint32_t& Variable);
			static void SendPayLoad(uint32_t& Data,const std::string&DataName,uint32_t* TextureID=nullptr,const char* name=nullptr);
		};
	}
}

