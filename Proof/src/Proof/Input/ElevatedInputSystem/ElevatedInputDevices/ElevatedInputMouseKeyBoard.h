#pragma once
#include "Proof/Core/Core.h"
#include "ElevatedInputDevice.h"
#include <glm/glm.hpp>
namespace Proof
{
	//https://github.com/EpicGames/UnrealEngine/blob/072300df18a94f18077ca20a14224b5d99fee872/Engine/Plugins/EnhancedInput/Source/InputEditor/Public/EnhancedInputEditorProcessor.h
	class ElevatedInputDeviceMouseKeyboard : public ElevatedInputDevice
	{
	public:
		virtual void OnEvent(Event& event);
		virtual void OnUpdate(float deltaTime);

	private:
		void UpdateCachePosition(glm::vec2 data);
		void ProcessAccumulatedPointerInput(float deltaTime);
	private:
		/**
	 * The number of cursor samples that happened this frame. The X and Y will be incremented when there is a mouse move event this frame
	 * and it will be reset on ProcessAccumulatedPointerInput.
	 */
		glm::uvec2 m_NumCursorSamplesThisFrame = glm::uvec2(0,0);

		/** The value of the mouse cursor from the most recent mouse event */
		glm::vec2 m_CachedCursorDelta = glm::vec2(0.0f);
	};
}