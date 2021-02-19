#include "Proofprch.h"
#include "Application.h"


namespace Proof {
			WindowsWindow* Application::MainWindow = nullptr;
			Application::Application() {
					
	}
	Application::~Application() {

		

	}

	void Application::Run() {
		
			
			MainWindow = new WindowsWindow(200, 300);
				PF_ENGINE_ERROR("WE ARE FINALLY BACK AFTER {} DAYS", 56);
				

				MainWindow->createWindow();
				bool newWindow = false;
				float Num1 = 5;
				if (glfwGetKey(MainWindow->GetWindow(), KeyBoard::B) == KeyBoard::KeyPressed) {
							PF_ENGINE_ERROR("noasdfas");
				}
				while ((glfwGetKey( MainWindow->GetWindow(), KeyBoard::Escape) != KeyBoard::KeyPressed) && (glfwWindowShouldClose( MainWindow->GetWindow()) == false)) {

							 MainWindow->WindowBegin();
								
								
							 MainWindow->windowPollEvents(false);
				};

		 MainWindow->WindowEnd();
		
		
	}
	
}

