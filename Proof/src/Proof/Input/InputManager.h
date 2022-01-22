#pragma once
#include <glm/glm.hpp>
#include "KeyCodes.h"
#include "Proof/Events/Event.h"
#include <unordered_map>
namespace Proof {
	struct Action {

	};
	class InputManager {
	public:
		InputManager() = delete;

	private:
		void OnKeyClicked(KeyClickedEvent& e);
		std::unordered_map<std::string, Action> m_InputMapping;
		static void OnEvent(Event& e);
		friend class Application;
	};	
}