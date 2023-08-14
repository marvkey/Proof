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

		bool AllSubMeshes;
		uint32_t SubmeshIndex;// all submeshes has to be set to false before this is uses
		bool IsSelected;
		
		bool operator<(const MeshKey& other) const {

			// have to use everythign to make sure we are keeping unique

			// dont use memory location because each material table will propably have a different materialtabel because eahc mesh componet has a mateiral table
			//uint64_t memlocation1 = (uint64_t)MaterialTable.Get();
			//uint64_t memlocation2 = (uint64_t)other.MaterialTable.Get();

			if (MaterialTable->GetMaterialCount() < other.MaterialTable->GetMaterialCount())
				return true;
			if (MaterialTable->GetMaterialCount() > other.MaterialTable->GetMaterialCount())
				return false;

			if (MeshID < other.MeshID)
				return true;

			if (MeshID > other.MeshID)
				return false;

			if (AllSubMeshes)
			{
				if (SubmeshIndex < other.SubmeshIndex)
					return true;

				if (SubmeshIndex > other.SubmeshIndex)
					return true;
			}

			// this here because it would do an operotr over ach materials in the table 
			// a little expensive if the material tabel has oaver liek a 1000 eleemtns wich prolly will never happen
			if (*MaterialTable < *other.MaterialTable)
				return true;
			if (*MaterialTable > *other.MaterialTable)
				return false;


			return IsSelected < other.IsSelected;
		}

		// Define the greater than operator
		bool operator>(const MeshKey& other) const {
			return other < *this;
		}
	};
}

// std map does not use a hash it uses 
/*
	Here's how std::map uses the comparison to maintain the order:

	When you insert a new element, the map compares it with existing elements using the comparison operator < (or your custom comparison function).
	When you retrieve an element using its key, the map uses the same comparison operator to find the corresponding element efficiently.
	The elements in a std::map are always sorted according to their keys based on the comparison operator.
*/
//namespace std {
//	template <>
//	struct hash < Proof::MeshKey > {
//		size_t operator()(const Proof::MeshKey& key) const {
//			// Calculate the hash value using the memory location of MaterialTable and the AssetID (uint64_t).
//			size_t hashValue = std::hash<uint64_t>()((uint64_t)key.MeshID);
//			for (const auto& [index, material] : key.MaterialTable->GetMaterials())
//			{
//				uint64_t location = (uint64_t)material->GetID();
//				hashValue ^= std::hash< uint64_t>()(location);
//			}
//			return hashValue;
//		}
//	};
//}
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
	struct DirectionalLight 
	{
		Vector Color = { 1 };
		float Intensity = 0.0f;
		Vector Direction; // Make sure TO NORMALIZED

		int CastShadows = 1;//bool
		int CastSoftShadows = 1;// bool
		float ShadowStrength = 0.0f;
		float ShadowSoftness = 0.0f;
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
		uint32_t InstanceOffset = 0; // for selected mesh
	};
	struct DynamicMeshDrawInfo
	{
		Count<Mesh> Mesh = nullptr;
		uint32_t SubmeshIndex;
		Count<MaterialTable> MaterialTable;
		//Ref<VulkanMaterial> OverrideMaterial;

		uint32_t InstanceCount = 0;
		uint32_t InstanceOffset = 0;
		bool IsRigged = false;
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
	};

	struct ShadowSetting
	{
		//casde fading
		bool CascadeFading = true;
		float TransitionFade = 1.0f; // min 0, max (Float Max)

		float CascadeSplitLambda = 0.92f; // (min 0.0 max 1.0)
		float CascadeNearPlaneOffset = -50.0f; // min - Floatmax, max 0
		float CascadeFarPlaneOffset = 50.0f; // min 0, max Float Max
		float ScaleShadowCascadesToOrigin = 0.f; //min 0.0f max 1.0f
		

		//use Manual cascadeSPlits
		bool UseManualCascadeSplits;
		float CascadeSplits[4] = { 0.1,0.2,0.3,1.0 }; // min 0, max Flt max
	};
	struct TransformVertexData
	{
		//glm::mat4 not using that because the last row of a transform buffer is always 0,0,0,1 so we are saving data and we wll just set that in the shader
		//glm::vec4 MRow[3]; 
		// dont use glm::vec4[3] for now becaue when we do we get a wierd bug of transforms messing up 
		glm::mat4 Transform;
	};

	struct TransformBuffer
	{
		Count<VertexBuffer> Buffer;
		TransformVertexData* Data = nullptr;
	};
	struct TransformMapData
	{
		std::vector<TransformVertexData> Transforms;
		uint32_t TransformOffset = 0;
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
		void SubmitStaticMesh(Count<Mesh> mesh, Count<MaterialTable> materialTable, const glm::mat4& trnasform, bool CastShadowws = true);

		ShadowSetting ShadowSetting;

		int debugCascade = 0;
	private:

		// set up all the passes for current render
		void PrePass();
		// mesh data
		std::vector< TransformBuffer>  m_SubmeshTransformBuffers; // vector because of frame in flight

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

		std::map<MeshKey, TransformMapData> m_MeshTransformMap;
		std::map<MeshKey, MeshDrawInfo> m_MeshDrawList;
		std::map<MeshKey, MeshDrawInfo> m_MeshShadowDrawList;
		MeshRenderPipline m_MeshPipeline;
		Count<class Environment> m_Environment;
		Count<UniformBufferSet> m_SkyBoxUniformInfo;

		void CalculateCascades(CascadeData* cascades, const glm::vec3& lightDirection);
		void CalculateCascadesManualSplit(CascadeData* cascades, const glm::vec3& lightDirection);
		void Reset();

		// tehse are static so basically when wer are writng code we avoid errors of 
		// writing code to a speicif world rendere class
		static void RenderMeshWithMaterial(Count<RenderCommandBuffer> commandBuffer, Count<Mesh>, Count<RenderMaterial> material, Count<RenderPass> renderPass, Count<VertexBuffer> transformBuffer, uint32_t transformOffset, uint32_t instanceCount);
		static void RenderMeshWithMaterialTable(Count<RenderCommandBuffer> commandBuffer,Count<Mesh>, Count<RenderPass> renderPass, Count<MaterialTable> materialTable, Count<VertexBuffer> transformBuffer, uint32_t transformOffset, uint32_t instanceCount);
		CameraData m_CameraData;
		friend class Editore3D;

	};	
}


