#pragma once
#include "Proof/Core/Layer.h"
#include "SandBox3D.h"
class SandBox3D : public Proof::Layer{
public:
	SandBox3D();
	~SandBox3D() =default;
	virtual void OnUpdate(Proof::FrameTime DeltaTime);
	virtual void OnImGuiDraw() override;
private:
	friend class Sandbox;
};

