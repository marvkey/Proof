#pragma once

#include "Proof/Core/Core.h"
#include "Proof/Math/Math.h"
#include "Proof/Asset/Asset.h"

#include <map>
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

		std::string Name;
		ASSET_CLASS_TYPE(Material);
		//used for rendering
		UUID GetMaterialSpecificID() {
			return m_UniqeMaterialID;
		}
	private:
		const UUID m_UniqeMaterialID = UUID();
	};

	class MaterialTable {
	public:
		MaterialTable() {
			SetMaterial(0, Count<Material>::Create());
		}
		Count<MaterialTable> Copy();
		// material can be nulltr
		// index cna be existing or non exisitng
		void SetMaterial(uint32_t materialIndex,Count<Material> material) {
			m_Materials[materialIndex] =material;
		}

		void RemoveMaterial(uint32_t materialIndex) {
			PF_CORE_ASSERT(HasMaterial(materialIndex), "Does not conatin material index");
			m_Materials.erase(materialIndex);
		}
		
		Count<Material> GetMaterial(uint32_t materialIndex) {
			PF_CORE_ASSERT(HasMaterial(materialIndex), "Does not conatin material index");
			return m_Materials[materialIndex];
		}

		uint32_t GetMaterialCount( ) {
			return m_Materials.size();
		}

		bool HasMaterial(uint32_t materialIndex)const {
			return m_Materials.contains(materialIndex);
		}

		const std::map<uint32_t, Count<Material>>& GetMaterials()const {
			return m_Materials;
		};

	private:
		// index, materisl
		std::map<uint32_t ,Count<Material>> m_Materials;
	};

	bool operator==(const MaterialTable& other, const MaterialTable& other1);
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