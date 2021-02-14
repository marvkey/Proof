#pragma once
#include "Core.h"
#include "Platform/WindowsWindow.h"


namespace Proof {

	 	class Proof_API Application {
			public:
						virtual ~Application(); // main app//
						void Run();

						void* GetWindow() {
								
									return MainWindow->GetWindow();
						}

			protected:
						
						Application();
			private:
						 
						static WindowsWindow* MainWindow;

			};
			Application* CreateApplication();
}


