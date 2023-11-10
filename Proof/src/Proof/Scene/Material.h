#pragma once

#include "Proof/Core/Core.h"
#include "Proof/Core/Assert.h"
#include "Proof/Math/Math.h"
#include "Proof/Asset/Asset.h"
#include <map>
namespace Proof{
	// thse data only work if using Proofpbr_shader
	class Material : public Asset {
	public:

		Material();
		Material(const std::string& name);
		Material(const std::string& name, Count<class Shader> shader);
		Material(const std::string& name,Count<class RenderMaterial> material);
		ASSET_CLASS_TYPE(Material);
		std::string Name;

		Vector& GetAlbedoColor()const;
		void SetAlbedo(const Vector& vec);

		float& GetMetalness()const;
		void SetMetalness(float metallness);

		float& GetRoughness()const;
		void SetRoughness(float roghness);

		bool& GetAlbedoTextureToggle();
		void SetAlbedoTextureToggle(bool value);

		bool& GetNormalTextureToggle();
		void SetNormalTextureToggle(bool value);

		bool& GetRoughnessTextureToggle();
		void SetRoughnessTextureToggle(bool value);

		bool& GetMetalnessTextureToggle();
		void SetMetalnessTextureToggle(bool value);


		float& GetEmission()const;
		void SetEmission(float roghness);

		Vector2& GetTiling()const;
		void SetTiling(const Vector2& vec);

		Vector2& GetOffset()const;
		void SetOffset(const Vector2&)const;

		void SetAlbedoMap(Count<class Texture2D> texture);
		void SetNormalMap(Count<class Texture2D> texture);
		void SetMetalnessMap(Count<class Texture2D> texture);
		void SetRoughnessMap(Count<class Texture2D> texture);

		Count<class Texture2D> GetAlbedoMap();
		Count<class Texture2D> GetNormalMap();
		Count<class Texture2D> GetMetalnessMap();
		Count<class Texture2D> GetRoughnessMap();
		Count<class RenderMaterial> GetRenderMaterial() { return m_RenderMaterial; };
		bool IsDefaultPbrShader()
		{
			return m_DefaultShader;
		}
		//used for rendering
		UUID GetMaterialSpecificID() {
			return m_UniqeMaterialID;
		}
	private:
		const UUID m_UniqeMaterialID = UUID();
		void SetDefault();
		bool m_DefaultShader = false;
		Count<class RenderMaterial> m_RenderMaterial;
		#if  0
		/*
		float& GetMetallness()const;
		void SetMetallness(float metallness);

		float& GetRoughness()const;
		void SetRoughness(float roghness);

		Vector& GetColour()const;
		void SetColour(Vector vec);

		glm::vec2& GetTiling()const;
		void SetTiling(glm::vec2);

		glm::vec2& GetOfset()const;
		void SetOffset(glm::vec2)const;
		*/
		
		const std::string& GetName()const { return Name; };
		std::string Name;
		float Metallness = 0.0f;
		float Roughness = 0.0f;
		Vector Colour{ 1,1,1 };
		glm::vec2 Tiling{ 1,1 };
		glm::vec2 Offset{ 0,0 };
		bool UsePBR = true;

		Count<class Texture2D> AlbedoTexture;
		Count<class Texture2D> NormalTexture;
		Count<class Texture2D> MetallicTexture;
		Count<class Texture2D> RoughnessTexture;
		Count<class RenderMaterial> m_RenderMaterial;
		#endif

		

	};

	class MaterialTable : public RefCounted {
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
	bool operator<(const MaterialTable& other, const MaterialTable& other1);
	bool operator>(const MaterialTable& other, const MaterialTable& other1);

	
}