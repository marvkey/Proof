#pragma once
#include "Application.h"

#ifdef PF_PLATFORM_WINDOW64
bool ApplicationRun = true;
extern Proof::Application* Proof::CreateApplication(int argc, char** argv);
namespace Proof
{

	int Main(int argc, char** argv) {
		while (ApplicationRun == true) {
			Proof::Application* Proof = Proof::CreateApplication(argc, argv);
			Proof->Run();
			ApplicationRun = !Proof->GetApplicationShouldShutdown();
			delete Proof;
		}
		return 0;
	}
}

#ifdef PF_DIST
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,PSTR lpCmdLine, int nCmdShow)
{
	return Proof::Main(__argc, __argv);
}
#else

int main(int argc, char** argv) {

	return Proof::Main(argc, argv);
};
#endif
#endif


