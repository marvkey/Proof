#pragma once
#include "Proof/Core/Core.h"
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

	struct DirLight {
		Vector Color = { 1 };
		float Intensity = 1.0f;// has not been implemented
		Vector Direction;
	};
	struct LightPass {
		Count<class UniformBuffer> DirLightsBuffer;
		std::vector<DirLight> DirLights;
	};
	struct MaterialTableData {
	public:
		std::vector<UUID> MaterialIndexes;
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
		Count<class VertexBuffer> MeshesVertexBuffer;
		std::unordered_map<DescriptorSets, Count<DescriptorSet>> Descriptors;

		// id, and all teh transforms for that mesh
		std::unordered_map < UUID, std::vector< MeshPipeLine::MeshVertex>> MeshesTransforms;
		std::unordered_map < UUID, MeshInstance> Meshes;
		uint32_t NumberMeshes;
		// order teh meshes are pushed intehvector that stores all transforms
		std::vector<uint64_t> ElementsImplaced;

		MeshPipeLine(Count<RenderPass> renderPass);
		// the begin offset we should start rendering
		uint32_t OffsetBegin = 0;
		LightPass LightPass;
		Count<class CubeMap> IrradianceMap;
		Count<class CubeMap> PrefilterMap;
		Count<Texture2D> BRDf;
		void Reset() {
			MeshesTransforms.clear();
			Meshes.clear();
			LightPass.DirLights.clear();
			ElementsImplaced.clear();
			NumberMeshes = 0;
			OffsetBegin = 0;
			IrradianceMap = nullptr;
			PrefilterMap = nullptr;
			BRDf = nullptr;
		}

	};
	struct MaterialData {
		Vector Colour{ 1,1,1 };
		float Metallness = 0.0f;// also shinines
		float Roughness = 0.0f;
	};


	struct MaterialTextureData {
		Count<Texture2D>Color;
		Count<Texture2D>Normal;
		Count<Texture2D>Metallic;
		Count<Texture2D>Roughness;
	};
	// takes from mesh pipline
	// make 
	struct MeshMaterialPipeline {
	public:
		// going to use mesh pipline buffers
		Count<class GraphicsPipeline> GraphicsPipeline;
		Count<class Shader> Shader;
		Count <class PipeLineLayout> PipeLineLayout;
		MeshMaterialPipeline(Count<class RenderPass> renderPass);
		Count<class PushConstant> MaterialPushConstant;
		uint32_t NumberMeshes;
		// order teh meshes are pushed intehvector that stores all transforms
		std::vector<uint64_t> ElementsImplaced;
		// the begin offset we should start rendering
		uint32_t OffsetBegin = 0;
		// mesh unique id, mesh transfomr
		std::unordered_map < UUID, std::vector< MeshPipeLine::MeshVertex>> MeshesTransforms;
		// mesh uniqe id, mesh instance 
		std::unordered_map < UUID, std::vector<MaterialMeshInstance>> Meshes;
		std::unordered_map<DescriptorSets, Count<DescriptorSet>> Descriptors;
		void Reset() {
			MeshesTransforms.clear();
			ElementsImplaced.clear();
			Meshes.clear();
			NumberMeshes = 0;
			OffsetBegin = 0;
		}
	
	};
	
	struct RenderStorage {
		Count<RenderCommandBuffer> CommandBuffer;
		Count<UniformBuffer> CameraBuffer = nullptr;
		Count<ScreenFrameBuffer> CurrentFrameBuffer = nullptr;
	};
	class Mesh;
	class Renderer3DPBR {
	public:
		Renderer3DPBR(Count<class RenderPass> renderPass);
		~Renderer3DPBR() {

		}
		void Init();
		void BeginContext(class EditorCamera& editorCamera, Count<ScreenFrameBuffer>& frameBuffer,Count<RenderCommandBuffer>& commandBuffer);
		void BeginContext(const glm::mat4& projection, const glm::mat4& view, const Vector& Position, Count<ScreenFrameBuffer>& frameBuffer, Count<RenderCommandBuffer>& commandBuffer);
		void SetPbrMaps(Count<class CubeMap> irrdianceMap, Count<class CubeMap> prefilterMap,Count<Texture2D> brdf);

		void SubmitMesh(Count<Mesh> mesh, const glm::mat4& transform);
		void SubmitMeshWithMaterial(Count<Mesh> mesh, Count<MaterialTable> table, const glm::mat4& transform);
		void SubmitSubMesh(Count<Mesh> mesh, uint32_t meshIndex, const glm::mat4& transform);
		void SubmitSubMeshWithMaterial(Count<Mesh> mesh, uint32_t index, Count<Material> material, const glm::mat4& transform);
		
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
