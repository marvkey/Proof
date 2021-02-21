#pragma once

#if defined(_WIN64)
extern Proof::Application* Proof::CreateApplication();
int  main(int argc, char** argv){
		Proof::Log::Init();
		auto app = Proof::CreateApplication();

		app->Run();
		delete app;
}
#endif