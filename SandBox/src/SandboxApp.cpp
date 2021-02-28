#pragma once
#include "Proof/Core/ProofConnect.h"


class Sandbox :public Proof::Application {

public:
	~Sandbox() {

	}

	Sandbox() {
		
	}
};

Proof::Application* Proof::CreateApplication() {
	return new Sandbox();
}

