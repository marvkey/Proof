#pragma once
#include "Application.h"

#if defined(_WIN64)
	int  main(int argc,char** argv) {
		OPTICK_APP("Proof");
		Proof::Application* Proof = Proof::CreateApplication();
		Proof->Run();
		delete Proof;
	}
#endif