#pragma once
#include "Proof/Core/Core.h"

namespace Proof{

	class Proof_API Panel{
	public:
		virtual void ImGuiRender(class FrameTime deltaTime)=0;
		virtual void SetWindowVisibile(bool value)=0;
	};
};