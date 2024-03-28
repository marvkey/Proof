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
    Count<InputKeyBinding> InputKeyBindingBase::AddModifierKey(const ElevatedInputKey& key)
    {
        m_ModifierKeys.push_back(Count<InputKeyBinding>::Create(key, true));
        return m_ModifierKeys.back();
    }
    void InputKeyBindingBase::AddModifierKey(Count<InputKeyBinding> modifer)
    {
        if (!modifer->IsModifier())
        {
            PF_ENGINE_ERROR("Cannot add key that is not a Mofifier");
            return;
        }
        m_ModifierKeys.push_back(modifer);

    }
    void InputKeyBindingBase::RemoveModifierKey(uint32_t index)
    {
        if (m_ModifierKeys.size() > index)
            m_ModifierKeys.erase(m_ModifierKeys.begin() + index);
        else
            PF_ENGINE_WARN("Cannot Remove Modifier key it is out of Index");
    }
    Count<InputKeyBinding> InputKeyBindingBase::GetModifierKey(uint32_t index)
    {
        if (m_ModifierKeys.size() > index)
            return m_ModifierKeys.at(index);
        return nullptr;
    }
    bool InputKeyBindingBase::ProcessInputData(Count<class ElevatedPlayer> player, InputActionOutput& actionValue, Count<InputAction> action, const ElevatedInputKey& key, float deltaTime)
    {
        for (auto& modiferKeys : m_ModifierKeys)
        {
            // we return because if the sub modifers keys are not active
            // that means this action should not be proccessing an Input
            if (player->ShouldProccessInput(modiferKeys->GetKey()))
            {
                if (!modiferKeys.As<InputKeyBindingBase>()->ProcessInputData(player, actionValue, action, key, deltaTime))
                    return false;
            }
            else
            {
                return false;
            }
        }

        return true;
    }
    bool InputKeyBinding::ProcessInputData(Count<class ElevatedPlayer> player, InputActionOutput& actionValue, Count<InputAction> action, const ElevatedInputKey& key, float deltaTime)
    {
        if (!InputKeyBindingBase::ProcessInputData(player, actionValue, action, key, m_IsModifierKey))
            return false;


        if (!player->ShouldProccessInput(InputKey))
            return false;

        auto& actionData = player->GetActionData(action);

        auto rawValue = actionValue.Get<glm::vec3>();
        InputActionOutputType ValueType = actionData.ActionOutput.GetOutputType();
        InputActionOutput modifiedValue = player->ApplyCustomizer(Customizers, InputActionOutput(ValueType, rawValue), deltaTime);

        InputStateTracker triggerStateTracker;
        InteractionState calcedState = triggerStateTracker.EvaluateInteractions(player, Interactions, modifiedValue, deltaTime);


        triggerStateTracker.SetStateForNoTriggers(modifiedValue.IsNonZero() ? InteractionState::Triggered : InteractionState::None);

        // modifeeir key must be triggerd to be able to procces input data

        if (m_IsModifierKey)
        {
            return triggerStateTracker.GetState() == InteractionState::Triggered;
        }

        bool interactionsApplied = (Interactions.size()) > 0;
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
        actionData.InteractionStateTracker.SetBindingInteractionApplied(interactionsApplied);

    }
    InputKeyBindingBundle::InputKeyBindingBundle()
    {
        EnumReflection::ForEach< InputKeyBindingBundleTypes>([&](InputKeyBindingBundleTypes val)
            {
                m_Bindings[val] = Count<InputKeyBinding>::Create();
            });
    }
    InputKeyBindingBundle::InputKeyBindingBundle(const std::unordered_map< InputKeyBindingBundleTypes, ElevatedInputKey>& inputBindings)
    {
        EnumReflection::ForEach< InputKeyBindingBundleTypes>([&](InputKeyBindingBundleTypes val)
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
    Count<InputKeyBinding> InputKeyBindingBundle::SetKey(InputKeyBindingBundleTypes type, const ElevatedInputKey& key)
    {
        if(key.IsDigital())
            m_Bindings[type]->InputKey = key;

        return m_Bindings[type];
    }
    void InputKeyBindingBundle::SetKey(InputKeyBindingBundleTypes type, Count<InputKeyBinding> binding)
    {
        if (!binding->GetKey().IsDigital())
            return;

        if(binding && !binding->IsModifier())
            m_Bindings[type] = binding;
    }
    void InputKeyBindingBundle::UnSetKey(InputKeyBindingBundleTypes type)
    {
        m_Bindings[type]->InputKey = ElevatedInputKeys::Invalid;
    }

    Count<InputKeyBinding> InputKeyBindingBundle::GetKey(InputKeyBindingBundleTypes type)
    {
        return m_Bindings[type];
    }

    bool InputKeyBindingBundle::ProcessInputData(Count<class ElevatedPlayer> player, InputActionOutput& actionValue, Count<InputAction> action, const ElevatedInputKey& key,float deltaTime)
    {
        if (!InputKeyBindingBase::ProcessInputData(player, actionValue, action, key, deltaTime))
            return false;

        for (auto& [binding, keyBinding] : m_Bindings)
        {
            if (!keyBinding)
                continue;

            if (keyBinding->GetKey() == key)
                continue;

            if (!keyBinding.As<InputKeyBindingBase>()->ProcessInputData(player, actionValue, action,key, deltaTime))
                return false;
        }

        return true;
    }
    void InputKeyBindingBundle::CheckOrUpdateAction(const ElevatedInputKey& key, glm::vec3& axis, Count<InputAction> action, float deltaTime)
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
                case Proof::InputKeyBindingBundleTypes::PositiveX:
                {
                    axis.x = axis.x;
                }
                break;
                case Proof::InputKeyBindingBundleTypes::NegativeX:
                {
                    axis.x = -axis.x;
                    //axis.x = 0;
                }
                break;
                case Proof::InputKeyBindingBundleTypes::PositiveY:
                {
                    if(action->OutputType > InputActionOutputType::Float)
                        axis.y = axis.x;
                    //axis.x = 0;
                }
                break;
                case Proof::InputKeyBindingBundleTypes::NegativeY:
                {
                    if (action->OutputType > InputActionOutputType::Float)
                        axis.y = -axis.x;
                    //axis.x = 0;
                }
                break;
                case Proof::InputKeyBindingBundleTypes::PositiveZ:
                {
                    if (action->OutputType > InputActionOutputType::Vector2D)
                        axis.z = axis.x;
                    //axis.x = 0;
                }
                break;
                case Proof::InputKeyBindingBundleTypes::NegativeZ:
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
