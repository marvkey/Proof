#pragma once
#include "Proof/Scene/Script.h"
#include "Proof/Input/Input.h"
namespace Proof{
	//REGISTER_SCRIPT(MovementScript)
	class Proof_API MovementScript:public Script{
	public:
		float Speed =0.2;
		virtual void OnUpdate(FrameTime DeltaTime)override{
			if(Input::IsKeyPressed(KeyBoardKey::LeftArrow)){
				GetComponent<TransformComponent>()->Location.X+=Speed;
			}

			if(Input::IsKeyPressed(KeyBoardKey::RightArrow)){
				GetComponent<TransformComponent>()->Location.X-=Speed;
			}

			if(Input::IsKeyPressed(KeyBoardKey::UpArrow)){
				GetComponent<TransformComponent>()->Location.Z+=Speed;
			}


			if (Input::IsKeyPressed(KeyBoardKey::DownArrow)) {
				GetComponent<TransformComponent>()->Location.Z -= Speed;
			}
		}
	};
}	