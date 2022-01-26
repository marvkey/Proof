#include "Proofprch.h"
#include "InputManager.h"
#include "Proof/Events/Event.h"
namespace Proof {
	std::unordered_map<std::string, Action> InputManager::S_ActionMapping;
	void InputManager::OnEvent(Event& e){
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch< KeyClickedEvent>(InputManager::OnKeyClicked);
	}
	void InputManager::OnKeyClicked(KeyClickedEvent& e){
		for (const auto& [name, action] : S_ActionMapping) {
			for (auto& inputs : action.m_Inputs) {
				if (inputs.Key == (int)e.GetKey()) {
					action.FunctionCallback();
				}

			}
		}
	}
}
