#pragma once
#include <mono/jit/jit.h>
#include<mono/metadata/assembly.h>
#include<mono/metadata/debug-helpers.h>
namespace Proof
{
	class InitlizeCSharp{
	public:
		InitlizeCSharp();
		virtual ~InitlizeCSharp();
		void OnUpdate(float deltaTime);
	private:
		MonoDomain*m_Domain =nullptr;
		MonoAssembly* m_Assembly =nullptr;
		MonoImage* m_Image =nullptr;
	};
}