#pragma once
#include <unordered_map>
#include <array>
#include <vector>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "Proof/Scene/Material.h"
#include "Proof/Renderer/RendererAPI.h"
#include "Proof/Renderer/FrameBuffer.h"
#include "Proof/Renderer/CommandBuffer.h"
#include "Proof/Renderer/UniformBuffer.h"
#include "Proof/Scene/Component.h"

namespace Proof
{
	struct DirLight {
		Vector Color = { 1 };
		float Intensity = 1.0f;// has not been implemented
		Vector Direction;
	};
	struct LightPass {
		Count<class UniformBuffer> DirLightsBuffer;
		std::vector<DirLight> DirLights;
	};
	struct MeshPipeLine {
		struct MeshVertex {

			MeshVertex(const glm::mat4& transform) :
				m_Transform(transform) {
			}
			glm::mat4 m_Transform;
		};
		Count<class GraphicsPipeline> GraphicsPipeline;
		Count<class Shader> Shader;
		Count <class PipeLineLayout> PipeLineLayout;
		Count<VertexBuffer> MeshesVertexBuffer;
		std::unordered_map<DescriptorSets, Count<DescriptorSet>> Descriptors;

		// id, and all teh transforms for that mesh
		std::unordered_map < AssetID, std::vector< MeshPipeLine::MeshVertex>> MeshesTransforms;
		std::unordered_map < AssetID, uint32_t> AmountMeshes;
		uint32_t NumberMeshes;
		std::vector<AssetID> ElementsImplaced;

		MeshPipeLine(Count<RenderPass> renderPass);
		// the begin offset we should start rendering
		uint32_t OffsetBegin = 0;
		LightPass LightPass;
		void Reset() {
			MeshesTransforms.clear();
			AmountMeshes.clear();
			LightPass.DirLights.clear();
			ElementsImplaced.clear();
			NumberMeshes = 0;
		}
	};
	struct MaterialData {
		Vector Colour{ 1,1,1 };
		float Metallness = 0.0f;// also shinines
		float Roughness = 0.0f;
	};
	// takes from mesh pipline
	struct MeshMaterialPipeline {
	public:
		// going to use mesh pipline buffers
		Count<class GraphicsPipeline> GraphicsPipeline;
		Count<class Shader> Shader;
		Count <class PipeLineLayout> PipeLineLayout;
		MeshMaterialPipeline(Count<RenderPass> renderPass);
		Count<class PushConstant> MaterialPushConstant;
		uint32_t NumberMeshes;
		std::vector<AssetID> ElementsImplaced;
		// the begin offset we should start rendering
		uint32_t OffsetBegin = 0;
		std::unordered_map < AssetID, std::vector< MeshPipeLine::MeshVertex>> MeshesTransforms;
		std::unordered_map < AssetID, uint32_t> AmountMeshes;
		std::unordered_map < AssetID, MaterialData> MaterialDatas;
		std::unordered_map<DescriptorSets, Count<DescriptorSet>> Descriptors;
		void Reset() {
			MeshesTransforms.clear();
			ElementsImplaced.clear();
			AmountMeshes.clear();
			MaterialDatas.clear();
			NumberMeshes = 0;
		}
	
	};
	struct DebugMeshPipeLine {
		Count<class GraphicsPipeline> GraphicsPipeline;
		Count<class Shader> Shader;
		Count <class PipeLineLayout> PipeLineLayout;
		Count <class RenderPass > RenderPass;
		std::unordered_map<DescriptorSets, Count<DescriptorSet>> Descriptors;

		DebugMeshPipeLine();
	};
	
	struct RenderStorage {
		Count<RenderCommandBuffer> CommandBuffer;
		Count<UniformBuffer> CameraBuffer = nullptr;
		Count<ScreenFrameBuffer> CurrentFrameBuffer = nullptr;
	};

	class Renderer3DPBR {
	public:
		Renderer3DPBR(Count<RenderPass> renderPass);
		~Renderer3DPBR() {

		}
		void Init();
		void BeginContext(class EditorCamera& editorCamera, Count<ScreenFrameBuffer>& frameBuffer,Count<RenderCommandBuffer>& commandBuffer);
		void BeginContext(const glm::mat4& projection, const glm::mat4& view, const Vector& Position, Count<ScreenFrameBuffer>& frameBuffer, Count<RenderCommandBuffer>& commandBuffer);
		void SubmitMesh(class MeshComponent& meshComponent, const glm::mat4& transform);
		void SubmitDirectionalLight(const DirLight& light);
		void SubmitPointLight(class PointLightComponent& comp, class TransformComponent& transform);
		void SubmitSpotLight(class SpotLightComponent& comp, class TransformComponent& transform);
		void DrawDebugMesh(class Mesh* mesh, const glm::mat4& transform);
		void EndContext();
		void Destroy();
	private:
		void SetPasses();
		void SetMeshPass();
		void MeshPass();
		void RenderMesh();
		void RenderMeshMaterial();
		// these 2 piplines work togethere
		Special<MeshPipeLine> m_MeshPipeLine;
		Special< MeshMaterialPipeline> m_MeshMaterialPipeline;
		Special<RenderStorage> m_RenderStorage;
		void DrawContext();
		void DrawMeshSource(uint64_t id,uint64_t num,uint64_t offset);
		void Reset();
		bool s_InContext = false;
		Count <class RenderPass > m_RenderPass;
	};
}
