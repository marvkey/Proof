#include "Proofprch.h"
#include "Renderer3DPBR.h"
#include "../Shader.h"
#include "../VertexArray.h"
#include "../Renderer.h"

#include "Proof/Scene/Mesh.h"
#include "Proof/Scene/Component.h"
#include "Proof/Scene/Camera/OrthagraphicCamera.h"
#include "Proof/Scene/Camera/EditorCamera.h"

#include "Proof/Core/FrameTime.h"
#include "Proof/Scene/Mesh.h"
#include "Proof/Scene/Entity.h"
#include "Proof/Scene/Material.h"
#include "../UniformBuffer.h"
#include "Proof/Scene/Component.h"
#include "../PipeLineLayout.h"
#include "../RenderPass.h"
#include "../GraphicsPipeLine.h"
#include "Proof/Asset/AssetManager.h"
namespace Proof
{
	enum class DescriptorSet0 {
	  //struct
		CameraData = 0,
		//struct
		WorldData = 1,

	};

	enum class DescriptorSet1 {
		//texture
		AlbedoMap = 0,
		//texture
		NormalMap = 1,
		//texture
		metallicMap = 2,
		//texture
		roughnessMap = 3,
		//texture
		DiffuseMap = 4
	};

	static RenderStorage* s_RenderStorage;
	static MeshPipeLine* s_MeshPipeLine = nullptr;
	struct CameraData {
		CameraData() {};
		CameraData(const glm::mat4& projection, const glm::mat4& view, const Vector& pos) :
			m_Projection(projection), m_View(view), m_Positon(pos) {
		};
		glm::mat4 m_Projection;
		glm::mat4 m_View;
		Vector m_Positon;
	};
	static CameraData s_CurrentCamera;
	bool Renderer3DPBR::s_InContext;

	void Renderer3DPBR::Init() {
		PF_PROFILE_FUNC();
		s_RenderStorage = new RenderStorage();
		s_RenderStorage->MeshesVertexBuffer = VertexBuffer::Create(s_RenderStorage->MaxMesh * sizeof(MeshPipeLine::MeshVertex));
		s_RenderStorage->CommandBuffer = CommandBuffer::Create();
		s_RenderStorage->CameraBuffer = UniformBuffer::Create(sizeof(CameraData), DescriptorSets::Zero, (uint32_t)DescriptorSet0::CameraData);
		InitDescriptors();
		s_MeshPipeLine = new MeshPipeLine();
	}
	void Renderer3DPBR::BeginContext(EditorCamera& editorCamera, Count<ScreenFrameBuffer>& frameBuffer) {
		BeginContext(editorCamera.m_Projection, editorCamera.m_View, editorCamera.m_Positon, frameBuffer);
	}
	void Renderer3DPBR::BeginContext(const glm::mat4& projection, const glm::mat4& view, const Vector& Position, Count<ScreenFrameBuffer> frameBuffer) {
		PF_PROFILE_FUNC()
		PF_SCOPE_TIME_THRESHHOLD_TYPE(__FUNCTION__, 1.0f, TimerTypes::RendererBase);
		PF_CORE_ASSERT(s_InContext ==false, "Cannot begin context if already in a context");
		s_InContext = true;
		s_CurrentCamera = CameraData{ projection,view,Position };
		s_RenderStorage->CurrentFrameBuffer = frameBuffer;
		s_RenderStorage->CommandBuffer = CommandBuffer::Create();
	}

	void Renderer3DPBR::SubmitMesh(MeshComponent& mesh, const glm::mat4& transform) {
		uint64_t meshPointerId = mesh.GetMeshAssetID();
		if (meshPointerId == 0)return; // means that therer is no mesh 
		PF_PROFILE_FUNC();
		PF_PROFILE_TAG("Mesh ID", meshPointerId);

		if (s_RenderStorage->AmountMeshPerMeshAssetID.contains(meshPointerId)) {
			s_RenderStorage->AmountMeshPerMeshAssetID[meshPointerId] += 1;
			auto instanceBeginPos = s_RenderStorage->MeshesPositionAddedIndexTransforms.at(meshPointerId);

			uint32_t instanceCurrentPos = 0;
			uint32_t difference = s_RenderStorage->Transforms.size() - instanceBeginPos;
			instanceCurrentPos = instanceBeginPos + difference;

			MeshPipeLine::MeshVertex vertex(transform);
			s_RenderStorage->Transforms.insert(s_RenderStorage->Transforms.begin() + instanceCurrentPos, vertex);
			s_RenderStorage->NumberMeshes++;
			return;
		}
		s_RenderStorage->AmountMeshPerMeshAssetID.insert({ meshPointerId,1 });
		s_RenderStorage->MeshesPositionAddedIndexTransforms.insert({ meshPointerId , s_RenderStorage->Transforms.size() });
		s_RenderStorage->MeshesID.emplace_back(meshPointerId);
		MeshPipeLine::MeshVertex vertex(transform);
		s_RenderStorage->Transforms.emplace_back(vertex);
		s_RenderStorage->NumberMeshes++;
	}

	void Renderer3DPBR::SubmitDirectionalLight(class DirectionalLightComponent& comp, TransformComponent& transform) {
	}
	void Renderer3DPBR::SubmitPointLight(class PointLightComponent& comp, TransformComponent& transform) {
	}
	void Renderer3DPBR::SubmitSpotLight(class SpotLightComponent& comp, TransformComponent& transform) {
	}
	void Renderer3DPBR::DrawDebugMesh(Mesh* mesh, const glm::mat4& transform) {

	}
	
	void Renderer3DPBR::EndContext() {
		PF_CORE_ASSERT(s_InContext == true, "Cannot end context if already n a context");
		PF_PROFILE_FUNC()
		PF_SCOPE_TIME_THRESHHOLD_TYPE(__FUNCTION__, 0, TimerTypes::RendererBase);
		DrawContext();
		Reset();
		s_InContext = false;
	}
	void Renderer3DPBR::Reset() {
		s_RenderStorage->AmountMeshPerMeshAssetID.clear();
		s_RenderStorage->MeshesPositionAddedIndexTransforms.clear();
		s_RenderStorage->Transforms.clear();
		s_RenderStorage->MeshesID.clear();
		s_RenderStorage->NumberMeshes = 0;
		s_RenderStorage->CurrentFrameBuffer = nullptr;
	}

	void Renderer3DPBR::Destroy() {
	
	}
	void Renderer3DPBR::DrawMeshSource(uint64_t ID, uint64_t numMeshPerID, uint64_t offset) {
		auto descriptor1 = s_RenderStorage->Descriptors[DescriptorSets::One];
		const auto meshAsset = AssetManager::GetAsset<MeshSourceFileAsset>(ID);
				// get asset also does a check for an id so we will need to fix this
		auto& mesh = *meshAsset->GetMesh();
	
		for (const auto& subMesh : mesh.GetSubMeshes()) {
			if (subMesh.Enabled == false)return;
			Count<Texture2D> texture = subMesh.GetDiffuseIndex().size() > 0 ? mesh.textures_loaded[subMesh.GetDiffuseIndex()[0]]
				: Renderer::GetWhiteTexture();

			descriptor1->WriteImage((int)DescriptorSet1::AlbedoMap, texture);
			descriptor1->Bind(s_RenderStorage->CommandBuffer, s_MeshPipeLine->PipeLineLayout);
			subMesh.GetVertexBuffer()->Bind(s_RenderStorage->CommandBuffer);
			subMesh.GetIndexBuffer()->Bind(s_RenderStorage->CommandBuffer);
			s_RenderStorage->MeshesVertexBuffer->Bind(s_RenderStorage->CommandBuffer, 1);

			Renderer::DrawElementIndexed(s_RenderStorage->CommandBuffer, subMesh.GetIndexBuffer()->GetCount(), numMeshPerID, offset);
		}
		
	}
	void Renderer3DPBR::DrawContext() {
		s_RenderStorage->CameraBuffer->SetData(&s_CurrentCamera, sizeof(CameraData));
		auto descriptor0 = s_RenderStorage->Descriptors[DescriptorSets::Zero];
		auto descriptor1 = s_RenderStorage->Descriptors[DescriptorSets::One];

		descriptor0->WriteBuffer((int)DescriptorSet0::CameraData, s_RenderStorage->CameraBuffer);

		Renderer::BeginRenderPass(s_RenderStorage->CommandBuffer, s_MeshPipeLine->RenderPass,s_RenderStorage->CurrentFrameBuffer);
		Renderer::RecordRenderPass(s_MeshPipeLine->RenderPass, [&](Count <CommandBuffer> commandBuffer) {
			descriptor0->Bind(commandBuffer, s_MeshPipeLine->PipeLineLayout);
			uint32_t offset = 0;
			s_RenderStorage->MeshesVertexBuffer->AddData(s_RenderStorage->Transforms.data(), s_RenderStorage->Transforms.size() * sizeof(MeshPipeLine::MeshVertex));
			for (const uint64_t& ID : s_RenderStorage->MeshesID) {
				const uint64_t numMeshPerID = s_RenderStorage->AmountMeshPerMeshAssetID[ID];
				if (AssetManager::HasID(ID) == false)
					continue;
				const auto meshInfo = AssetManager::GetAssetInfo(ID);

				if (meshInfo.Type == AssetType::MeshSourceFile) {
				//this is a rare case for just teh asset manager if we want to render a mehs source
				//yeah simple thing
					DrawMeshSource(ID, numMeshPerID, offset);
					offset += numMeshPerID;
					continue;
				}

				const auto meshAsset = AssetManager::GetAsset<MeshAsset>(ID);
				// get asset also does a check for an id so we will need to fix this
				auto& mesh = *meshAsset->GetMesh();
				for (int i = 0; i < meshAsset->GetDiscardedMesh().size(); i++) {
					const auto & index= meshAsset->GetDiscardedMesh()[i];
					mesh.meshes[index].Enabled = false;
				}
				for (const auto& subMesh : mesh.GetSubMeshes()) {
					if (subMesh.Enabled == false)continue;
					Count<Texture2D> texture = subMesh.GetDiffuseIndex().size() > 0 ? mesh.textures_loaded[subMesh.GetDiffuseIndex()[0]]
						: Renderer::GetWhiteTexture();

					descriptor1->WriteImage((int)DescriptorSet1::AlbedoMap, texture);
					descriptor1->Bind(s_RenderStorage->CommandBuffer, s_MeshPipeLine->PipeLineLayout);
					subMesh.GetVertexBuffer()->Bind(s_RenderStorage->CommandBuffer);
					subMesh.GetIndexBuffer()->Bind(s_RenderStorage->CommandBuffer);
					s_RenderStorage->MeshesVertexBuffer->Bind(s_RenderStorage->CommandBuffer,1);

					Renderer::DrawElementIndexed(s_RenderStorage->CommandBuffer, subMesh.GetIndexBuffer()->GetCount(), numMeshPerID, offset);
				}
				for (int i = 0; i < meshAsset->GetDiscardedMesh().size(); i++) {
					const auto& index = meshAsset->GetDiscardedMesh()[i];
					mesh.meshes[index].Enabled = true;
				}
				offset += numMeshPerID;
			}
		});
		Renderer::EndRenderPass(s_MeshPipeLine->RenderPass);
		Renderer::SubmitCommandBuffer(s_RenderStorage->CommandBuffer);
	}

	

	void Renderer3DPBR::InitDescriptors() {
		{
			auto descriptor = DescriptorSet::Builder(DescriptorSets::Zero)
				.AddBinding((int)DescriptorSet0::CameraData, DescriptorType::UniformBuffer, ShaderStage::Vertex)
				.AddBinding((int)DescriptorSet0::WorldData, DescriptorType::UniformBuffer, ShaderStage::Vertex)
				.Build();
			s_RenderStorage->Descriptors.insert({ DescriptorSets::Zero,descriptor });
		}

		{
			auto descriptor = DescriptorSet::Builder(DescriptorSets::One)
				.AddBinding((int)DescriptorSet1::AlbedoMap, DescriptorType::ImageSampler, ShaderStage::Fragment)
				.AddBinding((int)DescriptorSet1::NormalMap, DescriptorType::ImageSampler, ShaderStage::Fragment)
				.AddBinding((int)DescriptorSet1::metallicMap, DescriptorType::ImageSampler, ShaderStage::Fragment)
				.AddBinding((int)DescriptorSet1::roughnessMap, DescriptorType::ImageSampler, ShaderStage::Fragment)
				.AddBinding((int)DescriptorSet1::DiffuseMap, DescriptorType::ImageSampler, ShaderStage::Fragment)
				.Build();
			s_RenderStorage->Descriptors.insert({ DescriptorSets::One,descriptor });
		}
	
	}

	MeshPipeLine::MeshPipeLine() {
		Shader = Shader::Create("MeshShader", ProofCurrentDirectorySrc + "Proof/Renderer/Asset/Shader/Mesh.shader");
		PipeLineLayout = PipeLineLayout::Create(std::vector{ s_RenderStorage->Descriptors[DescriptorSets::Zero],s_RenderStorage->Descriptors[DescriptorSets::One] });
		RenderPass = RenderPass::Create();

		Count<VertexArray> meshVertexArray = VertexArray::Create({ { sizeof(Vertex)}, {sizeof(MeshPipeLine::MeshVertex), VertexInputRate::Instance} });
		meshVertexArray->AddData(0, DataType::Vec3, offsetof(Vertex, Vertex::Vertices));
		meshVertexArray->AddData(1, DataType::Vec3, offsetof(Vertex, Vertex::Normal));
		meshVertexArray->AddData(2, DataType::Vec2, offsetof(Vertex, Vertex::TexCoords));
		meshVertexArray->AddData(3, DataType::Vec3, offsetof(Vertex, Vertex::Tangent));
		meshVertexArray->AddData(4, DataType::Vec3, offsetof(Vertex, Vertex::Bitangent));


		meshVertexArray->AddData(5, DataType::Vec4, 0, 1);
		meshVertexArray->AddData(6, DataType::Vec4, (sizeof(glm::vec4) * 1), 1);
		meshVertexArray->AddData(7, DataType::Vec4, (sizeof(glm::vec4) * 2), 1);
		meshVertexArray->AddData(8, DataType::Vec4, (sizeof(glm::vec4) * 3), 1);

		GraphicsPipeline = GraphicsPipeline::Create(Shader, RenderPass, PipeLineLayout, meshVertexArray);
		RenderPass->SetGraphicsPipeline(GraphicsPipeline);
	}
}