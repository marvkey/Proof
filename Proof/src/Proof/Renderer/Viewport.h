#pragma once
#include "Proof/Math/Vector.h" 

namespace Proof
{
	struct Viewport {
		// upper left and right corner
		float X;
		float Y;

		float Width = 0.0f;
		float Height = 0.0f;

		float MinDepth = 0.0f;
		float MaxDepth = 1.0f;
	};

	struct ViewportScissor {
		Vector2 Offset = { 0,0 };
		Vector2 Extent = { 0,0 };
	};
}