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
#include "../Vertex.h"
namespace Proof
{
	//using uint64_t = uint64_t;
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
		std::unordered_map < uint64_t, std::vector< MeshPipeLine::MeshVertex>> MeshesTransforms;
		std::unordered_map < uint64_t, MeshInstance> Meshes;
		uint32_t NumberMeshes;
		// order teh meshes are pushed intehvector that stores all transforms
		std::vector<uint64_t> ElementsImplaced;

		MeshPipeLine(Count<RenderPass> renderPass);
		// the begin offset we should start rendering
		uint32_t OffsetBegin = 0;
		LightPass LightPass;
		void Reset() {
			MeshesTransforms.clear();
			Meshes.clear();
			LightPass.DirLights.clear();
			ElementsImplaced.clear();
			NumberMeshes = 0;
			OffsetBegin = 0;
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
		// order teh meshes are pushed intehvector that stores all transforms
		std::vector<uint64_t> ElementsImplaced;
		// the begin offset we should start rendering
		uint32_t OffsetBegin = 0;
		std::unordered_map < uint64_t, std::vector< MeshPipeLine::MeshVertex>> MeshesTransforms;
		std::unordered_map < uint64_t, MeshInstance> Meshes;
		std::unordered_map < uint64_t, MaterialData> MaterialDatas;
		std::unordered_map<DescriptorSets, Count<DescriptorSet>> Descriptors;
		void Reset() {
			MeshesTransforms.clear();
			ElementsImplaced.clear();
			Meshes.clear();
			MaterialDatas.clear();
			NumberMeshes = 0;
			OffsetBegin = 0;
		}
	
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
		void SubmitMesh(Count<Mesh> mesh, const glm::mat4& transform);
		void SubmitMeshWithMaterial(Count<Mesh> mesh, Count<Material> material, const glm::mat4& transform);
		void SubmitDirectionalLight(const DirLight& light);
		void SubmitPointLight(class PointLightComponent& comp, class TransformComponent& transform);
		void SubmitSpotLight(class SpotLightComponent& comp, class TransformComponent& transform);
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
		void Reset();
		bool s_InContext = false;
		Count <class RenderPass > m_RenderPass;
	};
}
