#pragma once
#include "Proof/Core/Core.h"
#include "Proof/Core/UUID.h"
#include <Imgui.h>
namespace Proof{

	class Panel : public RefCounted 
	{
	public:
		virtual ~Panel() {};
		Panel() {};
		virtual void OnImGuiRender(const char* dsiplayName,bool& isOpen) = 0;
		virtual void SetWorldContext(const Count<class World>& world) {}
		virtual void OnEvent(class Event& e) {}
		friend class Editore3D;
	protected:
		UUID32 GetCustomPushID() { return m_CustomPushID; };
	private:
		UUID32 m_CustomPushID = UUID32();

	};
};