#include "Proofprch.h"
#include "InputManager.h"
#include "Proof/Events/Event.h"
namespace Proof {
	std::unordered_map<std::string, Action> InputManager::m_ActionMapping;
	void InputManager::OnEvent(Event& e){
		EventDispatcher dispatcher(e);
	}
	void InputManager::OnKeyClicked(KeyClickedEvent& e){
		//PF_INFO(e.ToString().c_str());
	}
}
