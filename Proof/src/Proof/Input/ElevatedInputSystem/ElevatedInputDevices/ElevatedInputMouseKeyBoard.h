#pragma once
#include "Proof/Core/Core.h"
#include "ElevatedInputDevice.h"
#include <glm/glm.hpp>
namespace Proof
{
	enum class ElevatedInputDeviceMouseKeyboardMouseAxisDispatch
	{
		None = 0,
		X = BIT(0),
		Y = BIT(1)
	};
	class ElevatedInputDeviceMouseKeyboard : public ElevatedInputDevice
	{
	public:
		ElevatedInputDeviceMouseKeyboard();
		virtual void OnEvent(Event& event);
		virtual void OnUpdate(float deltaTime);

	private:
		void UpdateCachePosition(glm::vec2 data);
		void ProcessAccumulatedPointerInput(float deltaTime);
	private:

		std::unordered_map<ElevatedInputKey, bool> m_ModifierKeyStates; // bool down or not
		/**
	 * The number of cursor samples that happened this frame. The X and Y will be incremented when there is a mouse move event this frame
	 * and it will be reset on ProcessAccumulatedPointerInput.
	 */
		glm::uvec2 m_NumCursorSamplesThisFrame = glm::uvec2(0,0);

		/** The value of the mouse cursor from the most recent mouse event */
		glm::vec2 m_CachedCursorDelta = glm::vec2(0.0f);

		ElevatedInputDeviceMouseKeyboardMouseAxisDispatch m_MouseAxisDispatch;
		bool m_MousePosSetToRelease = true;// using the mouseAxisDispacth is not workign so using the viariable to track;
	};
	DEFINE_ENUM_CLASS_FLAGS(ElevatedInputDeviceMouseKeyboardMouseAxisDispatch);
}