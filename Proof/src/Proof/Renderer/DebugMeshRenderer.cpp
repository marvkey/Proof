#include "Proofprch.h"
#include "DebugMeshRenderer.h"
#include "Vertex.h"
#include "VertexArray.h"
#include "GraphicsPipeLine.h"
#include "Proof/Scene/Mesh.h"
#include "Buffer.h"
namespace Proof{
		// add debug names
	enum class DescriptorSet0 {
	  //struct
		CameraData = 0,
	};
	static CameraData s_CurrentCamera;
	DebugMeshRenderer::DebugMeshRenderer(Count<RenderPass> renderPass)
	{
		m_RenderPass = renderPass;
		m_Pipeline = CreateSpecial<DebugMeshPipeLine>(m_RenderPass);
		m_CameraBuffer = UniformBuffer::Create(sizeof(CameraData));
	}

	void DebugMeshRenderer::BeginContext(const glm::mat4& projection, const glm::mat4& view, const Vector& Position, Count<ScreenFrameBuffer>& frameBuffer, Count<RenderCommandBuffer>& commandBuffer)
	{
		PF_PROFILE_FUNC()
		PF_SCOPE_TIME_THRESHHOLD_TYPE(__FUNCTION__, 1.0f, TimerTypes::RendererBase);
		PF_CORE_ASSERT(m_InContext == false, "Cannot begin context if already in a context");
		m_InContext = true;
		s_CurrentCamera = CameraData{ projection,view,glm::mat4(1),Position };
		m_CurrentFrameBuffer = frameBuffer;
		m_CommandBuffer = commandBuffer;
	}
	void DebugMeshRenderer::SubmitMesh(Count<class Mesh> mesh, const glm::mat4& transform)
	{
		PF_PROFILE_FUNC();
		if (mesh == nullptr)return;
		MemoryAddress meshPointerId = (MemoryAddress) &*mesh.Get();
		DebugMeshPipeLine::MeshVertex vertex(transform);

		if (m_Pipeline->Meshes.contains(meshPointerId))
			m_Pipeline->Meshes[meshPointerId].Count += 1;
		else
		{
			m_Pipeline->Meshes[meshPointerId].Mesh = mesh;
			m_Pipeline->Meshes[meshPointerId].Count += 1;
		}

		m_Pipeline->MeshesTransforms[meshPointerId].emplace_back(vertex);
		m_Pipeline->NumberMeshes++;
	}
	void DebugMeshRenderer::EndContext()
	{
		PF_CORE_ASSERT(m_InContext == true, "Cannot end context if already n a context");
		PF_PROFILE_FUNC()
		PF_SCOPE_TIME_THRESHHOLD_TYPE(__FUNCTION__, 0, TimerTypes::RendererBase);
		Draw();
		m_Pipeline->Reset();

		m_InContext = false;
	}
	void DebugMeshRenderer::Draw()
	{
		//std::vector<DebugMeshPipeLine::MeshVertex> meshesVertex;
		//
		//// setup
		//{
		//	m_Pipeline->OffsetBegin = 0;
		//	for (auto& [ID, transforms] : m_Pipeline->MeshesTransforms)
		//	{
		//		// dont use std::end instead back inserter because using std::end can resulst in undefinded behavior
		//		std::move(transforms.begin(), transforms.end(), std::back_inserter(meshesVertex));
		//		m_Pipeline->ElementsImplaced.emplace_back(ID);
		//	}
		//	if (meshesVertex.empty() == false)
		//		m_Pipeline->MeshesVertexBuffer = VertexBuffer::Create(meshesVertex.data(), meshesVertex.size() * sizeof(DebugMeshPipeLine::MeshVertex));
		//}
		//// render
		//if (m_Pipeline->ElementsImplaced.empty())return;
		//auto descriptor0 = m_Pipeline->Descriptors[DescriptorSets::Zero];
		//m_CameraBuffer->SetData(&s_CurrentCamera, sizeof(CameraData));
		//descriptor0->WriteBuffer((uint32_t)DescriptorSet0::CameraData, m_CameraBuffer);
		//uint32_t currentOffset = m_Pipeline->OffsetBegin;
		////Renderer::RecordRenderPass(m_RenderPass, m_Pipeline->GraphicsPipeline);
		//	descriptor0->Bind(m_CommandBuffer, m_Pipeline->PipeLineLayout);
		//	for (const uint64_t& ID : m_Pipeline->ElementsImplaced)
		//	{
		//		const uint64_t meshInstances = m_Pipeline->Meshes[ID].Count;
		//		Count<Mesh> mesh = m_Pipeline->Meshes[ID].Mesh;
		//
		//		Vector color(0.46275f, 0.72549f, 0.00000f);
		//		for (const auto& subMesh : mesh->GetMeshSource()->GetSubMeshes())
		//		{
		//			subMesh.VertexBuffer->Bind(m_CommandBuffer);
		//			subMesh.IndexBuffer->Bind(m_CommandBuffer);
		//			m_Pipeline->MeshesVertexBuffer->Bind(m_CommandBuffer, 1);
		//			m_Pipeline->PushConstantColor->PushData(m_CommandBuffer, m_Pipeline->PipeLineLayout, &color);
		//			Renderer::DrawElementIndexed(m_CommandBuffer, subMesh.IndexBuffer->GetCount(), meshInstances, currentOffset);
		//		}
		//
		//		currentOffset += meshInstances;
		//	}
	}

	DebugMeshPipeLine::DebugMeshPipeLine(Count<RenderPass> renderPass)
	{
		{
			///auto descriptor = DescriptorSet::Builder(DescriptorSets::Zero)
			///	.AddBinding((uint32_t)DescriptorSet0::CameraData, DescriptorType::UniformBuffer, ShaderStage::Vertex)
			///	.Build();
			///Descriptors.insert({ DescriptorSets::Zero,descriptor });

		}

		//Shader = Shader::GetOrCreate("DebugMesh", ProofCurrentDirectorySrc + "Proof/Renderer/Asset/Shader/DebugMeshShader.shader");
		//PushConstantColor = PushConstant::Create(sizeof(Vector), 0, ShaderStage::Fragment);
		//PipeLineLayout = PipeLineLayout::Create(std::vector{ Descriptors[DescriptorSets::Zero]}, PushConstantColor);

		Count<VertexArray> meshVertexArray = VertexArray::Create({ { sizeof(Vertex)}, {sizeof(MeshVertex), VertexInputRate::Instance} });
		meshVertexArray->AddData(0, DataType::Vec3, offsetof(Vertex, Vertex::Vertices));
		meshVertexArray->AddData(1, DataType::Vec3, offsetof(Vertex, Vertex::Normal));
		meshVertexArray->AddData(2, DataType::Vec2, offsetof(Vertex, Vertex::TexCoords));
		meshVertexArray->AddData(3, DataType::Vec3, offsetof(Vertex, Vertex::Tangent));
		meshVertexArray->AddData(4, DataType::Vec3, offsetof(Vertex, Vertex::Bitangent));


		meshVertexArray->AddData(5, DataType::Vec4, 0, 1);
		meshVertexArray->AddData(6, DataType::Vec4, (sizeof(glm::vec4) * 1), 1);
		meshVertexArray->AddData(7, DataType::Vec4, (sizeof(glm::vec4) * 2), 1);
		meshVertexArray->AddData(8, DataType::Vec4, (sizeof(glm::vec4) * 3), 1);
		GraphicsPipelineConfig graphicsPipelineConfig;
		graphicsPipelineConfig.DebugName = "Debug-Mesh";
		graphicsPipelineConfig.Shader = Shader;
		graphicsPipelineConfig.VertexArray = meshVertexArray;
		//graphicsPipelineConfig.PipelineLayout = PipeLineLayout;
		//graphicsPipelineConfig.RenderPass = renderPass;
		graphicsPipelineConfig.DrawMode = DrawType::LineStrip;
		graphicsPipelineConfig.LineWidth =5;
		//GraphicsPipeline = GraphicsPipeline::Create(graphicsPipelineConfig);
	}
}
