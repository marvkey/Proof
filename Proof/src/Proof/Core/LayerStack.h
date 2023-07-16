#pragma once
#include "SmartPointer.h"
#include <vector>
namespace Proof {
	class Layer;
	class LayerStack : public RefCounted

	{
	public:
		LayerStack() = default;
		~LayerStack();
		void PushLayer(Count<Layer> layer);
		void PopLayer(Count<Layer> layer);
		void PushOverlay(Count<Layer> layer);
		void PopOverlay(Count<Layer> layer);

		void Empty();
		std::vector<Count<Layer>>::iterator begin() { return V_LayerStack.begin(); };
		std::vector<Count<Layer>>::iterator end() { return V_LayerStack.end(); };
		std::vector<Count<Layer>>::reverse_iterator rbegin() { return V_LayerStack.rbegin(); }
		//std::vector<Count<Layer>>::reverse_iterator rend() { return V_LayerStack.rend(); }
	private:
		std::vector<Count<Layer>>V_LayerStack;
		unsigned int LayerStackIndex = 0;
		friend class Application;
	};
}