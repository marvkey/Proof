#include "Proofprch.h"
#include "LayerStack.h"
namespace Proof {

	LayerStack::~LayerStack() {
		for (Layer* layer : V_LayerStack) {
			layer->OnDetach();
			delete layer;
		}
	}

	void LayerStack::PushLayer(Layer* layer) {
		V_LayerStack.emplace(V_LayerStack.begin() + LayerStackIndex, layer);
		LayerStackIndex++;
	}

	void LayerStack::PopLayer(Layer* layer){
		auto it = std::find(V_LayerStack.begin(), V_LayerStack.begin() + LayerStackIndex, layer);
		if (it != V_LayerStack.begin() + LayerStackIndex) {
			layer->OnDetach();
			V_LayerStack.erase(it);
			LayerStackIndex--;
		}
	}

	void LayerStack::PushOverlay(Layer* layer) {
		V_LayerStack.emplace_back(layer);
	}

	void LayerStack::PopOverlay(Layer* layer) {
		auto it = std::find(V_LayerStack.begin() + LayerStackIndex, V_LayerStack.end(), layer);
		if (it != V_LayerStack.end()) {
			layer->OnDetach();
			V_LayerStack.erase(it);
		}
	}
}
