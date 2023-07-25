#pragma once
#include "FrameBuffer.h"
#include "Proof/Renderer/3DRenderer/Renderer3DPBR.h"
#include "Proof/Scene/World.h"
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
	struct RenderSettings 
	{
		bool ViewColliders = false;
	};

	//used in shader don't change variable ordering
	struct DirectionalLight {
		Vector Color = { 1 };
		float Intensity = 1.0f;
		Vector Direction;
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
	class WorldRenderer : public RefCounted {
	public:
		//WorldRenderer()=default;
		virtual ~WorldRenderer();


		void BeginContext();
		void EndContext();
		void Render(const glm::mat4& projection, const glm::mat4& view, const Vector& location, Viewport viewport, ViewportScissor scissor, RenderSettings renderSettings, bool clearPreviousFrame = true, Count<UITable> uiTable = nullptr);
		WorldRenderer(Count<World>world, uint32_t textureWidth, uint32_t textureHeight);
		void Resize(ScreenSize windowSize);
		void SetContext(Count<World>world) {
			m_World = world;
		}
		
		void Clear();
		void Render(EditorCamera& camera, RenderSettings renderSettings);
		void Render(CameraComponent& comp, Vector& location, RenderSettings renderSettings,Count<UITable> uiTable =nullptr);
		void Render(CameraComponent& comp, Vector& location, Viewport viewport, ViewportScissor scissor, RenderSettings renderSettings,bool clearPreviousFrame = true, Count<UITable> uiTable = nullptr);
		Count<Image2D> GetImage(){
			return m_ScreenFrameBuffer->GetImage();
		}
		Count<ScreenFrameBuffer>m_ScreenFrameBuffer;

	private:
		LightScene m_LightScene;
		Special<Renderer3DPBR> m_Renderer3D;
		Special<class DebugMeshRenderer> m_DebugMeshRenderer;
		Special<class Renderer2D>  m_Renderer2D;
		Special<class Renderer2D>  m_UIRenderer;
		Special<class Renderer2D>  m_ParticleSystemRenderer;
		Count<RenderCommandBuffer> m_CommandBuffer;
		//Count<class RenderPass> m_RenderPass;
		Count<World>m_World=nullptr;

		Count<class Texture2D> m_BRDFLUT;

		Count<class StorageBuffer> m_DirectionalLights;
		void SubmitStaticMesh(Count<Mesh> mesh, Count<MaterialTable> materialTable, const glm::mat4& trnasform);

		void MeshPass();
		std::unordered_map<MeshKey, std::vector<glm::mat4>> m_MeshTransformMap;
		std::unordered_map<MeshKey, MeshDrawInfo> m_MeshDrawList;
		MeshRenderPipline m_MeshPipeline;

		void Reset();
	};	
}


