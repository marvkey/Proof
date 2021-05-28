#pragma once
#include <iostream>
#include "FrameTime.h"
#include "ImGUIOpenGL/imgui.h"
#include "Proof/Core/Log.h"
namespace Proof {
   class Proof_API Layer {
   public:
	  Layer(const std::string& Name = "Debug Layer"):
		 DebugName(Name)
	  {

	  }
	  virtual ~Layer() {};
	  virtual void OnUpdate(FrameTime DeltaTime) {}
	  virtual void OnAttach() {}
	  virtual void OnDetach() {}
	  virtual void OnImGuiDraw() {}
	  virtual const std::string& GetName() const { return DebugName; }
   private:
	  std::string DebugName;
   };
}