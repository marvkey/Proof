#pragma once
#include "Proof/Asset/Asset.h"
#include "InputActionOutput.h"
#include "InputTypes.h"

#include <vector>
namespace Proof
{
	class InputInteraction;
	class InputCustomizer;

	enum class InputActionOutputValueBehavior 
	{
		/**
		 * Takes the value from the Binding with the highest Absolute Value.
		 *
		 * For example, in a game where the player controls character movement using the arrow keys:
		 * - Pressing the "Up" arrow key produces a positive value (e.g., +0.8) for forward movement.
		 * - Simultaneously pressing the "Down" arrow key produces a negative value (e.g., -0.6) for backward movement.
		 * - Despite conflicting inputs, the resulting action's value reflects the highest absolute movement input (e.g., 0.8).
		 * - 
		 * This behavior is particularly useful for player movement in games, where simultaneous input from multiple keys may lead to conflicting movement directions.
		 * By considering only the highest absolute input value, the player's movement accurately reflects the most significant directional change,
		 * ensuring smooth and intuitive character control even amidst conflicting key presses.
		 */
		MaximumAbsolute,

		/**
		* Aggregates the values from different Bindings.
		*
		* For example, if pressing the up arrow key contributes a value of +0.5 and pressing the down arrow key contributes -0.5 to the same input action,
		* the resulting value would be 0.0, effectively canceling each other out.
		*
		* This behavior might be useful in scenarios where you want conflicting inputs, such as pressing up and down arrow keys simultaneously, to result in no net movement or action.
		*/
		Aggregate,
	};
	class InputAction : public Asset
	{
	public:
		ASSET_CLASS_TYPE(InputAction);
		bool TriggerWhenPaused = false;
		InputActionOutputType OutputType = InputActionOutputType::Bool;
		InputActionOutputValueBehavior OutputValueBehavior = InputActionOutputValueBehavior::MaximumAbsolute;

		std::vector<Count< InputInteraction>> Interactions;
		std::vector<Count< InputCustomizer>> Customizers;
	private:
		
	};
}