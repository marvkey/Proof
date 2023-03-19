#pragma once

#include "Proof/Core/Core.h"
#include "Proof/Math/Math.h"
#include "Proof/Asset/Asset.h"

#include <map>
namespace Proof{
	struct Material : public Asset {
		Material(const std::string& name = "");
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
		MaterialTable(bool createMatIndex0Default = true) {
			if(createMatIndex0Default)
				SetMaterial(0, Count<Material>::Create(fmt::format("Default")));
		}
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
		PhysicsMaterial(float staticFriction =0.6f, float dynamicFriction = 0.6f, float bounciness = 0.0f);
		float GetStaticFriction() {
			return m_StaticFriction;
		}
		float GetDynamicFriction() {
			return m_DynamicFriction;
		}
		float GetBounciness() {
			return m_Bounciness;
		}

		void SetStaticFriction(float friciono);
		void SetDynamicFriction(float friction);
		void SetBounciness(float bounciness);

		CombineMode GetFrictionCombineMode() {
			return m_FrictionCombineMode;
		}

		CombineMode GetBouncinessCombineMode() {
			return m_BouncinessCombineMode;
		}

		void SetFrictionCombineMode(CombineMode mode);
		void SetBouncinessCombineMode(CombineMode mode);

		ASSET_CLASS_TYPE(PhysicsMaterial);
	private:
		float m_StaticFriction = 0.6f;
		float m_DynamicFriction = 0.6f;
		float m_Bounciness = 0.0f;

		CombineMode m_FrictionCombineMode = CombineMode::Average;
		CombineMode m_BouncinessCombineMode = CombineMode::Average;
		void* m_RuntimeBody = nullptr;
		friend class PhysicsEngine;
		friend class PhysicsActor;
	};
}