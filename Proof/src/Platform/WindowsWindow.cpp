#include "Proofprch.h"
#include "WindowsWindow.h"


namespace Proof {
			
	GLFWwindow* WindowsWindow::GetWindow() {
				return MainWindow;
		}

		unsigned int WindowsWindow::GetWidth() {
				return 0;
		}
		unsigned int WindowsWindow::GetHeight() {

				return 0;
		}
		void WindowsWindow::windowPollEvents(bool UsingGui){
			if (UsingGui == true) {

				ImGui::Render();
				ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
			}
			glfwSwapBuffers(MainWindow);
			glfwPollEvents();
		}
		std::pair<float, float> WindowsWindow::GetPlatformMouseLocation(){
					double X, Y;
					glfwGetCursorPos(MainWindow, &X, &Y);

					return { (float)X, (float)Y };
		}
		void WindowsWindow::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){

					if(action ==GLFW_PRESS){
								PF_ENGINE_INFO("MAYBE");
					}
					
		}
		void WindowsWindow::OnNewFrame() {


		}
		
		int WindowsWindow::createWindow() {
			
		
			
				if (!glfwInit()) {
						PF_ENGINE_ERROR("GlFW Not initilised");
						return -1;
				}

				
				MainWindow = glfwCreateWindow(Width, Height, "Proof Engine", NULL, NULL);

				if (MainWindow == nullptr) {
						PF_ENGINE_ERROR("Main Window nOt work");
						glfwTerminate();
						return -1;
				}

				
				glfwMakeContextCurrent(MainWindow);
				glfwSetKeyCallback(MainWindow, key_callback);
				if (glewInit() != GLEW_OK) {
						PF_ENGINE_ERROR("Glew Init Not defined");
						return -1;
				}
			

				float Position[6] = {
					-0.1f, -0.1f,
				0.0f,  0.1f,
				0.5f, -0.5f
				};

				unsigned int buffer;
				glGenBuffers(1, &buffer);
				glBindBuffer(GL_ARRAY_BUFFER, buffer);
				glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), Position, GL_STATIC_DRAW);


				glEnableVertexAttribArray(0);
				glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);
				glBindBuffer(GL_ARRAY_BUFFER, 0);

				ImGui::CreateContext();
				ImGui_ImplGlfw_InitForOpenGL(MainWindow, true);
				ImGui::StyleColorsDark();
				

				
				return 0;
		}

		int WindowsWindow::WindowEnd(){
			ImGui_ImplOpenGL3_Shutdown();
			ImGui::DestroyContext();
			glfwTerminate();
			return 0;

		}

		void WindowsWindow::WindowBegin() {
				 bool newWindow = false;
					glClear(GL_COLOR_BUFFER_BIT);
					glDrawArrays(GL_TRIANGLES, 0, 3);
					ImGui_ImplOpenGL3_NewFrame();
					ImGui_ImplGlfw_NewFrame();
			
		}
		
}
