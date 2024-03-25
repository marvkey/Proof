#pragma once
#include "Proof/Core/Core.h"
#include "InputActionOutput.h"
#include "InputTypes.h"
namespace Proof
{
    class ElevatedPlayer;
    /*
  *
  *
  Direct Trigger:
      A direct trigger is a condition or event that directly determines whether an action should be triggered.
      In the context of input actions, a direct trigger could be a specific key press, button press, or input event that directly corresponds to the action.
      When an action has direct triggers, it can be triggered if any of these triggers are activated.
      In other words, if any of the specified conditions or events occur, the action can be triggered.
      For example, if an action is associated with the "Jump" key, pressing the "Jump" key would be a direct trigger for that action.

  Indirect Trigger:
      An indirect trigger is a condition or event that indirectly contributes to the triggering of an action.
      In contrast to direct triggers, indirect triggers might not directly correspond to the action itself
      but instead represent additional conditions or requirements that must be met for the action to be triggered.
      When an action has indirect triggers, it can be triggered only if all of these triggers are activated.
      In other words, all specified conditions or events must occur simultaneously for the action to be triggered.
      For example, if an action requires both the "Forward" and "Jump" keys to be pressed simultaneously,
      the activation of both keys would be indirect triggers for that action.

      In summary, direct triggers directly determine whether an action should be triggered,
      while indirect triggers represent additional conditions or requirements that must be met for the action to be triggered.
      Both types of triggers play important roles in defining the behavior and conditions under which actions are activated in a game or application.

  Blocking Trigger:
      A blocking trigger is a special type of trigger that, when activated, prevents other triggers from activating, regardless of their state.
      In other words, if a blocking trigger is triggered, it takes precedence over other triggers and prevents them from causing the associated action to be triggered.
      Blocking triggers are often used to implement priority-based or exclusive behaviors where certain conditions must be met before other actions can occur.
      For example, imagine a game where the player can only attack if they are not blocking.
      In this case, the "blocking" action would be a blocking trigger, preventing the "attack" action from being triggered while the player is blocking.
  */
    enum class InteractionMode
    {
        Direct,
        Indirect,
        Blocker,
    };


#define INPUT_INTERACTION_MODE(type) static InteractionMode GetStaticInteractionMode() { return InteractionMode::type; }\
								virtual InteractionMode GetInteractionMode() const { return GetStaticInteractionMode(); }
	class InputInteraction : public RefCounted
	{
    public:

        INPUT_INTERACTION_MODE(Direct);
        /*
        
        The InteractionThreshold within the InputInteraction class serves as a pivotal parameter dictating the sensitivity of interaction detection. 
        Represented by a floating-point value, this threshold acts as a cutoff point against which the magnitude of an 
        input action value is measured. By comparing the squared magnitude of an input action value with the squared 
        InteractionThreshold, the class swiftly determines whether an interaction has occurred. 
        If the squared magnitude surpasses or equals the squared threshold value, the interaction is promptly recognized. 
        This mechanism not only streamlines computation by obviating the need for costly square root calculations 
        but also affords developers granular control over the responsiveness of interaction detection. 
        Fine-tuning the InteractionThreshold enables developers to tailor interaction sensitivity to the unique demands of 
        their application or game, ensuring a seamless and intuitive user experience.
    */
        float InteractionThreshold = 0.5f;

        const InputActionOutput& GetLastActionValue()const { return m_LastValue; }
        
        bool IsInteractionDetected(const InputActionOutput& value) const { return  value.GetMagnitudeSq() >= InteractionThreshold * InteractionThreshold; }
    protected:
        virtual InteractionState UpdateInteractionState(Count<ElevatedPlayer> player, InputActionOutput modifiedValue, float deltaTime) { return IsInteractionDetected(modifiedValue) ? InteractionState::Triggered : InteractionState::None;; };
    private:
        InputActionOutput m_LastValue;	
        friend class InputStateTracker;
	};
    /**
    * trigers when for every frame key is down and not released
    */
    class InputInteractionDown : public InputInteraction
    {
    public:
        INPUT_INTERACTION_MODE(Direct);

    protected:
        virtual InteractionState UpdateInteractionState(Count<ElevatedPlayer> player, InputActionOutput modifiedValue, float deltaTime) override;
    };


    class InputInteractionClickRelease : public InputInteraction
    {
    public:
        enum class ClickReleaseMode
        {
            Click, // return trigger event when the key is Click
            Release,// return trigger event when the key is Released
            ClickRelease // return trigger event when the key is click or released
        };
        ClickReleaseMode Mode;
        INPUT_INTERACTION_MODE(Direct);

    protected:
        virtual InteractionState UpdateInteractionState(Count<ElevatedPlayer> player, InputActionOutput modifiedValue, float deltaTime) override;
    };

    class InputInteractionTimeBound : public InputInteraction
    {
    public:
        float GetHeldDuration()const { return m_HeldDuration; }

    protected:
        float CalculateHeldDuration(const float deltaTime) const;
        virtual InteractionState UpdateInteractionState(Count<ElevatedPlayer> player, InputActionOutput modifiedValue, float deltaTime) override;
    private:
        float m_HeldDuration = 0.0f;
    };

    class InputInteractionHold : public InputInteractionTimeBound
    {
    public:
        INPUT_INTERACTION_MODE(Direct);

        // How Long to hold before Trigger Event fired
        float HoldTimeThreshold = 1.0f;

        // Determine whether this trigger should activate only once or every frame after the hold time threshold is met.
        bool FireOnce = false;
    protected:
        virtual InteractionState UpdateInteractionState(Count<ElevatedPlayer> player, InputActionOutput modifiedValue, float deltaTime) override;
    private:
        bool m_IsTriggered = false;
    };


    class InputInteractionMultiClick : public InputInteraction
    {
    public:
        INPUT_INTERACTION_MODE(Direct);

        uint32_t TapCount = 2;

        // Maximum time allowed to be passed between clicks
        // if time exceded this sate will be set to None
        float TapSpacing = 0.75f;

        // Maximum Time for a key to be releaed to be registerd as a Tap
        //
        float TapTime = 0.2f;

    protected:
        virtual InteractionState UpdateInteractionState(Count<ElevatedPlayer> player, InputActionOutput modifiedValue, float deltaTime) override;

    private:
        void Reset()
        {
            m_CurrentTapCount = 0;
            m_LastReleaseTime = 0;
            m_LastStartTime = 0;
            m_Phase = Phase::None;
        }
        uint32_t m_CurrentTapCount = 0;
        float m_LastReleaseTime = 0.0f;
        float m_LastStartTime = 0.0f;
        enum class Phase
        {
            None =0,
            ReleasePending,
            ClickPending
        };
        Phase m_Phase = Phase::None;
    };
}