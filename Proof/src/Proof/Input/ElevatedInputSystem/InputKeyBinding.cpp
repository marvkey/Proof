#include "Proofprch.h"
#include "InputKeyBinding.h"
#include "Proof/Input/ElevatedInputSystem/InputActionOutput.h"
#include "Proof/Input/ElevatedInputSystem/ElevatedPlayer.h"
#include "Proof/Input/ElevatedInputSystem/InputAction.h"
namespace Proof
{
    bool InputKeyBindingBase::IsKeyMappedAsModifier(const ElevatedInputKey& inputKey)const
    {
        for (auto& key : m_ModifierKeys)
        {
            if (key->GetKey() == inputKey)
                return true;
        }
        return false;
    }
    bool InputKeyBindingBase::ProcessInputData(Count<class ElevatedPlayer> player, InputActionOutput& actionValue, Count<InputAction> action, const ElevatedInputKey& key, bool isModifierKey)
    {
        for (auto& modiferKeys : m_ModifierKeys)
        {
            // we return because if the sub modifers keys are not active
            // that means this action should not be proccessing an Input
            if (player->ShouldProccessInput(modiferKeys->GetKey()))
            {
                if (!modiferKeys.As<InputKeyBindingBase>()->ProcessInputData(player, actionValue, action, key, true))
                    return false;
            }
            else
            {
                return false;
            }
        }

        return true;
    }
    bool InputKeyBinding::ProcessInputData(Count<class ElevatedPlayer> player, InputActionOutput& actionValue, Count<InputAction> action, const ElevatedInputKey& key, bool isModifierKey)
    {
        if (!InputKeyBindingBase::ProcessInputData(player, actionValue, action, key, isModifierKey))
            return false;


        if (!player->ShouldProccessInput(m_InputKey))
            return false;
        float deltaTime = FrameTime::GetWorldDeltaTime();

        auto& actionData = player->GetActionData(action);

        auto rawValue = actionValue.Get<glm::vec3>();
        InputActionOutputType ValueType = actionData.ActionOutput.GetOutputType();
        InputActionOutput modifiedValue = player->ApplyCustomizer(m_Customizers, InputActionOutput(ValueType, rawValue), deltaTime);

        InputStateTracker triggerStateTracker;
        InteractionState calcedState = triggerStateTracker.EvaluateInteractions(player, m_Triggers, modifiedValue, deltaTime);


        triggerStateTracker.SetStateForNoTriggers(modifiedValue.IsNonZero() ? InteractionState::Triggered : InteractionState::None);

        // modifeeir key must be triggerd to be able to procces input data

        if (isModifierKey)
        {
            return triggerStateTracker.GetState() == InteractionState::Triggered;
        }

        bool interactionsApplied = (m_Triggers.size()) > 0;
        const InputActionOutputValueBehavior accumulationBehavior = action->OutputValueBehavior;
        if (modifiedValue.GetMagnitudeSq())
        {
            const int NumComponents = glm::max(1, int(ValueType));
            glm::vec3 modified = modifiedValue.Get<glm::vec3>();
            glm::vec3 merged = actionData.ActionOutput.Get<glm::vec3>();
            for (int component = 0; component < NumComponents; ++component)
            {
                if (accumulationBehavior == InputActionOutputValueBehavior::Aggregate)
                {
                    merged[component] += modified[component];
                }
                else // Maximum absolute
                {
                    // going to use > just to ensure teh existing value is kept
                    // because it could be a negative 
                    
                    //if (glm::abs(modified[component]) >= glm::abs(merged[component]))
                    if (glm::abs(modified[component]) > glm::abs(merged[component]))
                    {
                        merged[component] = modified[component];
                    }
                }
            }
            actionData.ActionOutput = InputActionOutput(ValueType, merged);

        }

        actionData.InteractionStateTracker = actionData.InteractionStateTracker > triggerStateTracker ? actionData.InteractionStateTracker : triggerStateTracker;
        actionData.InteractionStateTracker.SetMappingInteractionApplied(interactionsApplied);

    }
    InputKeyBindingBundle::InputKeyBindingBundle()
    {
        EnumReflection::ForEach< InputKeyBidningBundleTypes>([&](InputKeyBidningBundleTypes val)
            {
                m_Bindings[val] = Count<InputKeyBinding>::Create();
            });
    }
    InputKeyBindingBundle::InputKeyBindingBundle(const std::unordered_map< InputKeyBidningBundleTypes, ElevatedInputKey>& inputBindings)
    {
        EnumReflection::ForEach< InputKeyBidningBundleTypes>([&](InputKeyBidningBundleTypes val)
            {
                m_Bindings[val] = Count<InputKeyBinding>::Create();
            });

        for (auto& [binding, keyBinding] : inputBindings)
        {
            if (keyBinding.IsAnalog())
            {
                PF_ENGINE_ERROR("InputKeyBindingBundle only supports Digital Input {} is not Digital", keyBinding.GetKeyName());
                continue;
            }

            m_Bindings[binding] = Count<InputKeyBinding>::Create(keyBinding);
        }
    }
    bool InputKeyBindingBundle::ProcessInputData(Count<class ElevatedPlayer> player, InputActionOutput& actionValue, Count<InputAction> action, const ElevatedInputKey& key,bool isModifierKey)
    {
        if (!InputKeyBindingBase::ProcessInputData(player, actionValue, action, key, isModifierKey))
            return false;

        for (auto& [binding, keyBinding] : m_Bindings)
        {
            if (!keyBinding)
                continue;

            if (keyBinding->GetKey() == key)
                continue;

            if (!keyBinding.As<InputKeyBindingBase>()->ProcessInputData(player, actionValue, action,key, false))
                return false;
        }

        return true;
    }
    void InputKeyBindingBundle::CheckOrUpdateAction(const ElevatedInputKey& key, glm::vec3& axis, Count<InputAction> action)
    {
        for (auto& [binding, keyBinding] : m_Bindings)
        {

            if (!keyBinding)
                continue;

            if (keyBinding->GetKey() != key)
                continue;

            if (axis.x <= 0.0f)
                continue;

            switch (binding)
            {
                case Proof::InputKeyBidningBundleTypes::PositiveX:
                {
                    axis.x = axis.x;
                }
                break;
                case Proof::InputKeyBidningBundleTypes::NegativeX:
                {
                    axis.x = -axis.x;
                    //axis.x = 0;
                }
                break;
                case Proof::InputKeyBidningBundleTypes::PositiveY:
                {
                    if(action->OutputType > InputActionOutputType::Float)
                        axis.y = axis.x;
                    //axis.x = 0;
                }
                break;
                case Proof::InputKeyBidningBundleTypes::NegativeY:
                {
                    if (action->OutputType > InputActionOutputType::Float)
                        axis.y = -axis.x;
                    //axis.x = 0;
                }
                break;
                case Proof::InputKeyBidningBundleTypes::PositiveZ:
                {
                    if (action->OutputType > InputActionOutputType::Vector2D)
                        axis.z = axis.x;
                    //axis.x = 0;
                }
                break;
                case Proof::InputKeyBidningBundleTypes::NegativeZ:
                {
                    if (action->OutputType > InputActionOutputType::Vector2D)
                        axis.z = -axis.x;
                    //axis.x = 0;
                }
            break;
            default:
                break;
            }
        }
    }
}
