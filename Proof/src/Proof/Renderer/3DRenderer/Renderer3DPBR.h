#pragma once
#include <unordered_map>
#include <array>
#include <vector>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "Proof/Scene/Material.h"
#include "Proof/Renderer/RendererAPI.h"
#include "Proof/Renderer/ScreenFrameBuffer.h"
#include "Proof/Renderer/CommandBuffer.h"
#include "Proof/Renderer/UniformBuffer.h"
#include "Proof/Scene/Component.h"

namespace Proof
{

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
		Count <class RenderPass > RenderPass;
		Count<VertexBuffer> MeshesVertexBuffer;
		std::unordered_map<DescriptorSets, Count<DescriptorSet>> Descriptors;

		// id, and all teh transforms for that mesh
		std::unordered_map < AssetID, std::vector< MeshPipeLine::MeshVertex>> MeshesTransforms;
		std::unordered_map < AssetID, uint32_t> AmountMeshes;
		uint32_t NumberMeshes;
		const uint32_t MaxMesh = 2000;
		MeshPipeLine();
	};
	
	struct RenderStorage {
		Count<CommandBuffer> CommandBuffer;
		Count<UniformBuffer> CameraBuffer = nullptr;
		Count<ScreenFrameBuffer> CurrentFrameBuffer = nullptr;
	};
	
	
	class Renderer3DPBR {
	public:
		static void Init();
		static void BeginContext(class EditorCamera& editorCamera, Count<ScreenFrameBuffer>& frameBuffer);
		static void BeginContext(const glm::mat4& projection, const glm::mat4& view, const Vector& Position, Count<ScreenFrameBuffer> frameBuffer);
		static void SubmitMesh(class MeshComponent& meshComponent, const glm::mat4& transform);
		static void SubmitDirectionalLight(class DirectionalLightComponent& comp, class TransformComponent& transform);
		static void SubmitPointLight(class PointLightComponent& comp, class TransformComponent& transform);
		static void SubmitSpotLight(class SpotLightComponent& comp, class TransformComponent& transform);
		static void DrawDebugMesh(class Mesh* mesh, const glm::mat4& transform);
		static void EndContext();
		static void Destroy();
	private:
		static void DrawContext();
		static void DrawMeshSource(uint64_t id,uint64_t num,uint64_t offset);
		static void Reset();
		static bool s_InContext;

	};
}
