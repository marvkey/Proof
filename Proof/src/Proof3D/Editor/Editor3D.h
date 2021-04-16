#pragma once
#include "Proof/Core/Layer.h"
namespace Proof {
	class Proof_API Editore3D : public Layer {
	public:
		Editore3D();
		~Editore3D() = default;
		virtual void OnUpdate(FrameTime DeltaTime) override;
		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnImGuiDraw() override;
		
	private:
		bool OpneLogConsole = true;
		void LogConsole();
		void ContentBrowser();
		void WorldObjects();
		void SetDocking(bool* p_open);
	};
}