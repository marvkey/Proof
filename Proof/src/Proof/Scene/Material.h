#pragma once

#include "Proof/Core/Core.h"
#include "Proof/Math/Math.h"
#include "Proof/Asset/Asset.h"
namespace Proof{
	struct Material : public Asset {
		Material();
		// setting mutable because we can always change
		// for liek when we get material through a material asset
		float Metallness =0.0f;// also shinines
		float Roughness = 0.0f;
		Vector Colour {1,1,1};

		Count<class Texture2D> AlbedoTexture;
		Count<class Texture2D> NormalTexture;
		Count<class Texture2D> MetallicTexture;
		Count<class Texture2D> RoughnessTexture;
		ASSET_CLASS_TYPE(Material);

	};

	class MaterialTable {
	public:
		MaterialTable(uint32_t size) {
			m_Materials.resize(size);
		}
		MaterialTable(class Mesh* mesh);
		MaterialTable(std::initializer_list<Count<Material>> materials) :
			m_Materials(materials)
		{

		}
		MaterialTable() {

		}
		// material can be nulltr
		void AddMaterial(Count<Material> material) {
			m_Materials.emplace_back(material);
		}

		void RemoveMaterial(uint32_t index) {
			PF_CORE_ASSERT(m_Materials.size() > index, "Material Tabel is not taht size");
			m_Materials.erase(m_Materials.begin() + index);
		}
		
		Count<Material> GetMaterial(uint32_t index) {
			PF_CORE_ASSERT(m_Materials.size() > index, "Material Tabel is not taht size");
			return m_Materials[index];
		}
		uint32_t GetSize() {
			m_Materials.size();
		}
	private:

		std::vector<Count<Material>> m_Materials;
	};
	enum class CombineMode 
	{
		Average =0,
		Min = 1,
		Mutltiply = 2,
		Max =3
	};
	struct PhysicsMaterial: public Asset
	{
		PhysicsMaterial();
		float StaticFriction = 0.6f;
		float DynamicFriction = 0.6f;
		float Bounciness = 0.0f;

		CombineMode FrictionCombineMode = CombineMode::Average;
		CombineMode BouncinessCombineMode = CombineMode::Average;
		ASSET_CLASS_TYPE(PhysicsMaterial);
	private:
		void* m_RuntimeBody = nullptr;
		friend class PhysicsEngine;
		friend class PhysicsActor;
	};
}