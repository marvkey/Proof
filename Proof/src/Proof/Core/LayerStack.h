#pragma once
#include <vector>
#include "Layer.h"
namespace Proof {
	class Proof_API LayerStack{
	public:
		LayerStack() = default;
		~LayerStack();
		void PushLayer(Layer* layer);
		void PopLayer(Layer* layer);
		void PushOverlay(Layer* layer);
		void PopOverlay(Layer* layer);

		std::vector<Layer*>::iterator begin() { return V_LayerStack.begin(); };
		std::vector<Layer*>::iterator end() { return V_LayerStack.end(); };
		std::vector<Layer*>::reverse_iterator rbegin() { return V_LayerStack.rbegin(); }
		std::vector<Layer*>::reverse_iterator rend() { return V_LayerStack.rend(); }
	private:
		std::vector<Layer*>V_LayerStack;
		unsigned int LayerStackIndex = 0;
		friend Application;
	};
}