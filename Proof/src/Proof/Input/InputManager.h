#pragma once
#include "KeyCodes.h"
#include "Proof/Events/Event.h"
#include <unordered_map>
#include <functional>
#include <string>
#include "Proof/Events/ControllerEvent.h"

#include<unordered_set>
namespace Proof {
	struct InputType {
		InputType(InputDevice inputDevice, int key) {
			Device = inputDevice;
			Key = key;
		}
		InputDevice Device = InputDevice::None;
		int Key = 0; /// value is an enum
	};
	struct Action {
		std::unordered_map<InputDevice,std::vector<InputType>> Inputs;
	};
	struct MotionInputType {
	
		MotionInputType(InputDevice inputDevice, int key,float motionValue=1) {
			Device = inputDevice;
			Key = key;
			MotionValue = motionValue;
		}
		InputDevice Device = InputDevice::None;
		int Key = 0; /// value is an enum
		float MotionValue = 1.0;
	};
	struct Motion {
		std::unordered_map<InputDevice, std::vector<MotionInputType>> Inputs;
	};
	struct PlayerActions 
	{
		std::string ActionName;
		//event, Action 
		std::unordered_map<InputEvent, std::function<void()>>Events;
		//InputEvent Event;
		//std::function<void()> Action;
	};

	struct PlayerMotion
	{
		std::string MotionName;
		//default motionValue =1.0f;
		std::function<void(float motionValue )> Action;
		
	};

	enum class PlayerInputState {
		Enabled =0,
		Disabled = 1
	};
	class InputManagerMeathods
	{
	public:
		// if player exist the player data is deleted
		// and a new instnace is created
		static void SetPlayer(uint32_t player);

		// delets players
		static void DeletePlayer(uint32_t player);
		static bool HasPlayer(uint32_t player);

		static void SetPlayerInput(uint32_t player,PlayerInputState state);
		/**
		 *
		 * 
		 * @param name : Action Name
		 * @param player : player index
		 * @param inputEvent : Input Type 
		 * @param func : pointer to function
		 */
		static void BindAction(const std::string& name, uint32_t player, InputEvent inputEvent, const std::function<void()>& func);
		
		/**
		 * @param name : Action Name
		 * @param player : player index
		 * @param func : pointer to function
		 */
		static void BindMotion(const std::string& name, uint32_t player, const std::function<void(float MotionValue)>& func);

		/**
		 * called automatically but here if we want to call it ourselves at any time.
		 * 
		 * \param name
		 * \param player
		 * \param inputEvent
		 */
		static void CallAction(const std::string& name, uint32_t player, InputEvent inputEvent);
		/**
		 * called automatically but here if we want to call it ourselves at any time.
		 *
		 * \param name
		 * \param player
		 * @param motionValue
		 */
		static void CallMotion(const std::string& name, uint32_t player, float motionValue);
	private:

		friend class InputManager;
	};
	// make this class per player ID
	//call it Input(Something)
	//have an atual input manger class that proccesses event 
	//then that class will send liek keyboard event to the id 
	// this class will be stored in a map with the id signifying the current player
	class InputManager {
	public: 
		static void OnEvent(Event& e);

		InputManager() = delete;
		
		static bool AddAction(const std::string& name);
		static void ActionAddKey(const std::string& name, InputType inputype);

		static bool AddMotion(const std::string& name);
		static void MotionAddKey(const std::string& name, MotionInputType inputType);

		static bool HasMotion(const std::string& name);
		static bool HasAction(const std::string& name);
		// MOTION INPUTS
		static void StartRuntime(uint32_t playerCount =1);
		static void EndRuntime();

		static const std::unordered_map<std::string, Action>& GetActionMappings();
		static const std::unordered_map<std::string, Motion>& GetMotionMappings();
	private:

		static void Init();
		static void Destroy();
		friend class Application;
		friend class InputManagerMeathods;
	};		
}