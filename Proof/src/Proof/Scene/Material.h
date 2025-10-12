#pragma once

#include "Proof/Core/Core.h"
#include "Proof/Core/Assert.h"
#include "Proof/Math/Math.h"
#include "Proof/Asset/Asset.h"
#include <map>
#include "Proof/Utils/VariableSystem/VariableUtils.h"
namespace Proof
{
	

	// for static mateiral not the transparent ones
	struct PbrSurfaceMaterial
	{
		PbrSurfaceMaterial(Count<class Material> material);

		glm::vec3& GetAlbedoColor()const;
		void SetAlbedo(const glm::vec3& vec);

		float& GetMetalness()const;
		void SetMetalness(float metallness);

		float& GetRoughness()const;
		void SetRoughness(float roghness);

		bool& GetNormalTextureToggle();
		void SetNormalTextureToggle(bool value);

		bool& GetEmissionOverrideColorToggle();
		void SetEmissionOverrideColorToggle(bool value);

		float& GetEmission()const;
		void SetEmission(float roghness);

		glm::vec3& GetEmissionOverrideColor()const;
		void SetEmissionOverrideColor(const glm::vec3& vec);

		glm::vec2& GetTiling()const;
		void SetTiling(const glm::vec2& vec);

		glm::vec2& GetOffset()const;
		void SetOffset(const glm::vec2&)const;

		void SetAlbedoMap(Count<class Texture2D> texture);
		void SetNormalMap(Count<class Texture2D> texture);
		void SetMetalnessMap(Count<class Texture2D> texture);
		void SetRoughnessMap(Count<class Texture2D> texture);

		Count<class Texture2D> GetAlbedoMap();
		Count<class Texture2D> GetNormalMap();
		Count<class Texture2D> GetMetalnessMap();
		Count<class Texture2D> GetRoughnessMap();
	private:
		Count<class Material> m_Material;
		Count<class RenderMaterial> m_RenderMaterial;
	};

	enum class MaterialTypes
	{
		Surface = 0, // pbr and toehrs
		PostProcess = 1, // post proccess custom thigns like user can write
		Other = 2,
	};


	// thse data only work if using Proofpbr_shader
	class Material : public Asset 
	{
	public:
		Material();
		Material(const std::string& name);
		Material(const std::string& name, Count<class Shader> shader);
		Material(const std::string& name, Count<class RenderMaterial> material);

		Material(const Material& material);
		MaterialTypes GetSurfaceType();
		ASSET_CLASS_TYPE(Material);
		std::string Name;
		
		Count<class RenderMaterial> GetRenderMaterial() { return m_RenderMaterial; };
		bool IsDefaultPbrShader()
		{
			return m_DefaultShader;
		}
		//used for rendering
		UUID GetMaterialSpecificID() {
			return m_UniqeMaterialID;
		}
		void SetMaterialShader(const std::string& materialName,Count<class Shader> shader);

		// maybe will add storage buffer later
		// taht can store more variables
		// for now only push constants
		std::map<std::string, VariableTypes> GetMaterialVariables(); // only push constants
		std::vector<std::string> GetEditableTextures(); // onluy set 0
	private:
		const UUID m_UniqeMaterialID = UUID();
		void SetDefault();
		bool m_DefaultShader = false;
		Count<class RenderMaterial> m_RenderMaterial;
	};


	class MaterialVariableRegistry : public RefCounted
	{
		MaterialVariableRegistry(Count<Material> material);

	private:
		Count<Material> m_Material;
		Count< class VariableRegistry> m_VariableRegistry;
	};


	class MaterialTable : public RefCounted 
	{
	public:
		MaterialTable(bool createMatIndex0Default = true, MaterialTypes type = MaterialTypes::Surface) 
		{
			m_Type = type;
			if(createMatIndex0Default)
				SetMaterial(0, Count<Material>::Create(fmt::format("Default")));
		}

		MaterialTable(Count<MaterialTable> other)
		{
			m_Materials = other->m_Materials;
		} 

		// index cna be existing or non exisitng
		void SetMaterial(uint32_t materialIndex,Count<Material> material) 
		{
			//if(material->GetSurfaceType() == m_Type)
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
		MaterialTypes m_Type;
	};

	bool operator==(const MaterialTable& other, const MaterialTable& other1);
	bool operator<(const MaterialTable& other, const MaterialTable& other1);
	bool operator>(const MaterialTable& other, const MaterialTable& other1);
}