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

        // Function to bind to the multicast delegate
        template <bool(*TFunction)(const ElevatedInputKeyParams&)>
        void BindToEventDelegate()
        {
            m_EventDelegate.Bind<TFunction>();
        }

        // Lambda binding
        template <class TLambda>
        void BindToEventDelegate(const TLambda& lambda) 
        {
            m_EventDelegate.Bind(lambda);
        }

        // Member function binding
        template <auto TFunction, class TClass>
        void BindToEventDelegate(TClass* object) 
        {
            m_EventDelegate.Bind<TFunction>(object);
        }

        // Const member function binding
        template <class TClass, auto TFunction>
        void BindToEventDelegate(const TClass* object)
        {
            m_EventDelegate.Bind<TFunction>(object);
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