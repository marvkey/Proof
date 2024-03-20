#pragma once
#include "Proof/Core/Core.h"
#include "../InputTypes.h"
#include "Proof/Events/Event.h"
#include "Proof/Core/Delegate.h"

namespace Proof
{
	class ElevatedInputDevice : RefCounted
	{
	public:
		virtual void OnEvent(Event& event) {};

        // Bind a free function to the event delegate
        template<typename TReturn, typename... TArgs>
        void Bind(TReturn(*function)(TArgs...)) {
            m_EventDelegate.Bind<function>();
        }

        // Bind a lambda to the event delegate
        template<typename TLambda>
        void Bind(const TLambda& lambda) {
            m_EventDelegate.Bind(lambda);
        }

        // Bind a member function to the event delegate
        template<typename TClass>
        void Bind(bool (TClass::* function)(const ElevatedInputKeyParams&), TClass* object) {
            m_EventDelegate.Bind<function>(object);
        }

        // Unbind a free function from the event delegate
        template<typename TReturn, typename... TArgs>
        void Unbind(TReturn(*function)(TArgs...)) {
            m_EventDelegate.Unbind<function>();
        }

        // Unbind a lambda from the event delegate
        template<typename TLambda>
        void Unbind(const TLambda& lambda) {
            m_EventDelegate.Unbind(lambda);
        }

        // Unbind a member function from the event delegate
        template<typename TClass>
        void Unbind(bool (TClass::* function)(const ElevatedInputKeyParams&), TClass* object) {
            m_EventDelegate.Unbind<TClass, function>(object);
        }

	protected:
		MulticastDelegate<bool(const ElevatedInputKeyParams&)> m_EventDelegate;

	protected:
		bool InvokeStep(const ElevatedInputKeyParams& params)
		{
			for (uint32_t index = 0; index < m_EventDelegate.GetNumberInvocation(); index++)
			{
				if (m_EventDelegate.InvokeByStep(index, params))
					return true;
			}
            return false;
		}
	};
}