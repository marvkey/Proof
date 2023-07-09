#include "Proofprch.h"
#include "LayerStack.h"
#include "Layer.h"

namespace Proof {
	LayerStack::~LayerStack() {
		Empty();
	}

	void LayerStack::PushLayer(Count<Layer> layer) {
		V_LayerStack.emplace(V_LayerStack.begin() + LayerStackIndex, layer);
		LayerStackIndex++;
		PF_ENGINE_TRACE("{} Attached", layer->GetName().c_str());
		layer->OnAttach();
	}

	void LayerStack::PopLayer(Count<Layer> layer){
		auto it = std::find(V_LayerStack.begin(), V_LayerStack.begin() + LayerStackIndex, layer);
		if (it != V_LayerStack.begin() + LayerStackIndex) {
			PF_ENGINE_TRACE("{} Detach",layer->GetName().c_str());
			layer->OnDetach();
			V_LayerStack.erase(it);
			LayerStackIndex--;
		}
	}

	void LayerStack::PushOverlay(Count<Layer> layer) {
		V_LayerStack.emplace_back(layer);
		PF_ENGINE_TRACE("{} Attached",layer->GetName().c_str());
		layer->OnAttach();
	}

	void LayerStack::PopOverlay(Count<Layer> layer) {
		auto it = std::find(V_LayerStack.begin() + LayerStackIndex, V_LayerStack.end(), layer);
		if (it != V_LayerStack.end()) {
			PF_ENGINE_TRACE("{} Detach", layer->GetName().c_str());
			layer->OnDetach();
			V_LayerStack.erase(it);
		}
	}
	void LayerStack::Empty()
	{
		for (auto& layer : V_LayerStack)
		{
			layer->OnDetach();
			PF_ENGINE_TRACE("{} Detach ", layer->GetName().c_str());
			layer = nullptr;
		}
		V_LayerStack.clear();
		LayerStackIndex = 0;
	}
}
