#include "Proofprch.h"
#include "InputManager.h"
namespace Proof {
	std::unordered_map<std::string, Action> InputManager::m_ActionMapping;
	void InputManager::OnEvent(Event& e){
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrollEvent>(InputManager::OnKeyClicked);
	}
	void InputManager::OnKeyClicked(KeyClickedEvent& e){

	}
}
