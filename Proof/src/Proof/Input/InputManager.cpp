#include "Proofprch.h"
#include "InputManager.h"
#include "Proof/Input/InputManager.h"
#include "Proof/Input/Controller.h"
#include "Proof/Events/Event.h"
namespace Proof {


	// KEYBAORD AND MOUSE FOR NOW ARE SET TO PLAYER 1
	struct InputManagerData 
	{
		std::unordered_map<std::string, Action> ActionMapping;
		std::unordered_map<std::string, Motion> MotionMapping;
		// (pleyer index, (action name, actuion))
		std::unordered_map<uint32_t, std::unordered_map<std::string, PlayerActions >> PlayerActions;
		// (pleyer index, (motion name, Motion))
		std::unordered_map<uint32_t, std::unordered_map<std::string, PlayerMotion>> PlayerMotions;

		bool Runtime = false;
		uint32_t PlayerCount = 0;
	};
	InputManagerData* s_Data;

	void InputManagerMeathods::SetPlayer(uint32_t player)
	{
		s_Data->PlayerActions[player] = {};
		s_Data->PlayerMotions[player] = {};
	}

	void InputManagerMeathods::DeletePlayer(uint32_t player)
	{
		PF_CORE_ASSERT(HasPlayer(player),"Does not contain player index");

		s_Data->PlayerActions.erase(player);
		s_Data->PlayerMotions.erase(player);
	}

	bool InputManagerMeathods::HasPlayer(uint32_t player)
	{
		return s_Data->PlayerActions.contains(player);
	}
	void InputManagerMeathods::BindAction(const std::string& name, uint32_t player, InputEvent inputEvent, const std::function<void()>& func) 
	{
		PF_CORE_ASSERT(HasPlayer(player), "Does not contain player");

		auto& actionMapping = s_Data->PlayerActions[player];


		if (!s_Data->ActionMapping.contains(name))
		{
			PF_EC_WARN("Player {} Tried binding Action to {} wich does not exist",player, name);
			return;
		}

		actionMapping[name].ActionName = name;
		actionMapping[name].Action = func;
		actionMapping[name].Event = inputEvent;
	}
	void InputManagerMeathods::BindMotion(const std::string& name, uint32_t player, const std::function<void(float)>& func)
	{
		PF_CORE_ASSERT(HasPlayer(player), "Does not contain player");

		auto& motionMapping = s_Data->PlayerMotions[player];

		if (!s_Data->MotionMapping.contains(name))
		{
			PF_EC_WARN("Player {} Tried binding motion to {} wich does not exist", player, name);
			return;
		}
		motionMapping[name].MotionName = name;
		motionMapping[name].Action = func;
	}

	void InputManagerMeathods::CallAction(const std::string& name, uint32_t player, InputEvent inputEvent)
	{
		PF_CORE_ASSERT(HasPlayer(player), "Does not contain player");
		auto& actionMapping = s_Data->PlayerActions[player];
		if (actionMapping.contains(name))
		{
			auto& action = actionMapping[name];
			if(action.Event == inputEvent)
				action.Action();
		}
	}
	void InputManagerMeathods::CallMotion(const std::string& name, uint32_t player, float motionValue)
	{
		PF_CORE_ASSERT(HasPlayer(player), "Does not contain player");
		auto& motionMapping = s_Data->PlayerMotions[player];
		if (motionMapping.contains(name))
		{
			motionMapping[name].Action(motionValue);
		}
	}
	void InputManager::ActionAddKey(const std::string& name, InputType inputype){
		PF_CORE_ASSERT(HasAction(name), fmt::format("Input manager cannot add aciton {} when it has not been added", name));
		auto& action =s_Data->ActionMapping[name];
		action.Inputs[inputype.Device].emplace_back(inputype);
	}
	
	bool InputManager::AddAction(const std::string& name){
		if (s_Data->ActionMapping.contains(name))
			return false;
		s_Data->ActionMapping[name] =Action() ;
		return true;
	}

	bool InputManager::HasMotion(const std::string& name) 
	{
		return s_Data->MotionMapping.contains(name);
	}
	bool InputManager::HasAction(const std::string& name) {
		return s_Data->ActionMapping.contains(name);
	}
	void InputManager::MotionAddKey(const std::string& name, MotionInputType inputType) {
		PF_CORE_ASSERT(HasMotion(name), fmt::format("Input manager cannot add Motion {} when it has not been added", name));
		auto& motion = s_Data->MotionMapping[name];
		motion.Inputs[inputType.Device].emplace_back(inputType);
	}

	bool InputManager::AddMotion(const std::string& name) {
		if (s_Data->MotionMapping.contains(name))
			return false;
		s_Data->MotionMapping[name] = Motion();
		return true;
	}
	
	void InputManager::StartRuntime(uint32_t playerCount)
	{
		s_Data->Runtime = true;
		s_Data->PlayerCount = playerCount;

		auto& controllers = Application::Get()->GetWindow()->s_Controllers;

		int currentPlayer = 0;
		for (auto [ID, contoller] : controllers)
		{
			if (playerCount > ID)
			{
				contoller.Player = (Players)currentPlayer;
			}
			currentPlayer++;
		}
	}

	void InputManager::EndRuntime()
	{
		s_Data->PlayerActions.clear();
		s_Data->PlayerMotions.clear();

		s_Data->Runtime = false;
	}


	void OnKeyHold(KeyHoldEvent& e){

		for (const auto& [name, action] : s_Data->ActionMapping) {
			// checking if key hold is an available format
			if (!action.Inputs.contains(InputDevice::KeyBoard))
				continue;

			auto& data = action.Inputs.at(InputDevice::KeyBoard);
			for (auto& inputs : data)
			{
				if (inputs.Key == (int)e.GetKey())
				{
					InputManagerMeathods::CallAction(name, (uint32_t)Players::Player0, InputEvent::KeyHold);
				}
			}
		}
	}
	void OnKeyDoubleClicked(KeyDoubleClickEvent& e){
		for (const auto& [name, action] : s_Data->ActionMapping)
		{
			// checking if key hold is an available format
			if (!action.Inputs.contains(InputDevice::KeyBoard))
				continue;

			auto& data = action.Inputs.at(InputDevice::KeyBoard);
			for (auto& inputs : data)
			{
				if (inputs.Key == (int)e.GetKey())
				{
					InputManagerMeathods::CallAction(name, (uint32_t)Players::Player0, InputEvent::KeyDouble);
				}
			}
		}
	}
	void OnKeyReleased(KeyReleasedEvent& e){
		for (const auto& [name, action] : s_Data->ActionMapping)
		{
			// checking if key hold is an available format
			if (!action.Inputs.contains(InputDevice::KeyBoard))
				continue;
			auto& data = action.Inputs.at(InputDevice::KeyBoard);
			for (auto& inputs : data)
			{
				if (inputs.Key == (int)e.GetKey())
				{
					InputManagerMeathods::CallAction(name, (uint32_t)Players::Player0, InputEvent::KeyReleased);
				}
			}
		}
	}
	
	void OnKeyClicked(KeyClickedEvent& e){
		for (const auto& [name, action] : s_Data->ActionMapping)
		{
			// checking if key hold is an available format
			if (!action.Inputs.contains(InputDevice::KeyBoard))
				continue;
			auto& data = action.Inputs.at(InputDevice::KeyBoard);
			for (auto& inputs : data)
			{
				if (inputs.Key == (int)e.GetKey())
				{
					InputManagerMeathods::CallAction(name, (uint32_t)Players::Player0, InputEvent::KeyClicked);
				}
			}
		}
	}
	void OnKeyPressed(KeyPressedEvent& e){
		// Motion movement
		{
			for (const auto& [name, action] : s_Data->MotionMapping)
			{
				// checking if key hold is an available format
				if (!action.Inputs.contains(InputDevice::KeyBoard))
					continue;
				auto& data = action.Inputs.at(InputDevice::KeyBoard);
				for (auto& inputs : data)
				{
					if (inputs.Key == (int)e.GetKey())
					{
						InputManagerMeathods::CallMotion(name, (uint32_t)Players::Player0, inputs.MotionValue);
					}
				}
			}
		}

		// Action 
		{
			for (const auto& [name, action] : s_Data->ActionMapping)
			{
				// checking if key hold is an available format
				if (!action.Inputs.contains(InputDevice::KeyBoard))
					continue;
				auto& data = action.Inputs.at(InputDevice::KeyBoard);
				for (auto& inputs : data)
				{
					if (inputs.Key == (int)e.GetKey())
					{
						InputManagerMeathods::CallAction(name, (uint32_t)Players::Player0, InputEvent::KeyPressed);
					}
				}
			}
		}
	}

	void OnMouseClicked(MouseButtonClickedEvent& e){
		for (const auto& [name, action] : s_Data->ActionMapping) {
			// checking if key hold is an available format
			if (!action.Inputs.contains(InputDevice::MouseButton))
				continue;
			auto& data = action.Inputs.at(InputDevice::MouseButton);
			for (auto& inputs : data)
			{
				if (inputs.Key == (int)e.GetButton())
				{
					InputManagerMeathods::CallAction(name, (uint32_t)Players::Player0, InputEvent::KeyClicked);
				}
			}
		}
	}

	void OnMousePressed(MouseButtonPressedEvent& e){
		for (const auto& [name, action] : s_Data->ActionMapping)
		{
			// checking if key hold is an available format
			if (!action.Inputs.contains(InputDevice::MouseButton))
				continue;
			auto& data = action.Inputs.at(InputDevice::MouseButton);
			for (auto& inputs : data)
			{
				if (inputs.Key == (int)e.GetButton())
				{
					InputManagerMeathods::CallAction(name, (uint32_t)Players::Player0, InputEvent::KeyPressed);
				}
			}
		}
		// Motion movement
		{
			for (const auto& [name, motion] : s_Data->MotionMapping) {
				if (!motion.Inputs.contains(InputDevice::MouseMovement))
					continue;
				auto& data = motion.Inputs.at(InputDevice::MouseMovement);
				for (auto& inputs : data)
				{
					if (inputs.Key == (int)e.GetButton())
					{
						InputManagerMeathods::CallMotion(name, (uint32_t)Players::Player0, inputs.MotionValue);
					}
				}
			}
		}
	}

	void OnMouseReleased(MouseButtonReleasedEvent& e){
		for (const auto& [name, action] : s_Data->ActionMapping)
		{
			// checking if key hold is an available format
			if (!action.Inputs.contains(InputDevice::MouseButton))
				continue;

			auto& data = action.Inputs.at(InputDevice::MouseButton);
			for (auto& inputs : data)
			{
				if (inputs.Key == (int)e.GetButton())
				{
					InputManagerMeathods::CallAction(name, (uint32_t)Players::Player0, InputEvent::KeyReleased);
				}
			}
		}
	}
	
	void OnMouseMoved(MouseMoveEvent& e){
		float movedX = e.GetMovedX();
		float movedY = e.GetMovedY();
		// Motion Movement
		for (const auto& [name, motion] : s_Data->MotionMapping)
		{
			if (motion.Inputs.contains(InputDevice::MouseMovement))
			{
				auto& data = motion.Inputs.at(InputDevice::MouseMovement);
				for (auto& inputs : data)
				{
					if (inputs.Key == (int)MouseAxis::X)
					{
						InputManagerMeathods::CallMotion(name, (uint32_t)Players::Player0, inputs.MotionValue * movedX);
					}
					else if (inputs.Key == (int)MouseAxis::Y)
					{
						InputManagerMeathods::CallMotion(name, (uint32_t)Players::Player0, inputs.MotionValue* movedY);
					}
				}
			}
		}
	}
	void OnControllerClicked(ControllerButtonClickedEvent& e) {
		for (const auto& [name, action] : s_Data->ActionMapping)
		{
				// checking if key hold is an available format
			if (!action.Inputs.contains(InputDevice::ControllerButton))
				continue;

			auto& data = action.Inputs.at(InputDevice::ControllerButton);
			for (auto& inputs : data)
			{
				if (inputs.Key == (int)e.GetButton())
				{
					Controller& controller = Application::Get()->GetWindow()->GetController(e.GetIndex());
					InputManagerMeathods::CallAction(name, (uint32_t)controller.Player, InputEvent::KeyClicked);
				}
			}
		}
	}
	void OnControllerPressed(ControllerButtonPressedEvent& e){
		// ACTION
		{
			for (const auto& [name, action] : s_Data->ActionMapping)
			{
					// checking if key hold is an available format
				if (!action.Inputs.contains(InputDevice::ControllerButton))
					continue;

				auto& data = action.Inputs.at(InputDevice::ControllerButton);
				for (auto& inputs : data)
				{
					if (inputs.Key == (int)e.GetButton())
					{
						Controller& controller = Application::Get()->GetWindow()->GetController(e.GetIndex());
						InputManagerMeathods::CallAction(name, (uint32_t)controller.Player, InputEvent::KeyPressed);
					}
				}
			}
		}
		// M0TIION
		{
			for (const auto& [name, motion] : s_Data->MotionMapping) {
					// checking if key hold is an available format
				if (!motion.Inputs.contains(InputDevice::ControllerButton))
					continue;

				auto& data = motion.Inputs.at(InputDevice::ControllerButton);
				for (auto& inputs : data)
				{
					if (inputs.Key == (int)e.GetButton())
					{
						Controller& controller = Application::Get()->GetWindow()->GetController(e.GetIndex());
						InputManagerMeathods::CallMotion(name, (uint32_t)controller.Player, inputs.MotionValue);
					}
				}
			}
		}
	}
	void OnControllerReleased(ControllerButtonReleasedEvent& e){
		for (const auto& [name, action] : s_Data->ActionMapping)
		{
				// checking if key hold is an available format
			if (!action.Inputs.contains(InputDevice::ControllerButton))
				continue;

			auto& data = action.Inputs.at(InputDevice::ControllerButton);
			for (auto& inputs : data)
			{
				if (inputs.Key == (int)e.GetButton())
				{
					Controller& controller = Application::Get()->GetWindow()->GetController(e.GetIndex());

					InputManagerMeathods::CallAction(name, (uint32_t)controller.Player, InputEvent::KeyReleased);
				}
			}
		}
	}

	void OnControllerDoubleClick(ControllerButtonDoubleClickEvent& e) {
		for (const auto& [name, action] : s_Data->ActionMapping)
		{
				// checking if key hold is an available format
			if (!action.Inputs.contains(InputDevice::ControllerButton))
				continue;

			auto& data = action.Inputs.at(InputDevice::ControllerButton);
			for (auto& inputs : data)
			{
				if (inputs.Key == (int)e.GetButton())
				{
					Controller& controller = Application::Get()->GetWindow()->GetController(e.GetIndex());

					InputManagerMeathods::CallAction(name, (uint32_t)controller.Player, InputEvent::KeyDouble);
				}
			}
		}
	}
	void ControllerTriggerAxis(ControllerTriggerAxisEvent& e){
		for (const auto& [name, motion] : s_Data->MotionMapping)
		{
			// checking if key hold is an available format
			if (!motion.Inputs.contains(InputDevice::ControllerAxis))
				continue;

			auto& data = motion.Inputs.at(InputDevice::ControllerAxis);
			for (auto& inputs : data)
			{
				if (inputs.Key == (int)e.GetTriggerAxis())
				{
					Controller& controller = Application::Get()->GetWindow()->GetController(e.GetIndex());

					InputManagerMeathods::CallMotion(name, (uint32_t)controller.Player, inputs.MotionValue * e.GetAxis());
				}
			}
		}
	}
	void ControllerLeftJoystickAxis(ControllerLeftJoystickAxisEvent& e){
		for (const auto& [name, motion] : s_Data->MotionMapping)
		{
			// checking if key hold is an available format
			if (!motion.Inputs.contains(InputDevice::ControllerAxis))
				continue;

			auto& data = motion.Inputs.at(InputDevice::ControllerAxis);
			for (auto& inputs : data)
			{
				Controller& controller = Application::Get()->GetWindow()->GetController(e.GetIndex());

				if (inputs.Key == (int)ControllerAxis::LeftX)
				{

					InputManagerMeathods::CallMotion(name, (uint32_t)controller.Player, inputs.MotionValue * e.GetX());
				}
				else if (inputs.Key == (int)ControllerAxis::LeftY)
				{
					InputManagerMeathods::CallMotion(name, (uint32_t)controller.Player, inputs.MotionValue * e.GetY());
				}
			}
		}
	}
	void ControllerRightJoystickAxis(ControllerRightJoystickAxisEvent& e)
	{
		for (const auto& [name, motion] : s_Data->MotionMapping)
		{
			// checking if key hold is an available format
			if (!motion.Inputs.contains(InputDevice::ControllerAxis))
				continue;

			auto& data = motion.Inputs.at(InputDevice::ControllerAxis);
			for (auto& inputs : data)
			{
				Controller& controller = Application::Get()->GetWindow()->GetController(e.GetIndex());

				if (inputs.Key == (int)ControllerAxis::RightX)
				{
					InputManagerMeathods::CallMotion(name, (uint32_t)controller.Player, inputs.MotionValue * e.GetX());
				}
				else if (inputs.Key == (int)ControllerAxis::RightX)
				{
					InputManagerMeathods::CallMotion(name, (uint32_t)controller.Player, inputs.MotionValue * e.GetY());
				}
			}
		}
	}
	void InputManager::OnEvent(Event& e) {
		if (s_Data->Runtime == false)
			return;
		EventDispatcher dispatcher(e);
		// KEYBOARD
		{
			dispatcher.Dispatch< KeyClickedEvent>(OnKeyClicked);
			dispatcher.Dispatch<KeyHoldEvent>(OnKeyHold);
			dispatcher.Dispatch<KeyDoubleClickEvent>(OnKeyDoubleClicked);
			dispatcher.Dispatch<KeyPressedEvent>(OnKeyPressed);
		}
		//MOUSE
		{
			dispatcher.Dispatch<MouseMoveEvent>(OnMouseMoved);
			dispatcher.Dispatch<MouseButtonClickedEvent>(OnMouseClicked);
			dispatcher.Dispatch<MouseButtonReleasedEvent>(OnMouseReleased);
		}
		// CONTROLLER
		{
			dispatcher.Dispatch<ControllerButtonClickedEvent>(OnControllerClicked);
			dispatcher.Dispatch<ControllerButtonReleasedEvent>(OnControllerReleased);
			dispatcher.Dispatch<ControllerButtonDoubleClickEvent>(OnControllerDoubleClick);

			// AXIS
			{
				dispatcher.Dispatch<ControllerLeftJoystickAxisEvent>(ControllerLeftJoystickAxis);
				dispatcher.Dispatch<ControllerRightJoystickAxisEvent>(ControllerRightJoystickAxis);

				dispatcher.Dispatch<ControllerTriggerAxisEvent>(ControllerTriggerAxis);
			}
		}
	}
	const std::unordered_map<std::string, Action>& InputManager::GetActionMappings() {
		return s_Data->ActionMapping;
	}
	const std::unordered_map<std::string, Motion>& InputManager::GetMotionMappings() {
		return s_Data->MotionMapping;
	}
	void InputManager::Init()
	{
		s_Data = new InputManagerData();
	}

	void InputManager::Destroy()
	{
		delete s_Data;
	}

}
