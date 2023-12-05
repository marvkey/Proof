#include "Proofprch.h"
#include "PhysicsLayerManager.h"
#include "Proof/Utils/ContainerUtils.h"
namespace Proof
{
	static std::vector<PhysicsLayer> s_PhysicsLayer;
	static std::vector<std::string> s_PhysicsLayerNames;
	static PhysicsLayer s_NullLayer = { (uint32_t)-1, "NULL", (uint32_t)-1, -1 };

    uint32_t PhysicsLayerManager::AddLayer(const std::string& name, bool setCollisions)
    {
        for (const auto& layer : s_PhysicsLayer)
        {
            if (layer.Name == name)
                return layer.LayerID;
        }

        uint32_t layerId = GetNextLayerID();
        PhysicsLayer layer = { layerId, name, BIT(layerId), BIT(layerId) };
        s_PhysicsLayer.insert(s_PhysicsLayer.begin() + layerId, layer);
		s_PhysicsLayerNames.insert(s_PhysicsLayerNames.begin() + layerId, name);

        for (const auto& layer2 : s_PhysicsLayer)
            SetLayerCollision(layer.LayerID, layer2.LayerID, setCollisions);

        return layer.LayerID;
    }
    void PhysicsLayerManager::RemoveLayer(uint32_t layerID)
    {
        PhysicsLayer& layerInfo = GetLayer(layerID);

        for (auto& otherLayer : s_PhysicsLayer)
        {
            if (otherLayer.LayerID == layerID)
                continue;

            if (otherLayer.CollidesWith & layerInfo.CollidesValue)
            {
                otherLayer.CollidesWith &= ~layerInfo.CollidesValue;
            }
        }

        Utils::RemoveIf(s_PhysicsLayer, [&](const PhysicsLayer& layer) { return layer.LayerID == layerID; });
		Utils::RemoveIf(s_PhysicsLayerNames, [&](const std::string& name) {return layerInfo.Name == name; });
    }
	void PhysicsLayerManager::UpdateLayerName(uint32_t layerId, const std::string& newName)
	{
		for (const auto& layer : s_PhysicsLayer)
		{
			if (layer.Name == newName)
				return;
		}
		PhysicsLayer& layer = GetLayer(layerId);
		const auto oldName = layer.Name;

		layer.Name = newName;

		for (auto& layerName : s_PhysicsLayerNames)
		{
			if (layerName == oldName)
			{
				layerName = newName;
				break;
			}
		}
		
	}

	void PhysicsLayerManager::SetLayerCollision(uint32_t layerId, uint32_t otherLayer, bool shouldCollide)
	{
		if (ShouldCollide(layerId, otherLayer) && shouldCollide)
			return;

		PhysicsLayer& layerInfo = GetLayer(layerId);
		PhysicsLayer& otherLayerInfo = GetLayer(otherLayer);

		if (shouldCollide)
		{
			layerInfo.CollidesWith |= otherLayerInfo.CollidesValue;
			otherLayerInfo.CollidesWith |= layerInfo.CollidesValue;
		}
		else
		{
			layerInfo.CollidesWith &= ~otherLayerInfo.CollidesValue;
			otherLayerInfo.CollidesWith &= ~layerInfo.CollidesValue;
		}
	}

	std::vector<PhysicsLayer> PhysicsLayerManager::GetLayerCollisions(uint32_t layerId)
	{
		const PhysicsLayer& layer = GetLayer(layerId);

		std::vector<PhysicsLayer> layers;
		for (const auto& otherLayer : s_PhysicsLayer)
		{
			if (otherLayer.LayerID == layerId)
				continue;

			if (layer.CollidesWith & otherLayer.CollidesValue)
				layers.push_back(otherLayer);
		}

		return layers;
	}

	const std::vector<PhysicsLayer>& PhysicsLayerManager::GetLayers()
	{
		return s_PhysicsLayer;
	}
	const std::vector<std::string>& PhysicsLayerManager::GetLayersNames()
	{
		return s_PhysicsLayerNames;
	}

	PhysicsLayer& PhysicsLayerManager::GetLayer(uint32_t layerId)
	{
		if (layerId < s_PhysicsLayer.size())
			return s_PhysicsLayer[layerId];
		PF_CORE_ASSERT(false, fmt::format("Layer does not Exist {}",layerId));
		return s_NullLayer;
	}

	PhysicsLayer& PhysicsLayerManager::GetLayer(const std::string& layerName)
	{
		for (auto& layer : s_PhysicsLayer)
		{
			if (layer.Name == layerName)
				return layer;
		}

		PF_CORE_ASSERT(false, fmt::format("Layer With name {} Does not exist", layerName));
		return s_NullLayer;
	}

	uint32_t PhysicsLayerManager::GetLayerCount()
	{
		return s_PhysicsLayer.size();
	}

	bool PhysicsLayerManager::ShouldCollide(uint32_t layer1, uint32_t layer2)
	{
		return GetLayer(layer1).CollidesWith & GetLayer(layer2).CollidesValue;
	}

	bool PhysicsLayerManager::IsLayerValid(uint32_t layerId)
	{
		PhysicsLayer layer = s_NullLayer;

		if (layerId < s_PhysicsLayer.size())
			layer = s_PhysicsLayer[layerId];

		return layer.LayerID != s_NullLayer.LayerID && layer.IsValid();
	}

	bool PhysicsLayerManager::IsLayerValid(const std::string& layerName)
	{
		PhysicsLayer layer = s_NullLayer;
		
		for (auto& l : s_PhysicsLayer)
		{
			if (l.Name == layerName) 
			{
				layer = l;
				break;
			}
		}
		return layer.LayerID != s_NullLayer.LayerID && layer.IsValid();
	}

	void PhysicsLayerManager::ClearLayers()
	{
		s_PhysicsLayer.clear();
	}

    uint32_t PhysicsLayerManager::GetNextLayerID()
    {
        int32_t lastId = -1;

        for (const auto& layer : s_PhysicsLayer)
        {
            if (lastId != -1)
            {
                if (layer.LayerID != lastId + 1)
                {
                    return lastId + 1;
                }
            }

            lastId = layer.LayerID;
        }

        return (uint32_t)s_PhysicsLayer.size();
    }
}
