#pragma once
#include "FrameBuffer.h"
#include "Proof/Core/Buffer.h"
#include "Proof/Scene/Camera/Camera.h"
#include "Viewport.h"
#include "Proof/Asset/AssetTypes.h"
#include <map>
#include "WorldRendererBuffers.h"
#include "WorldRendererSettings.h"
#include "Proof/Renderer/RenderereCustomTypeSet.h"
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

		float ShadowPass = 0.0f;

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
		float CompositePass = 0.0f;
		float DrawPhysicsColliders = 0.0f;

		// totla render time
		float TotalDrawScene = 0.0f;
	};

	struct WorldRendererStatistics
	{
		WorldRendererTimers Timers;
		UBLightScene LightSene;
		uint32_t TotalMeshProcessedCpu = 0;
		uint32_t TotalMeshSentToGpu = 0;

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
		void SubmitMesh(Count<Mesh> mesh, Count<RenderMaterial> renderMaterial, const glm::mat4& transform, bool CastShadowws = true);
		void SubmitMesh(Count<Mesh> mesh, uint32_t subMeshIndex,Count<RenderMaterial> renderMaterial, const glm::mat4& transform, bool CastShadowws = true);
		void SubmitMesh(Count<Mesh> mesh, Count<MaterialTable> materialTable, const glm::mat4& transform, bool CastShadowws = true);
		void SubmitDynamicMesh(Count<DynamicMesh> mesh, Count<MaterialTable> materialTable, uint32_t subMeshIndex, const glm::mat4& transform, bool CastShadowws = true, const std::vector<glm::mat4>& boneTransforms = {});
		void SubmitDynamicMesh(Count<DynamicMesh> mesh, Count<RenderMaterial> renderMaterial, uint32_t subMeshIndex, const glm::mat4& transform, bool CastShadowws = true, const std::vector<glm::mat4>& boneTransforms = {});

		void SubmitPhysicsDebugMesh(Count<Mesh> mesh, const glm::mat4& transform);
		void SubmitPhysicsDynamicDebugMesh(Count<DynamicMesh> mesh, uint32_t subMeshIndex, const glm::mat4& transform);
		// if the same size is passed it will not resize
		void SetViewportSize(uint32_t width, uint32_t height);

		// grass plane funciton does not chagne transform to mathc localpositon of grass plane
		// have to do that manualy
		void SubmitGrassPlane(Count<class GrassBladePlane> plane, const glm::mat4& transform);
		Count<Image2D> GetFinalPassImage();
		Count<Image2D> GetShadowPassDebugImage();

		//external can use this to drw to
		Count<FrameBuffer> GetExternalCompositePassFrameBuffer() { return m_ExternalCompositeFrameBuffer; }
		Count<class Renderer2D> GetRenderer2D() { return m_Renderer2D; }
		const WorldRendererStatistics& GetStats() const
		{
			return m_Stats;
		}
		UBScreenData GetScreenData()const { return m_UBScreenData; }
		Count<class RenderCommandBuffer> GetCommandBuffer() { return m_CommandBuffer; };
		const UBCameraData& GetCameraUBData()const
		{
			return m_UBCameraData;
		}
		Count<class GlobalBufferSet> GetGlobalInputs() { return m_GlobalInputs; }
		Count<RenderPass> GetGeometryPass() { return m_GeometryPass; }
		const RendererCustomTypeSet<std::unordered_map<UUID, std::pair<Count<class GrassBladePlane>, glm::mat4>>>& GetGrassPlanes()const
		{
			return m_GrassPlanes;
		}

		Count <RenderPass> GetPreDepthRenderPass() { return m_PreDepthPass; }

		void SubmitParticleEmitter(Count<class ParticleEmitterInstance> emiter);

		void SubmitPostProcessMaterial(Count<class Material> material);
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

		std::vector<Count<class Material>> m_PostProcessMaterials;

		RendererCustomTypeSet<std::unordered_map<UUID, std::pair<Count<class GrassBladePlane>, glm::mat4>>> m_GrassPlanes;
		Count<class World> m_ActiveWorld;
		//buffer data
		UBFrameData m_UBFrameData;
		UBRenderData m_UBRenderData;
		UBSceneData m_UBSceneData;
		UBScreenData m_UBScreenData;
		UBCameraData m_UBCameraData;
		UBLightScene m_UBLightData;
		UBHBAOData m_HBAOData;
		UBSSR m_UBSSR;

		//buffer sts
		Count<UniformBufferSet> m_UBFrameBuffer;
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
		Count<class Renderer2D>  m_Renderer2D;
		Count<class RenderCommandBuffer> m_CommandBuffer;

		Count<class StorageBufferSet> m_SBDirectionalLights;

		std::map<MeshKey, TransformMapData> m_MeshTransformMap[2];
		std::map<MeshKey, TransformMapData>* m_CurTransformMap;
		std::map<MeshKey, TransformMapData>* m_PrevTransformMap;

		uint32_t m_TotalSubmeshesContext = 0;

		std::map<MeshKey, MeshDrawInfo> m_MeshDrawList;
		std::map<MeshKey, DynamicMeshDrawInfo> m_DynamicMeshDrawList;

		std::map<MeshKey, MeshDrawInfo> m_MeshShadowDrawList;
		std::map<MeshKey, DynamicMeshDrawInfo> m_DynamicMeshShadowDrawList;

		using BoneTransforms = std::array<glm::mat4, 100>; // Note: 100 == MAX_BONES from the shaders

		struct BoneTransformsMapData
		{
			std::vector<BoneTransforms> BoneTransformsData;
			uint32_t BoneTransformsBaseIndex = 0;
		};

		Count<class StorageBufferSet> m_BoneTransformStorageBuffersSet; 
		BoneTransforms* m_BoneTransformsData = nullptr;

		std::map<MeshKey, BoneTransformsMapData> m_MeshBoneTransformsMap;

		//debg 
		std::map<MeshKey, MeshDrawInfo> m_ColliderDrawList;
		std::map<MeshKey, DynamicMeshDrawInfo> m_DynamicColliderDrawList;

		std::map<MeshKey, MeshDrawInfo> m_TransparentMeshDrawList;

		std::unordered_map<Count<class Shader>,std::map<MeshKey, MeshDrawInfo>> m_GeometryPassInstancesDrawList; // shaderName
		std::unordered_map<Count<class Shader>,std::map<MeshKey, DynamicMeshDrawInfo>> m_GeometryPassDynamicMeshInstancesDrawList; // shaderName

		Count<class Environment> m_Environment;
		bool m_InContext = false;
		uint32_t m_ShadowMapResolution;
		// geometry pass
		Count<RenderPass> m_GeometryPass;
		Count<RenderPass> m_GeometryAnimPass;
		Count<RenderPass> m_TransparentGeometryPass;
		Count<RenderPass> m_TransparentPassComposite;

		struct GeometryInstanceRenderData
		{
			Count<class RenderPass> RenderPass;
			enum class DepthDrawType
			{
				None = 0,
				PreDepth = 1, // render depth
				OverrideDepthNecessary = 2, // render depth but same tim drawign col
			};

			bool TransperantPass = false;
			DepthDrawType DepthDraw = DepthDrawType::PreDepth;
		};
		// shader name, (render pass, drawing with dpeth buffer)
		std::unordered_map<Count<class Shader>, GeometryInstanceRenderData> m_GeometryPassInstances; // the int is how rendred depth
		Count<FrameBuffer> m_GeometryPassNoDepthFrameBuffer;
		// pre pass
		Count<RenderPass> m_PreDepthPass;
		Count<RenderPass> m_PreDepthTransparentPass;
		Count<class Image2D> m_PrevDepthImage;
		Count<class Image2D> m_PrevNormalImage;
		// debug
		Count<RenderPass> m_GeometryWireFramePass;
		Count<RenderPass> m_GeometryWireFrameOnTopPass;
		Count<class Material> m_GeometryWireFramePassMaterialAsset;
		Count<RenderMaterial> m_GeometryWireFramePassMaterial;// points to the render material of m_GeometryWireFramePassMaterialAsset

		// compoiste pass
		Count<RenderPass> m_SkyBoxPass;
		Count<RenderPass> m_CompositePass;

		Count<FrameBuffer> m_PostProcessFrameBufferA;
		Count<FrameBuffer> m_PostProcessFrameBufferB;
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
		Count<StorageBufferSet> m_SBVisibleDirectionalLightIndicesBuffer;

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
			Count<ComputePass> HierarchalDepthDownSamplerPass;
			Count<ComputePass> TileClassification;
			Count<ComputePass> BlueNoisePass;
			Count<ComputePass> IntersectArgs;
			Count<ComputePass> Intersect;
			Count<ComputePass> Reproject;
			Count<ComputePass> Prefilter;
			Count<ComputePass> Temporal;
			Count<RenderPass> ApplyPass;

			Count<StorageBufferSet> SBRayCounter;
			Count<StorageBufferSet> SBRayList;
			Count<StorageBufferSet> SBDenoiseList;
			Count<StorageBufferSet> SBIntersectCommand;
			Count<StorageBufferSet> SBDenoiseCommand;
			Count<StorageBufferSet> SBGlobalAtomic;

			Count<Image2D> Variance;
			Count<Image2D> PreviousVariance;

			Count<Image2D> Radiance;
			Count<Image2D> PreviousRadiance;

			Count<Image2D> ExtractRoughness;
			Count<Image2D> PreviousExtractRoughness;

			Count<Image2D> SampleCount;
			Count<Image2D> PreviousSampleCount;

			Count<Image2D> ReprojectImage;
			Count<Image2D> PreviousReprojectImage;

			Count<Image2D> AverageRadiance;
			Count<Image2D> BlueNoiseImage;
			Count<Texture2D> HierarchalDepthDownSamplerTexture;
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

		struct HizSSR
		{
			Count<ComputePass> SSSRPass;
			Count<Image2D> OutputImage;
		}m_HizSSR;
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
		bool m_InitalRanShadow = false;//idk why but hopfeully works

		Count<class GrassRenderer> m_GrassRenderer;

		Count<ComputePass> m_ParticleSpawnComputePass;
		Count<ComputePass> m_ParticleUpdateComputePass;
		Count<RenderPass> m_ParticleRenderPass;

		std::unordered_set<Count<class ParticleEmitterInstance>> m_Emitters;

		std::unordered_map<Count<class Shader>, Count<class RenderPass>> m_PostProcessPasses;
	private:

		// only put attach to depth when you are sure u are not changing any vertex position
		// if you change teh vertex positon and this is true u are going to see wierd things happen
		// as the depth buffer drawing is different so we do not want this drawn onto depth buffer for things like water

		GeometryInstanceRenderData CreateGeometryInstanceRenderData(Count<class Shader> shader);
		Count<RenderPass> CreateGeometryPassInstance(Count<class Shader>shader, GeometryInstanceRenderData::DepthDrawType type);
		Count<RenderPass> CreateTransparentPassInstance(Count<class Shader>shader, bool drawWithDepth);
		void Init();
		void CalculateCascades(CascadeData* cascades, const glm::vec3& lightDirection);
		void CalculateCascadesManualSplit(CascadeData* cascades, const glm::vec3& lightDirection);

		// set up all the passes for current render
		void SetPasses();
		void ShadowPass();
		void PreDepthPass();
		void GeometryPass();
		void LightFrustrumAndCullingPass();
		void RenderPostProcessingPasses();
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
		void HizSSRPass();
		void DrawScene();
		void ClearPass(Count<RenderPass> renderPass, bool explicitClear);
		void RenderParticleSystem();

		void CreatePostProcessRenderPass(Count<class RenderMaterial> material);
		// tehse are static so basically when wer are writng code we avoid errors of 
		// writing code to a speicif world rendere class

		static void RenderMesh(Count<RenderCommandBuffer>& commandBuffer, Count<Mesh>& mesh, Count<RenderPass>& renderPass, Count<VertexBuffer>& transformBuffer, uint32_t subMeshIndex, uint32_t transformOffset, uint32_t instanceCount, const Buffer& pushData = Buffer(), const std::string& pushName = "");
		static void RenderDynamicMesh(Count<RenderCommandBuffer>& commandBuffer, Count<DynamicMesh>& mesh, Count<RenderPass>& renderPass, Count<VertexBuffer>& transformBuffer, uint32_t subMeshIndex, uint32_t transformOffset, uint32_t instanceCount, const Buffer& pushData = Buffer(), const std::string& pushName = "");

		static void RenderMeshWithMaterial(Count<RenderCommandBuffer>& commandBuffer, Count<Mesh>& mesh, Count<RenderMaterial>& material, Count<RenderPass>& renderPass, Count<VertexBuffer>& transformBuffer, uint32_t subMeshIndex, uint32_t transformOffset, uint32_t instanceCount);
		static void RenderDynamicMeshWithMaterial(Count<RenderCommandBuffer>& commandBuffer, Count<DynamicMesh>& mesh, Count<RenderMaterial>& material, Count<RenderPass>& renderPass, Count<VertexBuffer>& transformBuffer, uint32_t subMeshIndex, uint32_t transformOffset, uint32_t instanceCount);

		static void RenderMeshWithMaterialTable(Count<RenderCommandBuffer>& commandBuffer, Count<Mesh>& mesh, Count<MaterialTable>& materialTable, Count<RenderPass>& renderPass, Count<VertexBuffer>& transformBuffer, uint32_t subMeshIndex, uint32_t transformOffset, uint32_t instanceCount);
		static void RenderDynamicMeshWithMaterialTable(Count<RenderCommandBuffer>& commandBuffer, Count<DynamicMesh>& mesh, Count<MaterialTable>& materialTable, Count<RenderPass>& renderPass, Count<VertexBuffer>& transformBuffer, uint32_t subMeshIndex, uint32_t transformOffset, uint32_t instanceCount);
		static void RenderDynamicMeshWithMaterialTable(Count<RenderCommandBuffer>& commandBuffer, Count<DynamicMesh>& mesh, Count<MaterialTable>& materialTable, Count<RenderPass>& renderPass, Count<VertexBuffer>& transformBuffer, uint32_t subMeshIndex, uint32_t transformOffset, uint32_t instanceCount, uint32_t boneTransformsOffset,Count<class StorageBuffer> boneTransformStorageBuffers);
		
		
		void CopyToBoneTransformStorage(const MeshKey& meshKey, Count<class MeshSource> meshSource, const std::vector<glm::mat4>& boneTransforms);
		friend class Editore3D;
		friend class WorldRendererPanel;
		friend class ViewPortEditorWorkspace;
		friend class World;
	};
}




