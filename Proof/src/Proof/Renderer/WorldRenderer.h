#pragma once
#include "FrameBuffer.h"
#include "Proof/Core/Buffer.h"
#include "Proof/Scene/Camera/Camera.h"
#include "Viewport.h"
#include "Proof/Asset/AssetTypes.h"
#include <map>
#include "WorldRendererBuffers.h"
#include "WorldRendererSettings.h"
namespace Proof
{
	struct MeshKey
	{
		AssetID MeshID;
		AssetID MaterialID;
		uint32_t SubmeshIndex;
		bool IsSelected;


		Count<class RenderMaterial> Material = nullptr; // temporary for now 
		bool operator<(const MeshKey& other) const {

			// have to use everythign to make sure we are keeping unique

			// dont use memory location because each material table will propably have a different materialtabel because eahc mesh componet has a mateiral table
			//uint64_t memlocation1 = (uint64_t)MaterialTable.Get();
			//uint64_t memlocation2 = (uint64_t)other.MaterialTable.Get();

			if (MeshID < other.MeshID)
				return true;

			if (MeshID > other.MeshID)
				return false;

			if (SubmeshIndex < other.SubmeshIndex)
				return true;

			if (SubmeshIndex > other.SubmeshIndex)
				return false;

			if (MaterialID < other.MaterialID)
				return true;

			if (MaterialID > other.MaterialID)
				return false;

			return IsSelected < other.IsSelected;
		}

		// Define the greater than operator
		bool operator>(const MeshKey& other) const {
			return other < *this;
		}
	};
}

namespace Proof
{
	class RenderMaterial;
	class UniformBufferSet;
	class Mesh;
	class VertexBuffer;
	class DynamicMesh;
	class StorageBufferSet;
	class Texture2D;
	class Image2D;
	struct MeshDrawInfo
	{
		Count<Mesh> Mesh = nullptr;
		uint32_t SubMeshIndex = 0;
		Count<class MaterialTable> MaterialTable = nullptr;
		Count<class RenderMaterial> OverrideMaterial;
		uint32_t InstanceCount = 0;
		uint32_t InstanceOffset = 0; // for selected mesh
	};
	struct DynamicMeshDrawInfo
	{
		Count<DynamicMesh> Mesh = nullptr;
		uint32_t SubMeshIndex = 0;
		Count<MaterialTable> MaterialTable;
		Count<class RenderMaterial> OverrideMaterial;

		uint32_t InstanceCount = 0;
		uint32_t InstanceOffset = 0;
		bool IsRigged = false;
	};
	struct SubMeshKey
	{
		AssetID MeshID;
		uint32_t MeshIndex;
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
	
	struct WorldRendererTimers
	{
		//set passes
		float SetPasses = 0.0f;

		float ShadowPass= 0.0f;

		float PreDepthPass = 0.0f;

		// geometry pass
		float GeometryPass = 0.0f;
		float GeometryMeshPass = 0.0f;
		float GeometryDynamicMeshPass = 0.0f;
		float GeometrySkyBoxPass = 0.0f;

		//lighting
		float LightCalculateGridFrustum = 0.0f;
		float LightCulling = 0.0f;
		//composite
		float CompositePass =0.0f;
		float DrawPhysicsColliders = 0.0f;

		// totla render time
		float TotalDrawScene = 0.0f;
	};

	struct WorldRendererStatistics
	{
		WorldRendererTimers Timers;
		UBLightScene LightSene;
	};
	


	class RenderPass;
	class ComputePass;

	struct WorldRendererCamera
	{
		Camera Camera;
		float NearPlane;
		float FarPlane;
		float Fov;
	};
	class WorldRenderer : public RefCounted {
	public:

		WorldRenderer();

		virtual ~WorldRenderer();

		void SetContext(Count<class World> world);
		// make sure all settings for the renderer are set before callign this fucntion
		// like make sure shadows are set to teh write shadow quality
		void BeginScene(const WorldRendererCamera& camera, const glm::vec3& location);
		void EndScene();

		void SubmitSkyLight(const UBSkyLight& skyLight, Count<class Environment> environment);
		void SubmitDirectionalLight(const SBDirectionalLightsSceneData& directionaLights);
		void SubmitPointLight(const SBPointLightSceneData& pointLights);
		void SubmitSpotLight(const SBSpotLightSceneData& spotLights);
		void SubmitMesh(Count<Mesh> mesh, Count<MaterialTable> materialTable, const glm::mat4& transform, bool CastShadowws = true);
		void SubmitDynamicMesh(Count<DynamicMesh> mesh, Count<MaterialTable> materialTable, uint32_t subMeshIndex,const glm::mat4& transform, bool CastShadowws = true);

		void SubmitPhysicsDebugMesh(Count<Mesh> mesh, const glm::mat4& transform);
		void SubmitPhysicsDynamicDebugMesh(Count<DynamicMesh> mesh, uint32_t subMeshIndex,const glm::mat4& transform);
		// if the same size is passed it will not resize
		void SetViewportSize(uint32_t width, uint32_t height);

		Count<Image2D> GetFinalPassImage();
		Count<Image2D> GetShadowPassDebugImage();
		
		//external can use this to drw to
		Count<FrameBuffer> GetExternalCompositePassFrameBuffer() { return m_ExternalCompositeFrameBuffer; }
		Count<class Renderer2D> GetRenderer2D() { return m_Renderer2D; }
		const WorldRendererStatistics& GetStats() const
		{
			return m_Stats;
		}
	public:
		// preProcess
		WorldRendererDebugOptions DebugOptions;
		PreProcessSettings PreProcessSettings;
		ShadowSetting& ShadowSetting;
		//postprocess
		PostProcessSettings PostProcessSettings;
		AmbientOcclusion& AmbientOcclusionSettings;
		BloomSettings& BloomSettings;
		DepthOfFieldSettings& DOFSettings;
		SSRSettings& SSRSettings;
	private:
		Count<class World> m_ActiveWorld;
		//buffer data
		UBRenderData m_UBRenderData;
		UBSceneData m_UBSceneData;
		UBScreenData m_UBScreenData;
		UBCameraData m_UBCameraData;
		UBLightScene m_UBLightData;
		UBHBAOData m_HBAOData;
		UBSSR m_UBSSR;

		//buffer sts
		Count<UniformBufferSet> m_UBRenderDataBuffer;
		Count<UniformBufferSet> m_UBSceneDataBuffer;
		Count<UniformBufferSet> m_UBCameraBuffer;
		Count<UniformBufferSet> m_UBSKyBoxBuffer;
		Count<UniformBufferSet> m_UBCascadeProjectionBuffer;
		Count<UniformBufferSet> m_UBScreenBuffer;
		Count<UniformBufferSet> m_UBLightSceneBuffer;
		Count<UniformBufferSet> m_UBHBAOBuffer;

		//storagebuffer
		Count<StorageBufferSet> m_SBDirectionalLightsBuffer;
		Count<StorageBufferSet> m_SBPointLightsBuffer;
		Count<StorageBufferSet> m_SBSpotLightsBuffer;
		
		Count<class GlobalBufferSet> m_GlobalInputs;
		// mesh data
		std::vector< TransformBuffer>  m_SubmeshTransformBuffers; // vector because of frame in flight

		//Count<class RenderPass> m_ShadowDepthRenderPass;
		std::array<Count<class RenderPass>, 4> m_ShadowMapPasses; // for cascades
		//Count<RenderMaterial> m_ShadowPassMaterial;
		//Count<GraphicsPipeline> m_ShadowPassPipeline;
		Count<class GraphicsPipeline> m_ShadowDebugPipeline;
		Count<class RenderPass> m_ShadowDebugPass;
		Count<UniformBufferSet> m_ShadowPassBuffer;
		Count<class Image2D> m_ShadowPassImage;

		//meshes
		Count<Mesh> m_Cube;
		Special<class DebugMeshRenderer> m_DebugMeshRenderer;
		Count<class Renderer2D>  m_Renderer2D;
		Special<class Renderer2D>  m_UIRenderer;
		Special<class Renderer2D>  m_ParticleSystemRenderer;
		Count<class RenderCommandBuffer> m_CommandBuffer;

		Count<class StorageBufferSet> m_SBDirectionalLights;

		std::map<MeshKey, TransformMapData> m_MeshTransformMap[2];
		std::map<MeshKey, TransformMapData>* m_CurTransformMap;
		std::map<MeshKey, TransformMapData>* m_PrevTransformMap;

		std::map<MeshKey, MeshDrawInfo> m_MeshDrawList;
		std::map<MeshKey, DynamicMeshDrawInfo> m_DynamicMeshDrawList;

		std::map<MeshKey, MeshDrawInfo> m_MeshShadowDrawList;
		std::map<MeshKey, DynamicMeshDrawInfo> m_DynamicMeshShadowDrawList;

		//debg 
		std::map<MeshKey, MeshDrawInfo> m_ColliderDrawList;
		std::map<MeshKey, DynamicMeshDrawInfo> m_DynamicColliderDrawList;

		Count<class Environment> m_Environment;
		bool m_InContext = false;
		uint32_t m_ShadowMapResolution;
		// geometry pass
		Count<RenderPass> m_GeometryPass;

		// pre pass
		Count<RenderPass> m_PreDepthPass;

		// debug
		Count<RenderPass> m_GeometryWireFramePass;
		Count<RenderPass> m_GeometryWireFrameOnTopPass;
		Count<class Material> m_GeometryWireFramePassMaterialAsset;
		Count<RenderMaterial> m_GeometryWireFramePassMaterial;// points to the render material of m_GeometryWireFramePassMaterialAsset

		// compoiste pass
		Count<RenderPass> m_SkyBoxPass;
		Count<RenderPass> m_CompositePass;
		Count<RenderMaterial> m_CompositeMaterial;
		Count<FrameBuffer> m_ExternalCompositeFrameBuffer;
		//AO
		Count<ComputePass> m_SSAOPass;
		Count<RenderMaterial> m_SSAOMaterial;
		Count<StorageBufferSet> m_SSAOSampleBuffer;
		Count<RenderPass> m_SSAOBlurPass;
		Count<Texture2D> m_SSAOImage;
		Count<Texture2D> m_SSAONoiseImage;
		Count<StorageBufferSet> m_SBSSAOSampleKernalBuffer;

		Count<RenderPass> m_AmbientOcclusionCompositePass;
		
		bool m_NeedResize = false;

		DirectionalLight m_MainDirectionllLight;

		// stats make sure this is first because variables gets initilized in class form top to bottom
		WorldRendererStatistics m_Stats;

		WorldRendererTimers& m_Timers;
		UBLightScene& m_LightScene;

		glm::uvec3 m_LightCullingWorkGroups;
		Count<class ComputePass> m_LightCullingPass;
		Count<StorageBufferSet> m_SBVisiblePointLightIndicesBuffer;
		Count<StorageBufferSet> m_SBVisibleSpotLightIndicesBuffer;

		inline static const uint32_t TILE_SIZE = 16u;
		inline static const uint32_t MAX_NUM_LIGHTS_PER_TILE = 1024u;

		// HBAO

		struct AmbientOcclusionRenderer
		{
			struct HBAO
			{
				Count<ComputePass> HBAOPass;
				Count<RenderPass> ReinterleavePass;
				Count<RenderPass> DeinterleavePass[2];
				Count<RenderPass> BlurPass[2];
				Count<class Image2D> HBAOOutputImage;
				glm::uvec3 WorkGroups{ 1 };
			} HBAO;
		} m_AmbientOcclusion;

		struct SSR
		{
			Count<RenderPass> SSRCompositePass;
			Count<ComputePass> SSRPass;
			Count<ComputePass> HierarchicalDepthPass;
			Count<ComputePass> PreIntegrationPass;
			Count<ComputePass> PreConvolutePass;
			Count<class Image2D> SSRImage;
			Count<class Texture2D> HierarchicalDepthTexture;
			Count<Texture2D> PreConvolutedTexture;
			Count<Texture2D> VisibilityTexture;//Pre-integration texture
			glm::uvec3 WorkGroups{ 1u };
		} m_SSR;

		struct NewSSR
		{
			Count<ComputePass> TileClassification;
			Count<ComputePass> IntersectArgs;

			Count<StorageBufferSet> SBRayCounter;
			Count<StorageBufferSet> SBRayList;
			Count<StorageBufferSet> SBDenoiseList;
			Count<StorageBufferSet> SBIntersectCommand;
			Count<StorageBufferSet> SBDenoiseCommand;

			Count<Image2D> Variance;
			Count<Image2D> PreviousVariance;

			Count<Image2D> Radiance;
			Count<Image2D> PreviousRadiance;

			Count<Image2D> ExtractRoughness;
			Count<Image2D> PreviousExtractRoughness;
			Count<class GlobalBufferSet> GlobalBuffer;
			struct RayCounterData			
			{
				uint32_t RayCount;
				uint32_t DenoiseTileCount;
			};
			struct GPUDispatchIndirectCommand
			{
				uint32_t x;
				uint32_t y;
				uint32_t z;
				uint32_t pad;
			};
		} m_NewSSR;
		//bloom
		Count<ComputePass> m_BloomComputePass;
		Count<Texture2D> m_BloomDirtTexture;
		std::vector<Count<Texture2D>> m_BloomComputeTextures{ 3 };
		// 
		Count<ComputePass> m_DOFPass;
		Count<Texture2D> m_DOFTexture;
		bool m_IsReady = false;
		bool m_ResourcesCreatedGPU = false;
		bool m_ResourcesCreated = false;
	private:
		void Init();
		void CalculateCascades(CascadeData* cascades, const glm::vec3& lightDirection);
		void CalculateCascadesManualSplit(CascadeData* cascades, const glm::vec3& lightDirection);
		
		// set up all the passes for current render
		void SetPasses();
		void ShadowPass();
		void PreDepthPass();
		void GeometryPass();
		void LightFrustrumAndCullingPass();
		void CompositePass();

		void HZBPass();
		void PreIntegrationPass();
		void PreConvolutePass();
		//post processing passes

		void AmbientOcclusionPass();
		void HBAOPass();

		void BloomPass();
		void DOFPass();

		void SSRPass();
		void NewSSRPass();
		void DrawScene();
		void ClearPass(Count<RenderPass> renderPass, bool explicitClear);
		// tehse are static so basically when wer are writng code we avoid errors of 
		// writing code to a speicif world rendere class

		static void RenderMesh(Count<RenderCommandBuffer>& commandBuffer, Count<Mesh>& mesh, Count<RenderPass>& renderPass, Count<VertexBuffer>& transformBuffer, uint32_t subMeshIndex, uint32_t transformOffset,  uint32_t instanceCount, const Buffer& pushData = Buffer(), const std::string& pushName = "");
		static void RenderDynamicMesh(Count<RenderCommandBuffer>& commandBuffer, Count<DynamicMesh>& mesh, Count<RenderPass>& renderPass, Count<VertexBuffer>& transformBuffer, uint32_t subMeshIndex, uint32_t transformOffset,  uint32_t instanceCount, const Buffer& pushData = Buffer(), const std::string& pushName ="");

		static void RenderMeshWithMaterial(Count<RenderCommandBuffer>& commandBuffer, Count<Mesh>& mesh, Count<RenderMaterial>& material, Count<RenderPass>& renderPass, Count<VertexBuffer>& transformBuffer, uint32_t subMeshIndex, uint32_t transformOffset, uint32_t instanceCount);
		static void RenderDynamicMeshWithMaterial(Count<RenderCommandBuffer>& commandBuffer, Count<DynamicMesh>& mesh, Count<RenderMaterial>& material, Count<RenderPass>& renderPass, Count<VertexBuffer>& transformBuffer, uint32_t subMeshIndex,uint32_t transformOffset, uint32_t instanceCount);
		
		static void RenderMeshWithMaterialTable(Count<RenderCommandBuffer>& commandBuffer,Count<Mesh>&mesh, Count<MaterialTable>& materialTable, Count<RenderPass>& renderPass , Count<VertexBuffer>& transformBuffer, uint32_t subMeshIndex, uint32_t transformOffset, uint32_t instanceCount);
		static void RenderDynamicMeshWithMaterialTable(Count<RenderCommandBuffer>& commandBuffer, Count<DynamicMesh>& mesh, Count<MaterialTable>& materialTable, Count<RenderPass>& renderPass, Count<VertexBuffer>& transformBuffer, uint32_t subMeshIndex, uint32_t transformOffset, uint32_t instanceCount);
		friend class Editore3D;
		friend class WorldRendererPanel;
		friend class ViewPortEditorWorkspace;
	};	
}


