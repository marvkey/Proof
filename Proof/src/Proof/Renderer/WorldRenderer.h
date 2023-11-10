#pragma once
#include "FrameBuffer.h"
#include "Proof/Core/Buffer.h"
#include "Proof/Scene/Camera/Camera.h"
#include "Viewport.h"
#include "Proof/Asset/AssetTypes.h"
#include <map>
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
		glm::vec3 Color = glm::vec3{ 1 };
		float Intensity = 0.0f;
		glm::vec3 Direction; // Make sure TO NORMALIZED

		int bCastShadows = 0;//bool
		int bCastSoftShadows = 0;// bool
		float ShadowStrength = 0.0f; // between 0 and 1 or shadow amount how dark the shadow is
		float ShadowSoftness = 0.0f; // penubra size or light size between 0 and 1
	};
	
	struct UBSkyLight 
	{
		glm::vec3 TintColor = glm::vec3{ 1 };
		float Lod = 0;
		//float Intensity = 1;
		float Intensity = 0;
		float Rotation = 0;
	};
	
	struct alignas(16) PointLight // this is what vulkan uses 16 bytes alignemnt
	{
		glm::vec3 Location{ 0 };
		float Intensity = 1; // min =0, max = 500f;
		glm::vec3 Color{ 1 };
		float MinRadius = 1; // min  = 0,maximum should be Radius
		float Radius = 10;// min is 0, max max float
		float Falloff = 1.0f; //min is 0, represent how far before the light becomes weaker
		int bCastsShadows;
		int bSoftShadows;
		float ShadowStrength = 0.5f;// 0.0 to 1.0 how dark sahdow is
		float ShadowSoftness = 0.5f;//how soft the shadow is from 0.0 to 1.0f
	};
	struct alignas(16) SpotLight
	{
		glm::vec3 Location{ 0 };
		float Intensity = 1.0f; // Range: 0.0 to positive infinity.
		glm::vec3 Direction{ 0 }; //NORMALIZE It
		// Attenuation factor for the spotlight cone's light intensity
		// as the angle between the light direction and surface normal increases.
		// A higher value results in a more rapid decrease in intensity
		// as the angle deviates from the spotlight's central direction.
		float AngleAttenuation = 5.0f; // Range: 0.0 to positive infinity.
		glm::vec3 Color{ 1.0f };
		float Range = 10.0f;// Range: 0.0 to positive infinity.
		float Angle = 60.0f;// Range: 0.0 to 180.0 degrees. // Angle of the spot light cone (in degrees).
		float Falloff = 1.0f;// Range: 0.0 to positive infinity.  factor that affects how the light intensity diminishes.
		int bCastsShadows = false;
		int bSoftShadows = false;
		float ShadowStrength = 0.5f;// 0.0 to 1.0 how dark sahdow is
		float ShadowSoftness = 0.5f;//how soft the shadow is from 0.0 to 1.0f 
	};
	struct SBDirectionalLightsSceneData
	{
		std::vector<DirectionalLight> DirectionalLights;
	};
	struct SBPointLightSceneData
	{
		std::vector<PointLight> PointLights;
	};
	struct SBSpotLightSceneData
	{
		std::vector<SpotLight> SpotLights;
	};
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

	struct WorldRendererOptions
	{
		enum class PhysicsColliderView
		{
			None = 0, Normal = 1, OnTop = 2
		};
		PhysicsColliderView ShowPhysicsColliders = PhysicsColliderView::None;
		glm::vec4 PhysicsColliderColor = glm::vec4{ 0.2f, 1.0f, 0.2f, 1.0f };
		bool ShowLightGrid = false;
	};
	enum class ShadowResolution
	{
		None = 0,
		Low,
		Mediuim,
		High, 
		Extreme
	};
	struct ShadowSetting
	{
		//casde fading
		bool CascadeFading = true;
		float CascadeTransitionFade = 1.0f; // min 0, max (Float Max)

		float CascadeSplitLambda = 0.92f; // (min 0.0 max 1.0)
		float CascadeNearPlaneOffset = -50.0f; // min - Floatmax, max 0
		float CascadeFarPlaneOffset = 50.0f; // min 0, max Float Max
		float ScaleShadowCascadesToOrigin = 0.f; //min 0.0f max 1.0f
		float MaxShadowDistance = 200.f; // how far away the shadow can be seen
		float ShadowFade = 50.0f; // the distnace the shader goes from opaque to transperant

		//use Manual cascadeSPlits
		bool UseManualCascadeSplits = false;
		float CascadeSplits[4] = { 0.1,0.2,0.3,1.0 }; // min 0, max Flt max

		bool ShowCascades =false;
		bool SoftShadows = false; // temporarily set to false for performance reasons
		ShadowResolution  ShadowResolution = ShadowResolution::Mediuim;

		bool RenderDebugPass = false;
		int DebugCascade = 0; // min 0 max 3
	};
	
	struct UBCameraData
	{
		glm::mat4 Projection;
		glm::mat4 InverseProjection;
		glm::mat4 UnreversedProjectionMatrix;
		glm::mat4 View;
		glm::mat4 InverseView;
		glm::mat4 ViewProjection;// projection * view
		glm::mat4 InverseViewProjection; 

		Vector Position;
		float NearPlane;
		float FarPlane;
	};
	struct UBRenderData
	{
		glm::vec4 cascadeSplit;
		int bShowCascades = false; // bools in glsl are 4 ytes
		int bSoftShadows =(int)true;
		float MaxShadowDistance = 200.f;
		float ShadowFade = 1.0f;
		int bCascadeFading = true; 
		float CascadeTransitionFade = 1.0f;
	};

	struct UBSceneData
	{
		glm::vec3 CameraPosition;
		int bShowLightGrid = (int)false;
	};
	struct UBScreenData
	{
		glm::vec2 FullResolution;
		glm::vec2 InverseFullResolution;
		glm::vec2 HalfResolution;
		glm::vec2 InverseHalfResolution;
		float AspectRatio;
		float ResolutionScaling = 1.0f;
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

	struct UBLightScene
	{
		uint32_t SkyLightCount = 0;
		uint32_t DirectionalLightCount = 0;
		uint32_t PointLightCount = 0;
		uint32_t SpotLightCount= 0;
		glm::uvec3 LightCullingWorkGroups;
	};

	struct WorldRendererStatistics
	{
		WorldRendererTimers Timers;
		UBLightScene LightSene;
	};
	struct AmbientOcclusion
	{
		bool Enabled = true;
		enum class AmbientOcclusionType
		{
			None = 0,
			SSAO, // Scren space Ambient Occlusion
			HBAO, // High 
			GTAO //Ground Truth Ambient Occlusion
		};

		AmbientOcclusionType Type = AmbientOcclusionType::SSAO;

		struct SSAOSettings
		{
			float Radius = 0.5f;
			float Bias = 0.25f;
			int KernelSize = 64;
			int NoiseSize = 4;
		};

		SSAOSettings SSAO;
	};

	struct BloomSettings
	{
		bool Enabled = true;
		float Threshold = 1.0f;
		float Knee = 0.1f;
		float UpsampleScale = 1.0f;
		float Intensity = 1.0f;
		float DirtIntensity = 1.0f;
	};

	class RenderPass;
	class ComputePass;
	class WorldRenderer : public RefCounted {
	public:
		WorldRenderer();

		virtual ~WorldRenderer();
		ShadowSetting ShadowSetting;
		WorldRendererOptions Options;
		AmbientOcclusion AmbientOcclusion;
		BloomSettings m_BloomSettings;
	public:

		void SetContext(Count<class World> world);
		// make sure all settings for the renderer are set before callign this fucntion
		// like make sure shadows are set to teh write shadow quality
		void BeginScene(const Camera& camera, const Vector& location, float nearPlane, float farPlane);
		void EndScene();

		void SubmitSkyLight(const UBSkyLight& skyLight, Count<class Environment> environment);
		void SubmitDirectionalLight(const SBDirectionalLightsSceneData& directionaLights);
		void SubmitPointLight(const SBPointLightSceneData& pointLights);
		void SubmitSpotLight(const SBSpotLightSceneData& spotLights);
		void SubmitMesh(Count<Mesh> mesh, Count<MaterialTable> materialTable, const glm::mat4& transform, bool CastShadowws = true);
		void SubmitDynamicMesh(Count<DynamicMesh> mesh, Count<MaterialTable> materialTable, uint32_t subMeshIndex,const glm::mat4& transform, bool CastShadowws = true);

		void SubmitPhysicsDebugMesh(Count<Mesh> mesh, const glm::mat4& transform);
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
		Count<class Texture2D> m_BRDFLUT;

	private:
		Count<class World> m_ActiveWorld;
		//buffer data
		UBRenderData m_UBRenderData;
		UBSceneData m_UBSceneData;
		UBScreenData m_UBScreenData;
		UBCameraData m_UBCameraData;
		UBLightScene m_UBLightData;

		//buffer sts
		Count<UniformBufferSet> m_UBRenderDataBuffer;
		Count<UniformBufferSet> m_UBSceneDataBuffer;
		Count<UniformBufferSet> m_UBCameraBuffer;
		Count<UniformBufferSet> m_UBSKyBoxBuffer;
		Count<UniformBufferSet>m_UBCascadeProjectionBuffer;
		Count<UniformBufferSet> m_UBScreenBuffer;
		Count<UniformBufferSet> m_UBLightSceneBuffer;

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

		std::map<MeshKey, TransformMapData> m_MeshTransformMap;

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

		//bloom
		const uint32_t m_BloomComputeWorkgroupSize = 4;
		Count<ComputePass> m_BloomComputePass;
		Count<Texture2D> m_BloomDirtTexture;
		std::vector<Count<Texture2D>> m_BloomComputeTextures{ 3 };
		std::vector<std::vector<Count<ImageView>>> m_BloomComputeImageViews {3};

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

		//post processing passes
		void AmbientOcclusionPass();
		void BloomPass();

		void DrawScene();
		// tehse are static so basically when wer are writng code we avoid errors of 
		// writing code to a speicif world rendere class

		static void RenderMesh(Count<RenderCommandBuffer>& commandBuffer, Count<Mesh>& mesh, Count<RenderPass>& renderPass, Count<VertexBuffer>& transformBuffer, uint32_t subMeshIndex, uint32_t transformOffset,  uint32_t instanceCount, const Buffer& pushData = Buffer(), const std::string& pushName = "");
		static void RenderDynamicMesh(Count<RenderCommandBuffer>& commandBuffer, Count<DynamicMesh>& mesh, Count<RenderPass>& renderPass, Count<VertexBuffer>& transformBuffer, uint32_t subMeshIndex, uint32_t transformOffset,  uint32_t instanceCount, const Buffer& pushData = Buffer(), const std::string& pushName ="");

		static void RenderMeshWithMaterial(Count<RenderCommandBuffer>& commandBuffer, Count<Mesh>& mesh, Count<RenderMaterial>& material, Count<RenderPass>& renderPass, Count<VertexBuffer>& transformBuffer, uint32_t subMeshIndex, uint32_t transformOffset, uint32_t instanceCount);
		static void RenderDynamicMeshWithMaterial(Count<RenderCommandBuffer>& commandBuffer, Count<DynamicMesh>& mesh, Count<RenderMaterial>& material, Count<RenderPass>& renderPass, Count<VertexBuffer>& transformBuffer, uint32_t subMeshIndex,uint32_t transformOffset, uint32_t instanceCount);
		
		static void RenderMeshWithMaterialTable(Count<RenderCommandBuffer>& commandBuffer,Count<Mesh>&mesh, Count<MaterialTable>& materialTable, Count<RenderPass>& renderPass , Count<VertexBuffer>& transformBuffer, uint32_t subMeshIndex, uint32_t transformOffset, uint32_t instanceCount);
		static void RenderDynamicMeshWithMaterialTable(Count<RenderCommandBuffer>& commandBuffer, Count<DynamicMesh>& mesh, Count<MaterialTable>& materialTable, Count<RenderPass>& renderPass, Count<VertexBuffer>& transformBuffer, uint32_t subMeshIndex, uint32_t transformOffset, uint32_t instanceCount);
		friend class Editore3D;

	};	
}


