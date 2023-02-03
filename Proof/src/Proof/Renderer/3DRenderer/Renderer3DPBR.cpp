#include "Proofprch.h"
#include "Renderer3DPBR.h"
#include "../Shader.h"
#include "../VertexArray.h"
#include "../Renderer.h"

#include "Proof/Scene/Mesh.h"
#include "Proof/Scene/Component.h"
#include "Proof/Scene/Camera/OrthagraphicCamera.h"
#include "Proof/Scene/Camera/EditorCamera.h"
#include <utility>      
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
#include "Proof/Asset/MaterialAsset.h"
#include "Proof/Renderer/PushConstant.h"
namespace Proof
{
	
	
	// add debug names
	enum class DescriptorSet0 {
	  //struct
		CameraData = 0,
		//struct
		WorldData = 2,
		DirectionalLight=1,
	};

	enum class DescriptorSet1 {
		//texture
		AlbedoMap = 0,
		//texture
		NormalMap = 1,
		//texture
		MetallicMap = 2,
		//texture
		RoughnessMap = 3,
	};

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

	Renderer3DPBR::Renderer3DPBR(Count<RenderPass> renderPass)
	{
		m_RenderPass = renderPass;
		Init();
	}

	void Renderer3DPBR::Init() {
		PF_PROFILE_FUNC();
		m_RenderStorage = CreateSpecial<RenderStorage>();
		m_MeshPipeLine = CreateSpecial<MeshPipeLine>(m_RenderPass);
		m_MeshMaterialPipeline = CreateSpecial<MeshMaterialPipeline>(m_RenderPass);
		m_RenderStorage->CameraBuffer = UniformBuffer::Create(sizeof(CameraData), DescriptorSets::Zero, (uint32_t)DescriptorSet0::CameraData);
	}
	void Renderer3DPBR::BeginContext(EditorCamera& editorCamera, Count<ScreenFrameBuffer>& frameBuffer, Count<RenderCommandBuffer>& commandBuffer) {
		BeginContext(editorCamera.m_Projection, editorCamera.m_View, editorCamera.m_Positon, frameBuffer,commandBuffer);
	}
	void Renderer3DPBR::BeginContext(const glm::mat4& projection, const glm::mat4& view, const Vector& Position, Count<ScreenFrameBuffer>& frameBuffer, Count<RenderCommandBuffer>& commandBuffer) {
		PF_PROFILE_FUNC()
		PF_SCOPE_TIME_THRESHHOLD_TYPE(__FUNCTION__, 1.0f, TimerTypes::RendererBase);
		PF_CORE_ASSERT(s_InContext ==false, "Cannot begin context if already in a context");
		s_InContext = true;
		s_CurrentCamera = CameraData{ projection,view,Position };
		m_RenderStorage->CurrentFrameBuffer = frameBuffer;
		m_RenderStorage->CommandBuffer = commandBuffer;
	}

	void Renderer3DPBR::SubmitMesh(MeshComponent& mesh, const glm::mat4& transform) {
		uint64_t meshPointerId = mesh.GetMeshAssetID();
		if (meshPointerId == 0)return; // means that therer is no mesh 
		PF_PROFILE_FUNC();
		PF_PROFILE_TAG("Mesh ID", meshPointerId);
		MeshPipeLine::MeshVertex vertex(transform);
		if (mesh.HasMaterial())
		{
			auto mat = AssetManager::GetAsset<MaterialAsset>(mesh.GetMaterialAssetID())->GetMaterial();
			MaterialData data;
			data.Colour = mat.Colour;
			data.Roughness = mat.Roughness;
			data.Metallness = mat.Metallness;

			m_MeshMaterialPipeline->MeshesTransforms[meshPointerId].emplace_back(vertex);
			m_MeshMaterialPipeline->AmountMeshes[meshPointerId] += 1;
			m_MeshMaterialPipeline->MaterialDatas[meshPointerId] = data;
			m_MeshMaterialPipeline->NumberMeshes++;
		}
		else
		{
			m_MeshPipeLine->MeshesTransforms[meshPointerId].emplace_back(vertex);
			m_MeshPipeLine->AmountMeshes[meshPointerId] += 1;
			m_MeshPipeLine->NumberMeshes++;
		}
		//if (mesh.HasMaterial())
			//vertex.Color = 
		// [] creatses it if it does not exist
	}

	void Renderer3DPBR::SubmitDirectionalLight(const DirLight& light) {
		m_MeshPipeLine->LightPass.DirLights.emplace_back(light);
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
		m_MeshPipeLine->Reset();
		m_MeshMaterialPipeline->Reset();

		m_RenderStorage->CurrentFrameBuffer = nullptr;
	}

	void Renderer3DPBR::Destroy() {
	
	}
	void Renderer3DPBR::SetPasses()
	{
		// camera data
		m_RenderStorage->CameraBuffer->SetData(&s_CurrentCamera, sizeof(CameraData));

		SetMeshPass();

	}
	void Renderer3DPBR::SetMeshPass()
	{
		std::vector<MeshPipeLine::MeshVertex> meshesVertex;

		auto descriptor0 = m_MeshPipeLine->Descriptors[DescriptorSets::Zero];

		descriptor0->WriteBuffer((uint32_t)DescriptorSet0::CameraData, m_RenderStorage->CameraBuffer);
		m_MeshPipeLine->LightPass.DirLightsBuffer = UniformBuffer::Create(sizeof(DirLight), DescriptorSets::Zero, (uint32_t)DescriptorSet0::DirectionalLight);
		if (m_MeshPipeLine->LightPass.DirLights.size() > 0)
		{
			m_MeshPipeLine->LightPass.DirLightsBuffer->SetData(&m_MeshPipeLine->LightPass.DirLights[0], sizeof(DirLight));
		}
		else
		{
			DirLight base;
			base.Color = Vector{ 0 };
			m_MeshPipeLine->LightPass.DirLightsBuffer->SetData(&base, sizeof(DirLight));
		}
		descriptor0->WriteBuffer((uint32_t)DescriptorSet0::DirectionalLight, m_MeshPipeLine->LightPass.DirLightsBuffer);
		// mesh pbr 
		{
			m_MeshPipeLine->OffsetBegin = 0;
			for (auto& [ID, transforms] : m_MeshPipeLine->MeshesTransforms)
			{
				// dont use std::end instead back inserter because using std::end can resulst in undefinded behavior
				std::move(transforms.begin(), transforms.end(), std::back_inserter(meshesVertex));
				m_MeshPipeLine->ElementsImplaced.emplace_back(ID);
			}
		}

		{
			descriptor0 = m_MeshMaterialPipeline->Descriptors[DescriptorSets::Zero];
			descriptor0->WriteBuffer((uint32_t)DescriptorSet0::CameraData, m_RenderStorage->CameraBuffer);
			descriptor0->WriteBuffer((uint32_t)DescriptorSet0::DirectionalLight, m_MeshPipeLine->LightPass.DirLightsBuffer);

			// mesh material pbr 
			m_MeshMaterialPipeline->OffsetBegin = meshesVertex.size();
			for (auto& [ID, transforms] : m_MeshMaterialPipeline->MeshesTransforms)
			{
				// dont use std::end instead back inserter because using std::end can resulst in undefinded behavior
				std::move(transforms.begin(), transforms.end(), std::back_inserter(meshesVertex));
				m_MeshMaterialPipeline->ElementsImplaced.emplace_back(ID);
			}
		}
		if(meshesVertex.empty() == false)
			m_MeshPipeLine->MeshesVertexBuffer = VertexBuffer::Create(meshesVertex.data(),meshesVertex.size() * sizeof(MeshPipeLine::MeshVertex));
	}
	void Renderer3DPBR::RenderMesh()
	{
		if (m_MeshPipeLine->ElementsImplaced.empty())return;

		auto descriptor0 = m_MeshPipeLine->Descriptors[DescriptorSets::Zero];
		auto descriptor1 = m_MeshPipeLine->Descriptors[DescriptorSets::One];
		uint32_t currentOffset = m_MeshPipeLine->OffsetBegin;
		Renderer::RecordRenderPass(m_RenderPass, m_MeshPipeLine->GraphicsPipeline, [&](Count <RenderCommandBuffer> commandBuffer) {
			descriptor0->Bind(m_RenderStorage->CommandBuffer, m_MeshPipeLine->PipeLineLayout);
			for (const uint64_t& ID : m_MeshPipeLine->ElementsImplaced)
			{
				const uint64_t numMeshPerID = m_MeshPipeLine->AmountMeshes[ID];
				if (AssetManager::HasID(ID) == false)
					continue;
				const auto meshInfo = AssetManager::GetAssetInfo(ID);

				if (meshInfo.Type == AssetType::MeshSourceFile)
				{
					//this is a rare case for just teh asset manager if we want to render a mehs source
					//yeah simple thing
					DrawMeshSource(ID, numMeshPerID, currentOffset);
					currentOffset += numMeshPerID;
					continue;
				}

				const auto meshAsset = AssetManager::GetAsset<MeshAsset>(ID);
				auto meshtemp = meshAsset->GetMesh();
				if (meshtemp == nullptr)continue;
				auto& mesh = *meshtemp;
				for (int i = 0; i < meshAsset->GetDiscardedMesh().size(); i++)
				{
					const auto& index = meshAsset->GetDiscardedMesh()[i];
					mesh.meshes[index].Enabled = false;
				}


				for (const auto& subMesh : mesh.GetSubMeshes())
				{
					if (subMesh.Enabled == false)continue;
					Count<Texture2D> texture = subMesh.GetDiffuseTextures().size() > 0 ? AssetManager::GetAsset<Texture2DAsset>(subMesh.GetDiffuseTextures()[0])->GetTexture()
						: Renderer::GetWhiteTexture();
					descriptor1->WriteImage((int)DescriptorSet1::AlbedoMap, texture);
					descriptor1->Bind(m_RenderStorage->CommandBuffer, m_MeshPipeLine->PipeLineLayout);
					subMesh.GetVertexBuffer()->Bind(m_RenderStorage->CommandBuffer);
					subMesh.GetIndexBuffer()->Bind(m_RenderStorage->CommandBuffer);
					m_MeshPipeLine->MeshesVertexBuffer->Bind(m_RenderStorage->CommandBuffer, 1);

					Renderer::DrawElementIndexed(m_RenderStorage->CommandBuffer, subMesh.GetIndexBuffer()->GetCount(), numMeshPerID, currentOffset);
				}
				for (int i = 0; i < meshAsset->GetDiscardedMesh().size(); i++)
				{
					const auto& index = meshAsset->GetDiscardedMesh()[i];
					mesh.meshes[index].Enabled = true;
				}
				currentOffset += numMeshPerID;
			}
		});
	}
	void Renderer3DPBR::RenderMeshMaterial()
	{
		if (m_MeshMaterialPipeline->ElementsImplaced.empty())return;

		auto descriptor0 = m_MeshMaterialPipeline->Descriptors[DescriptorSets::Zero];
		auto descriptor1 = m_MeshMaterialPipeline->Descriptors[DescriptorSets::One];
		uint32_t currentOffset = m_MeshMaterialPipeline->OffsetBegin;
		Renderer::RecordRenderPass(m_RenderPass, m_MeshMaterialPipeline->GraphicsPipeline, [&](Count <RenderCommandBuffer> commandBuffer) {
			descriptor0->Bind(commandBuffer, m_MeshMaterialPipeline->PipeLineLayout);
			for (const uint64_t& ID : m_MeshMaterialPipeline->ElementsImplaced)
			{
				const uint64_t numMeshPerID = m_MeshMaterialPipeline->AmountMeshes[ID];
				auto material = m_MeshMaterialPipeline->MaterialDatas[ID];

				if (AssetManager::HasID(ID) == false)
					continue;
				const auto meshInfo = AssetManager::GetAssetInfo(ID);


				const auto meshAsset = AssetManager::GetAsset<MeshAsset>(ID);
				auto meshtemp = meshAsset->GetMesh();
				if (meshtemp == nullptr)continue;
				auto& mesh = *meshtemp;
				for (int i = 0; i < meshAsset->GetDiscardedMesh().size(); i++)
				{
					const auto& index = meshAsset->GetDiscardedMesh()[i];
					mesh.meshes[index].Enabled = false;
				}

				for (const auto& subMesh : mesh.GetSubMeshes())
				{
					if (subMesh.Enabled == false)continue;
					Count<Texture2D> texture = subMesh.GetDiffuseTextures().size() > 0 ? AssetManager::GetAsset<Texture2DAsset>(subMesh.GetDiffuseTextures()[0])->GetTexture()
						: Renderer::GetWhiteTexture();

					descriptor1->WriteImage((int)DescriptorSet1::AlbedoMap, texture);
					//descriptor1->WriteImage((int)DescriptorSet1::NormalMap, Renderer::GetWhiteTexture());
					//descriptor1->WriteImage((int)DescriptorSet1::MetallicMap, Renderer::GetWhiteTexture());
					//descriptor1->WriteImage((int)DescriptorSet1::RoughnessMap, Renderer::GetWhiteTexture());
					descriptor1->Bind(m_RenderStorage->CommandBuffer, m_MeshMaterialPipeline->PipeLineLayout);
					subMesh.GetVertexBuffer()->Bind(m_RenderStorage->CommandBuffer);
					subMesh.GetIndexBuffer()->Bind(m_RenderStorage->CommandBuffer);

					// TODO(move) into the rnderer 3D class might go into the renderer3d itself
					m_MeshPipeLine->MeshesVertexBuffer->Bind(m_RenderStorage->CommandBuffer, 1);
					m_MeshMaterialPipeline->MaterialPushConstant->PushData(m_RenderStorage->CommandBuffer, m_MeshMaterialPipeline->PipeLineLayout, &material);
					Renderer::DrawElementIndexed(m_RenderStorage->CommandBuffer, subMesh.GetIndexBuffer()->GetCount(), numMeshPerID, currentOffset);
				}
				for (int i = 0; i < meshAsset->GetDiscardedMesh().size(); i++)
				{
					const auto& index = meshAsset->GetDiscardedMesh()[i];
					mesh.meshes[index].Enabled = true;
				}
				currentOffset += numMeshPerID;
			}
		});
	}
	void Renderer3DPBR::MeshPass()
	{
		RenderMesh();
		RenderMeshMaterial();
	}
	
	void Renderer3DPBR::DrawMeshSource(uint64_t ID, uint64_t numMeshPerID, uint64_t offset) {
		auto descriptor1 = m_MeshPipeLine->Descriptors[DescriptorSets::One];
		const auto meshAsset = AssetManager::GetAsset<MeshSourceFileAsset>(ID);
				// get asset also does a check for an id so we will need to fix this
		auto& mesh = *meshAsset->GetMesh();
	
		for (const auto& subMesh : mesh.GetSubMeshes()) {
			if (subMesh.Enabled == false)continue;
			Count<Texture2D> texture = subMesh.GetDiffuseTextures().size() > 0 ? AssetManager::GetAsset<Texture2DAsset>(subMesh.GetDiffuseTextures()[0])->GetTexture()
				: Renderer::GetWhiteTexture();

			descriptor1->WriteImage((int)DescriptorSet1::AlbedoMap, texture);
			descriptor1->Bind(m_RenderStorage->CommandBuffer, m_MeshPipeLine->PipeLineLayout);
			subMesh.GetVertexBuffer()->Bind(m_RenderStorage->CommandBuffer);
			subMesh.GetIndexBuffer()->Bind(m_RenderStorage->CommandBuffer);
			m_MeshPipeLine->MeshesVertexBuffer->Bind(m_RenderStorage->CommandBuffer, 1);

			Renderer::DrawElementIndexed(m_RenderStorage->CommandBuffer, subMesh.GetIndexBuffer()->GetCount(), numMeshPerID, offset);
		}
		
	}
	void Renderer3DPBR::DrawContext() {
		SetPasses();
		MeshPass();
	}


	MeshPipeLine::MeshPipeLine(Count<RenderPass> renderPass) {
		{
			auto descriptor = DescriptorSet::Builder(DescriptorSets::Zero)
				.AddBinding((uint32_t)DescriptorSet0::CameraData, DescriptorType::UniformBuffer, ShaderStage::Vertex)
				.AddBinding((uint32_t)DescriptorSet0::WorldData, DescriptorType::UniformBuffer, ShaderStage::Vertex)
				.AddBinding((uint32_t)DescriptorSet0::DirectionalLight, DescriptorType::UniformBuffer, ShaderStage::Fragment)
				.Build();
			Descriptors.insert({ DescriptorSets::Zero,descriptor });

		}

		{
			auto descriptor = DescriptorSet::Builder(DescriptorSets::One)
				.AddBinding((uint32_t)DescriptorSet1::AlbedoMap, DescriptorType::ImageSampler, ShaderStage::Fragment)
				.AddBinding((uint32_t)DescriptorSet1::NormalMap, DescriptorType::ImageSampler, ShaderStage::Fragment)
				.AddBinding((uint32_t)DescriptorSet1::MetallicMap, DescriptorType::ImageSampler, ShaderStage::Fragment)
				.AddBinding((uint32_t)DescriptorSet1::RoughnessMap, DescriptorType::ImageSampler, ShaderStage::Fragment)
				.Build();
			Descriptors.insert({ DescriptorSets::One,descriptor });
		}
		std::unordered_map<ShaderStage, std::string> shaders;
		shaders[ShaderStage::Vertex] = ProofCurrentDirectorySrc + "Proof/Renderer/Asset/Shader/PBR/Mesh-PBR.vert";
		shaders[ShaderStage::Fragment] = ProofCurrentDirectorySrc + "Proof/Renderer/Asset/Shader/PBR/Mesh-PBR.frag";

		Shader = Shader::GetOrCreate("MeshShader", shaders);
		PipeLineLayout = PipeLineLayout::Create(std::vector{ Descriptors[DescriptorSets::Zero],Descriptors[DescriptorSets::One] });

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
		GraphicsPipelineConfig graphicsPipelineConfig;
		graphicsPipelineConfig.DebugName = "Mesh";
		graphicsPipelineConfig.Shader = Shader;
		graphicsPipelineConfig.VertexArray = meshVertexArray;
		graphicsPipelineConfig.PipelineLayout = PipeLineLayout;
		graphicsPipelineConfig.RenderPass = renderPass;

		GraphicsPipeline = GraphicsPipeline::Create(graphicsPipelineConfig);
	}
	MeshMaterialPipeline::MeshMaterialPipeline(Count<RenderPass> renderPass)
	{
	
		{
			auto descriptor = DescriptorSet::Builder(DescriptorSets::Zero)
				.AddBinding((uint32_t)DescriptorSet0::CameraData, DescriptorType::UniformBuffer, ShaderStage::Vertex)
				.AddBinding((uint32_t)DescriptorSet0::WorldData, DescriptorType::UniformBuffer, ShaderStage::Vertex)
				.AddBinding((uint32_t)DescriptorSet0::DirectionalLight, DescriptorType::UniformBuffer, ShaderStage::Fragment)
				.Build();
			Descriptors.insert({ DescriptorSets::Zero,descriptor });

		}

		{
			auto descriptor = DescriptorSet::Builder(DescriptorSets::One)
				.AddBinding((uint32_t)DescriptorSet1::AlbedoMap, DescriptorType::ImageSampler, ShaderStage::Fragment)
				.AddBinding((uint32_t)DescriptorSet1::NormalMap, DescriptorType::ImageSampler, ShaderStage::Fragment)
				.AddBinding((uint32_t)DescriptorSet1::MetallicMap, DescriptorType::ImageSampler, ShaderStage::Fragment)
				.AddBinding((uint32_t)DescriptorSet1::RoughnessMap, DescriptorType::ImageSampler, ShaderStage::Fragment)
				.Build();
			Descriptors.insert({ DescriptorSets::One,descriptor });
		}
		std::unordered_map<ShaderStage, std::string> shaders;
		shaders[ShaderStage::Vertex] = ProofCurrentDirectorySrc + "Proof/Renderer/Asset/Shader/PBR/Mesh-PBR.vert";
		shaders[ShaderStage::Fragment] = ProofCurrentDirectorySrc + "Proof/Renderer/Asset/Shader/PBR/Mesh-PBRMaterial.frag";

		Shader = Shader::GetOrCreate("Mesh-PBRShader", shaders);
		MaterialPushConstant = PushConstant::Create(sizeof(MaterialData), 0, ShaderStage::Fragment);
		PipeLineLayout = PipeLineLayout::Create(std::vector{ Descriptors[DescriptorSets::Zero],Descriptors[DescriptorSets::One] }, MaterialPushConstant);

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
		GraphicsPipelineConfig graphicsPipelineConfig;
		graphicsPipelineConfig.DebugName = "Mesh";
		graphicsPipelineConfig.Shader = Shader;
		graphicsPipelineConfig.VertexArray = meshVertexArray;
		graphicsPipelineConfig.PipelineLayout = PipeLineLayout;
		graphicsPipelineConfig.RenderPass = renderPass;

		GraphicsPipeline = GraphicsPipeline::Create(graphicsPipelineConfig);
	}
	DebugMeshPipeLine::DebugMeshPipeLine()
	{
		/*
		{
			auto descriptor = DescriptorSet::Builder(DescriptorSets::Zero)
				.AddBinding((int)DescriptorSet0::CameraData, DescriptorType::UniformBuffer, ShaderStage::Vertex)
				.Build();
			Descriptors.insert({ DescriptorSets::Zero,descriptor });
		}

	
		Shader = Shader::GetOrCreate("DebugMesh", ProofCurrentDirectorySrc + "Proof/Renderer/Asset/Shader/Mesh.shader");
		PipeLineLayout = PipeLineLayout::Create(std::vector{ Descriptors[DescriptorSets::Zero] });
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

		//GraphicsPipeline = GraphicsPipeline::Create(Shader, RenderPass, PipeLineLayout, meshVertexArray);
		*/
	}
	
}