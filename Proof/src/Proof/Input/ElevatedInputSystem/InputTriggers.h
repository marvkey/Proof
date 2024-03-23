#pragma once
#include "Proof/Core/Core.h"
#include "InputActionValue.h"
#include "InputTypes.h"
namespace Proof
{
    class ElevatedPlayer;
    /*
    *
    * 
    Explicit Trigger:
        An explicit trigger is a condition or event that directly determines whether an action should be triggered.
        In the context of input actions, an explicit trigger could be a specific key press, button press, or input event that directly corresponds to the action.
        When an action has explicit triggers, it can be triggered if any of these triggers are activated. 
        In other words, if any of the specified conditions or events occur, the action can be triggered.
        For example, if an action is associated with the "Jump" key, pressing the "Jump" key would be an explicit trigger for that action.

    Implicit Trigger:
        An implicit trigger is a condition or event that indirectly contributes to the triggering of an action.
        In contrast to explicit triggers, implicit triggers might not directly correspond to the action itself 
        but instead represent additional conditions or requirements that must be met for the action to be triggered.
        When an action has implicit triggers, it can be triggered only if all of these triggers are activated. 
        In other words, all specified conditions or events must occur simultaneously for the action to be triggered.
        For example, if an action requires both the "Forward" and "Jump" keys to be pressed simultaneously, 
        the activation of both keys would be implicit triggers for that action.

        In summary, explicit triggers directly determine whether an action should be triggered, 
        while implicit triggers represent additional conditions or requirements that must be met for the action to be triggered.
        Both types of triggers play important roles in defining the behavior and conditions under which actions are activated in a game or application.

    Blocking Trigger:
        A blocking trigger is a special type of trigger that, when activated, prevents other triggers from activating, regardless of their state.
        In other words, if a blocking trigger is triggered, it takes precedence over other triggers and prevents them from causing the associated action to be triggered.
        Blocking triggers are often used to implement priority-based or exclusive behaviors where certain conditions must be met before other actions can occur.
        For example, imagine a game where the player can only attack if they are not blocking. 
        In this case, the "blocking" action would be a blocking trigger, preventing the "attack" action from being triggered while the player is blocking.
    */
    enum class TriggerMode
    {
        // Input may trigger if any explicit trigger is triggered.
        Explicit,

        // Input may trigger only if all implicit triggers are triggered. mainly for combos and chords
        Implicit,

        // Inverted trigger that will block all other triggers if it is triggered.
        Blocker,
    };


#define INPUT_TRIGGER_MODE(type) static TriggerMode GetStaticTriggerMode() { return TriggerMode::type; }\
								virtual TriggerMode GetTriggerMode() const { return GetStaticTriggerMode(); }
	class InputTrigger : public RefCounted
	{
    public:

        INPUT_TRIGGER_MODE(Explicit);
        float TriggerThreshold = 0.5f;

        const InputActionValue& GetLastActionValue()const { return m_LastValue; }
        
        bool IsTriggerDetected(const InputActionValue& value) const { return  value.GetMagnitudeSq() >= TriggerThreshold * TriggerThreshold; }
    protected:
        virtual TriggerState UpdateTriggerState(Count<ElevatedPlayer> player, InputActionValue modifiedValue, float deltaTime) { return IsTriggerDetected(modifiedValue) ? TriggerState::Triggered : TriggerState::None;; };
    private:
        InputActionValue m_LastValue;	
        friend class InputStateTracker;
	};

    class InputTriggerTimeBased : public InputTrigger
    {
    public:
        float GetHeldDuration()const { return m_HeldDuration; }

    protected:
        float CalculateHeldDuration(const float deltaTime) const;
        virtual TriggerState UpdateTriggerState(Count<ElevatedPlayer> player,InputActionValue modifiedValue, float deltaTime) override;
    private:
        float m_HeldDuration = 0.0f;
    };

    class InputTriggerClicked : public InputTrigger
    {
    public:
        INPUT_TRIGGER_MODE(Explicit);

    protected:
        virtual TriggerState UpdateTriggerState(Count<ElevatedPlayer> player,InputActionValue modifiedValue, float deltaTime) override;
    };

    class InputTriggerReleased  : public InputTrigger
    {
    public:
        INPUT_TRIGGER_MODE(Explicit);
    protected:
        virtual TriggerState UpdateTriggerState(Count<ElevatedPlayer> player,InputActionValue modifiedValue, float deltaTime) override;
    };
}