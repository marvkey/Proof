#pragma once
#include "Application.h"

#if defined(_WIN64)
	int  main(int argc,char** argv) {
		Proof::Application* Proof = Proof::CreateApplication();
		Proof->Run();
		delete Proof;
	}
#endif