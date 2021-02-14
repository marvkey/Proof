#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Proof/Core/Window.h"
#include "Proof/Core/GUI.h"


namespace Proof {
	

		class Proof_API WindowsWindow : public Window {
		public:
			friend class
						Application;
			virtual GLFWwindow* GetWindow();
			virtual unsigned int GetWidth();
			virtual unsigned int GetHeight();
			WindowsWindow(unsigned int Width, unsigned int Height) {
				this->Width = Width;
				this->Height = Height;
			}

			float GetMouseX() {

				auto [X, Y] = GetPlatformMouseLocation();
				return X;
			}

			float GetMouseY() {

				auto [X, Y] = GetPlatformMouseLocation();
				return Y;
			}


				
		
		protected:
				virtual void OnNewFrame();
		private:
			void  WindowLoop();

			virtual	int createWindow()override;

			int WindowEnd();

			void windowPollEvents(bool UsingGui);
			unsigned int Width, Height;
			GLFWwindow* MainWindow;
			std::pair<float, float> GetPlatformMouseLocation();
			static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
		};
}


