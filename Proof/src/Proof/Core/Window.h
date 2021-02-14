#pragma once
#include "Core.h"
namespace Proof{
		class Proof_API Window {
				
		public:
				//virtual void* GetWindow() = 0;
				virtual unsigned int GetWidth() = 0;
				virtual unsigned int GetHeight() = 0;
		protected:
				virtual void OnNewFrame() = 0;
		private:
				virtual int createWindow() = 0;
		};
}
