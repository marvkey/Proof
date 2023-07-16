#pragma once
#include "Proof/Core/Core.h"
#include <unordered_map>
#include "Proof/Renderer/FrameBuffer.h"
#include "Proof/Renderer/CommandBuffer.h"
#include "Proof/Renderer/UniformBuffer.h"
#include "Vertex.h"
namespace Proof{

	struct DebugMeshPipeLine {
		struct MeshVertex {

			MeshVertex(const glm::mat4& transform) :
				m_Transform(transform) {
			}
			glm::mat4 m_Transform;
		};
		Count<class GraphicsPipeline> GraphicsPipeline;
		Count<class Shader> Shader;
		Count<class VertexBuffer> MeshesVertexBuffer;
		//Count<class PushConstant> PushConstantColor;
		//Count <class PipeLineLayout> PipeLineLayout;
		std::unordered_map<DescriptorSets, Count<class DescriptorSet>> Descriptors;


		std::unordered_map < uint64_t, std::vector< MeshVertex>> MeshesTransforms;
		std::unordered_map < uint64_t, MeshInstance> Meshes;
		uint32_t OffsetBegin = 0;
		uint32_t NumberMeshes;
		// order teh meshes are pushed intehvector that stores all transforms
		std::vector<uint64_t> ElementsImplaced;
		DebugMeshPipeLine(Count<class RenderPass> renderPass);
		void Reset() {
			MeshesTransforms.clear();
			Meshes.clear();
			ElementsImplaced.clear();
			NumberMeshes = 0;
			OffsetBegin = 0;
		}
	};

	class DebugMeshRenderer {
	public:

		DebugMeshRenderer(Count<class RenderPass> renderPass);

		void BeginContext(const glm::mat4& projection, const glm::mat4& view, const Vector& Position, Count<ScreenFrameBuffer>& frameBuffer, Count<RenderCommandBuffer>& commandBuffer);
		void SubmitMesh(Count<class Mesh> mesh, const glm::mat4& transform);
		void EndContext();
	private:
		void Draw();

		bool m_InContext =false;
		Count<RenderCommandBuffer> m_CommandBuffer;
		Count<UniformBuffer> m_CameraBuffer = nullptr;
		Count<ScreenFrameBuffer> m_CurrentFrameBuffer = nullptr;
		Count <class RenderPass > m_RenderPass;

		Special< DebugMeshPipeLine> m_Pipeline;
	};
}
