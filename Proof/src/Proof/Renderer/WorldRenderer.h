#pragma once
#include "FrameBuffer.h"
#include "Proof/Renderer/3DRenderer/Renderer3DPBR.h"
#include "Proof/Scene/World.h"
#include <map>
#include "Viewport.h"
namespace Proof
{
	struct MeshKey
	{
		AssetID MeshID;
		Count<class MaterialTable> MaterialTable;
		 // Define the equality operator
		bool operator==(const MeshKey& other) const {
			return MeshID == other.MeshID && *MaterialTable == *other.MaterialTable;
		}

		 // Define the less than operator
		bool operator<(const MeshKey& other) const {

			// Calculate the hash value using the memory location of MaterialTable and the AssetID (uint64_t).
			size_t hashValue = std::hash<uint64_t>()((uint64_t)MeshID);
			for (const auto& [index, material] : MaterialTable->GetMaterials())
			{
				uint64_t location = (uint64_t)material->GetID();
				hashValue ^= std::hash< uint64_t>()(location);
			}

			size_t otherHashValue = std::hash<uint64_t>()((uint64_t)other.MeshID);
			for (const auto& [index, material] : other.MaterialTable->GetMaterials())
			{
				uint64_t location = (uint64_t)material->GetID();
				otherHashValue ^= std::hash< uint64_t>()(location);
			}



			if (hashValue < otherHashValue) return true;
			if (hashValue > otherHashValue) return false;
			return false;
		}

		// Define the greater than operator
		bool operator>(const MeshKey& other) const {
			return other < *this;
		}
	};
}

namespace std {
	template <>
	struct hash < Proof::MeshKey > {
		size_t operator()(const Proof::MeshKey& key) const {
			// Calculate the hash value using the memory location of MaterialTable and the AssetID (uint64_t).
			size_t hashValue = std::hash<uint64_t>()((uint64_t)key.MeshID);
			for (const auto& [index, material] : key.MaterialTable->GetMaterials())
			{
				uint64_t location = (uint64_t)material->GetID();
				hashValue ^= std::hash< uint64_t>()(location);
			}
			return hashValue;
		}
	};
}
namespace Proof
{
	class RenderMaterial;

	struct CascadeData
	{
		//glm::mat4 Projection;
		glm::mat4 ViewProjection;
		float SplitDepth;
	};

	struct RenderSettings 
	{
		bool ViewColliders = false;
	};

	//used in shader don't change variable ordering
	struct DirectionalLight {
		Vector Color = { 1 };
		float Intensity = 1.0f;
		Vector Direction;
		float ShadowAmount = 0.5;
	};
	
	struct SkyLight 
	{
		Vector TintColor = { 1 };
		float Lod = 0;
		float Intensity = 1;
		float Rotation = 0;
	};
	
	struct LightScene
	{
		std::vector<DirectionalLight> DirectionalLights;
	};
	struct MeshDrawInfo
	{
		Count<Mesh> Mesh = nullptr;
		Count<MaterialTable> MaterialTable = nullptr;
		//Count<RenderMaterial> OvverrideMaterial;
		uint32_t InstanceCount = 0;
	};
	struct SubMeshKey
	{
		AssetID MeshID;
		uint32_t MeshIndex;
	};
	struct MeshRenderPipline
	{
		Count<class GraphicsPipeline> Pipline;
		Count<class RenderPass> RenderPass;
		Count<class VertexBuffer> TransformsBuffer;
		std::vector<glm::mat4> Transforms;
	};

	struct ShadowSetting
	{
		float Near = -50.0f;
		float Far = 50.0f;
	};
	
	class WorldRenderer : public RefCounted {
	public:

		//WorldRenderer()=default;
		virtual ~WorldRenderer();


		void BeginContext();
		void EndContext();
		void Render(const glm::mat4& projection, const glm::mat4& view, const Vector& location, float nearPlane, float farPlane,Viewport viewport, ViewportScissor scissor, RenderSettings renderSettings, bool clearPreviousFrame = true, Count<UITable> uiTable = nullptr);
		WorldRenderer(Count<World>world, uint32_t textureWidth, uint32_t textureHeight);
		void Resize(ScreenSize windowSize);
		void SetContext(Count<World>world) {
			m_World = world;
		}
		
		void Clear();
		void Render(EditorCamera& camera, RenderSettings renderSettings);
		void Render(CameraComponent& comp, Vector& location, RenderSettings renderSettings,Count<UITable> uiTable =nullptr);
		void Render(CameraComponent& comp, Vector& location, Viewport viewport, ViewportScissor scissor, RenderSettings renderSettings,bool clearPreviousFrame = true, Count<UITable> uiTable = nullptr);
		Count<Image2D> GetImage();
		Count<ScreenFrameBuffer>m_ScreenFrameBuffer;
		void SubmitStaticMesh(Count<Mesh> mesh, Count<MaterialTable> materialTable, const glm::mat4& trnasform);

		ShadowSetting ShadowSettings;

		int debugCascade = 0;
	private:
		//Count<class RenderPass> m_ShadowDepthRenderPass;
		std::array<Count<RenderPass>, 4> m_ShadowMapPasses; // for cascades
		//Count<RenderMaterial> m_ShadowPassMaterial;
		//Count<GraphicsPipeline> m_ShadowPassPipeline;
		Count<GraphicsPipeline> m_ShadowDebugPipeline;
		Count<RenderPass> m_ShadowDebugPass;
		Count<UniformBufferSet> m_ShadowPassBuffer;
		Count<class Image2D> m_ShadowPassImage;
		Count<RenderMaterial> m_ShadowPassMaterial;
		Count<RenderMaterial> m_ShadowPassDebugMaterial;
		void CreateShadowMap();
		void MeshPass();
		void ShadowPass();

		LightScene m_LightScene;
		Special<Renderer3DPBR> m_Renderer3D;
		Special<class DebugMeshRenderer> m_DebugMeshRenderer;
		Special<class Renderer2D>  m_Renderer2D;
		Special<class Renderer2D>  m_UIRenderer;
		Special<class Renderer2D>  m_ParticleSystemRenderer;
		Count<RenderCommandBuffer> m_CommandBuffer;
		Count<World>m_World=nullptr;

		Count<class Texture2D> m_BRDFLUT;

		Count<class StorageBufferSet> m_DirectionalLights;

		std::map<MeshKey, std::vector<glm::mat4>> m_MeshTransformMap;
		std::map<MeshKey, MeshDrawInfo> m_MeshDrawList;
		MeshRenderPipline m_MeshPipeline;
		Count<class Environment> m_Environment;
		Count<UniformBufferSet> m_SkyBoxUniformInfo;

		void UpdateCascades(CascadeData* cascades, const glm::vec3& lightDirection);
		void Reset();
		CameraData m_CameraData;
		friend class Editore3D;

	};	
}


