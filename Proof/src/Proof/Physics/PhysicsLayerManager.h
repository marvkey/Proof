#pragma once
#include <iostream>


//https://github.com/Ant-Play/Ant/blob/2dab7c0362f017911df9090b1608ec4b81ad1f2c/Ant/src/Ant/Physics/3D/PhysicsLayer.h
namespace Proof
{
	struct PhysicsLayer
	{
		uint32_t LayerID;
		std::string Name;
		uint32_t CollidesValue;//value used to collid
		int32_t CollidesWith = 0;

		bool IsValid() const
		{
			return !Name.empty() && CollidesValue > 0;
		}
	};
	class PhysicsLayerManager
	{
	public:
		static uint32_t AddLayer(const std::string& name, bool setCollisions = true);
		static void RemoveLayer(uint32_t LayerID);
		static void UpdateLayerName(uint32_t layerId, const std::string& newName);

		static void SetLayerCollision(uint32_t layerId, uint32_t otherLayer, bool shouldCollide);
		static std::vector<PhysicsLayer> GetLayerCollisions(uint32_t layerId);

		static const std::vector<PhysicsLayer>& GetLayers();
		static const std::vector<std::string>& GetLayersNames();

		static PhysicsLayer& GetLayer(uint32_t layerId);
		static PhysicsLayer& GetLayer(const std::string& layerName);
		static uint32_t GetLayerCount();

		static bool ShouldCollide(uint32_t layerA, uint32_t layerB);
		static bool IsLayerValid(uint32_t layerId);
		static bool IsLayerValid(const std::string& layerName);

		static void ClearLayers();
	private:
		static uint32_t GetNextLayerID();
	};
}