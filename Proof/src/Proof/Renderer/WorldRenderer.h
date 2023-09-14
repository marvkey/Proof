#pragma once
#include "FrameBuffer.h"
#include "Proof/Core/Buffer.h"
#include "Proof/Scene/Material.h"
#include "Proof/Scene/Camera/Camera.h"
#include "Viewport.h"

#include <map>
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

			if (MaterialTable != nullptr)
			{
				if (MaterialTable->GetMaterialCount() < other.MaterialTable->GetMaterialCount())
					return true;
				if (MaterialTable->GetMaterialCount() > other.MaterialTable->GetMaterialCount())
					return false;
			}
			

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
			if (MaterialTable != nullptr)
			{
				// this here because it would do an operotr over ach materials in the table 
			// a little expensive if the material tabel has oaver liek a 1000 eleemtns wich prolly will never happen
				if (*MaterialTable < *other.MaterialTable)
					return true;
				if (*MaterialTable > *other.MaterialTable)
					return false;
			}

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
		Vector Color = { 1 };
		float Intensity = 0.0f;
		Vector Direction; // Make sure TO NORMALIZED

		int bCastShadows = 1;//bool
		int bCastSoftShadows = 1;// bool
		float ShadowStrength = 0.0f; // between 0 and 1 or shadow amount how dark the shadow is
		float ShadowSoftness = 0.0f; // penubra size or light size between 0 and 1
	};
	
	struct UBSkyLight 
	{
		Vector TintColor = { 1 };
		float Lod = 0;
		//float Intensity = 1;
		float Intensity = 0;
		float Rotation = 0;
	};
	
	struct PointLight
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
	struct SpotLight
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
	};

	struct WorldRendererStatistics
	{
		WorldRendererTimers Timers;
		UBLightScene LightSene;
	};
	class WorldRenderer : public RefCounted {
	public:
		WorldRenderer();

		virtual ~WorldRenderer();
		ShadowSetting ShadowSetting;
		WorldRendererOptions Options;

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
		void SubmitStaticMesh(Count<Mesh> mesh, Count<MaterialTable> materialTable, const glm::mat4& trnasform, bool CastShadowws = true);

		void SubmitPhysicsDebugMesh(Count<Mesh> mesh, const glm::mat4& transform);
		// if the same size is passed it will not resize
		void SetViewportSize(uint32_t width, uint32_t height);

		Count<Image2D> GetFinalPassImage();
		Count<Image2D> GetShadowPassDebugImage();

		const WorldRendererStatistics& GetStats() const
		{
			return m_Stats;
		}
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
		Special<class Renderer2D>  m_Renderer2D;
		Special<class Renderer2D>  m_UIRenderer;
		Special<class Renderer2D>  m_ParticleSystemRenderer;
		Count<class RenderCommandBuffer> m_CommandBuffer;

		Count<class Texture2D> m_BRDFLUT;

		Count<class StorageBufferSet> m_SBDirectionalLights;

		std::map<MeshKey, TransformMapData> m_MeshTransformMap;
		std::map<MeshKey, MeshDrawInfo> m_MeshDrawList;
		std::map<MeshKey, MeshDrawInfo> m_MeshShadowDrawList;

		//debg 
		std::map<MeshKey, MeshDrawInfo> m_ColliderDrawList;
		Count<class Environment> m_Environment;
		bool m_InContext = false;
		uint32_t m_ShadowMapResolution;
		// geometry pass
		Count<RenderPass> m_GeometryPass;

		// pre pass
		Count<RenderPass> m_PreDepthPass;
		Count<RenderPass> m_SkyBoxPass;
		Count<RenderPass> m_CompositePass;
		Count<FrameBuffer> m_ExternalCompositeFrameBuffer;

		// debug
		Count<RenderPass> m_GeometryWireFramePass;
		Count<RenderPass> m_GeometryWireFrameOnTopPass;
		Count<RenderMaterial> m_GeometryWireFramePassMaterial;
		// compoiste pass
		Count<RenderMaterial> m_CompositeMaterial;
		bool m_NeedResize = false;

		DirectionalLight m_MainDirectionllLight;

		// stats make sure this is first because variables gets initilized in class form top to bottom
		WorldRendererStatistics m_Stats;

		WorldRendererTimers& m_Timers;
		UBLightScene& m_LightScene;

		// fowardplus
		Count<StorageBufferSet> m_FrustrumsBuffer;
		Count<class ComputePass> m_FrustrumPass;
		Count<class ComputePass> m_LightCullingPass;
		Count<Image2D> m_PointLightGrid;
		Count<Image2D> m_SpotLightGrid;

		Count<StorageBufferSet> m_PointLightIndexCounterBuffer;
		Count<StorageBufferSet> m_SpotLightIndexCounterBuffer;

		Count<StorageBufferSet> m_PointLightIndexListBuffer;
		Count<StorageBufferSet> m_SpotLightIndexListBuffer;
		glm::uvec3 m_LightCullingWorkGroups;
		glm::uvec3 m_LightCullingNumThreads;
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


		void DrawScene();
		// tehse are static so basically when wer are writng code we avoid errors of 
		// writing code to a speicif world rendere class
		static void RenderMesh(Count<RenderCommandBuffer>& commandBuffer, Count<Mesh>& mesh, Count<RenderPass>& renderPass, Count<VertexBuffer>& transformBuffer, uint32_t transformOffset, uint32_t instanceCount, const Buffer& pushData = Buffer(), const std::string& pushName ="");
		static void RenderMeshWithMaterial(Count<RenderCommandBuffer>& commandBuffer, Count<Mesh>& mesh, Count<RenderMaterial>& material, Count<RenderPass>& renderPass, Count<VertexBuffer>& transformBuffer, uint32_t transformOffset, uint32_t instanceCount);
		static void RenderMeshWithMaterialTable(Count<RenderCommandBuffer>& commandBuffer,Count<Mesh>&mesh, Count<RenderPass>& renderPass, Count<MaterialTable>& materialTable, Count<VertexBuffer>& transformBuffer, uint32_t transformOffset, uint32_t instanceCount);
		friend class Editore3D;

	};	
}


