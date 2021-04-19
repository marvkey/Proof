#pragma once
#include "Proof/Core/ProofConnect.h"
#include "SandBox3D.h"

class Sandbox :public Proof::Application {

public:
	~Sandbox() {

	}

	Sandbox() {
		PushLayer(new SandBox3D());
	}
};

Proof::Application* Proof::CreateApplication() {
	return new Sandbox();
}


