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
#include "Proof/Renderer/PushConstant.h"
#include "Proof/Renderer/MeshWorkShop.h"
namespace Proof
{
	
	
	// add debug names
	enum class DescriptorSet0 {
	  //struct
		CameraData = 0,
		DirectionalLight=1,
		IrradianceMap = 4,
		PrefilterMap = 5,
		BRDF = 6,
		//struct
		WorldData = 3,
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
		BeginContext(editorCamera.m_Projection, editorCamera.m_View, GlmVecToProof(editorCamera.m_Positon), frameBuffer,commandBuffer);
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


	void Renderer3DPBR::SetPbrMaps(Count<class CubeMap> irrdianceMap, Count<class CubeMap> prefilterMap, Count<Texture2D> brdf)
	{
		m_MeshPipeLine->IrradianceMap = irrdianceMap;
		m_MeshPipeLine->PrefilterMap = prefilterMap;
		m_MeshPipeLine->BRDf = brdf;

	}

	void Renderer3DPBR::SubmitMesh(Count<Mesh> mesh, const glm::mat4& transform) {
		PF_PROFILE_FUNC();
		if (mesh == nullptr)return;
		uint64_t meshPointerId = mesh->GetMeshSpecificID();
		MeshPipeLine::MeshVertex vertex(transform);

		if (m_MeshPipeLine->Meshes.contains(meshPointerId))
			m_MeshPipeLine->Meshes[meshPointerId].Count += 1;
		else
		{
			m_MeshPipeLine->Meshes[meshPointerId].Mesh = mesh;
			m_MeshPipeLine->Meshes[meshPointerId].Count += 1;
		}

		m_MeshPipeLine->MeshesTransforms[meshPointerId].emplace_back(vertex);
		m_MeshPipeLine->NumberMeshes++;
	}

	void Renderer3DPBR::SubmitMeshWithMaterial(Count<Mesh> mesh, Count<Material> material, const glm::mat4& transform)
	{
		PF_PROFILE_FUNC();
		if (mesh == nullptr || material ==nullptr)return;
		uint64_t meshPointerId = mesh->GetMeshSpecificID();
		MeshPipeLine::MeshVertex vertex(transform);

		MaterialData data;
		data.Colour = material->Colour;
		data.Roughness = material->Roughness;
		data.Metallness = material->Metallness;

		if (m_MeshMaterialPipeline->Meshes.contains(meshPointerId))
			m_MeshMaterialPipeline->Meshes[meshPointerId].Count += 1;
		else
		{
			m_MeshMaterialPipeline->Meshes[meshPointerId].Mesh = mesh;
			m_MeshMaterialPipeline->Meshes[meshPointerId].Count += 1;
		}
		m_MeshMaterialPipeline->MeshesTransforms[meshPointerId].emplace_back(vertex);
		m_MeshMaterialPipeline->MaterialDatas[meshPointerId] = data;
		m_MeshMaterialPipeline->NumberMeshes++;
	}

	void Renderer3DPBR::SubmitDirectionalLight(const DirLight& light) {
		m_MeshPipeLine->LightPass.DirLights.emplace_back(light);
	}
	void Renderer3DPBR::SubmitPointLight(class PointLightComponent& comp, TransformComponent& transform) {
	}
	void Renderer3DPBR::SubmitSpotLight(class SpotLightComponent& comp, TransformComponent& transform) {
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
		m_RenderStorage->CommandBuffer = nullptr;
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
		descriptor0->WriteBuffer((uint32_t)DescriptorSet0::CameraData, m_RenderStorage->CameraBuffer);
		descriptor0->WriteBuffer((uint32_t)DescriptorSet0::DirectionalLight, m_MeshPipeLine->LightPass.DirLightsBuffer);
		PF_CORE_ASSERT(m_MeshPipeLine->IrradianceMap, "Irradiance map is empty");
		PF_CORE_ASSERT(m_MeshPipeLine->PrefilterMap, "Prefilter map is empty");
		PF_CORE_ASSERT(m_MeshPipeLine->BRDf, "BRDF map is empty");
		descriptor0->WriteImage((uint32_t)DescriptorSet0::IrradianceMap, m_MeshPipeLine->IrradianceMap);
		descriptor0->WriteImage((uint32_t)DescriptorSet0::PrefilterMap, m_MeshPipeLine->PrefilterMap);
		descriptor0->WriteImage((uint32_t)DescriptorSet0::BRDF, m_MeshPipeLine->BRDf);
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
			descriptor0->WriteImage((uint32_t)DescriptorSet0::IrradianceMap, m_MeshPipeLine->IrradianceMap);
			descriptor0->WriteImage((uint32_t)DescriptorSet0::PrefilterMap, m_MeshPipeLine->PrefilterMap);
			descriptor0->WriteImage((uint32_t)DescriptorSet0::BRDF, m_MeshPipeLine->BRDf);
			
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
				const uint64_t meshInstances = m_MeshPipeLine->Meshes[ID].Count;
				const Count<Mesh> mesh = m_MeshPipeLine->Meshes[ID].Mesh;

				for (const auto& subMesh : mesh->GetSubMeshes())
				{
					if (subMesh.Enabled == false)continue;
					Count<Texture2D> texture = subMesh.GetDiffuseTextures().size() > 0 ? AssetManager::GetAsset<Texture2D>(subMesh.GetDiffuseTextures()[0])
						: Renderer::GetWhiteTexture();
					//texture = Renderer::GetWhiteTexture();
					descriptor1->WriteImage((int)DescriptorSet1::AlbedoMap, texture);
					descriptor1->Bind(m_RenderStorage->CommandBuffer, m_MeshPipeLine->PipeLineLayout);
					subMesh.GetVertexBuffer()->Bind(m_RenderStorage->CommandBuffer);
					subMesh.GetIndexBuffer()->Bind(m_RenderStorage->CommandBuffer);
					m_MeshPipeLine->MeshesVertexBuffer->Bind(m_RenderStorage->CommandBuffer, 1);

					Renderer::DrawElementIndexed(m_RenderStorage->CommandBuffer, subMesh.GetIndexBuffer()->GetCount(), meshInstances, currentOffset);
				}
				
				currentOffset += meshInstances;
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
				const uint64_t meshInstances = m_MeshMaterialPipeline->Meshes[ID].Count;
				Count<Mesh> mesh = m_MeshMaterialPipeline->Meshes[ID].Mesh;
				MaterialData materialData = m_MeshMaterialPipeline->MaterialDatas[ID];

				for (const auto& subMesh : mesh->GetSubMeshes())
				{
					if (subMesh.Enabled == false)continue;
					Count<Texture2D> texture = subMesh.GetDiffuseTextures().size() > 0 ? AssetManager::GetAsset<Texture2D>(subMesh.GetDiffuseTextures()[0])
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
					m_MeshMaterialPipeline->MaterialPushConstant->PushData(m_RenderStorage->CommandBuffer, m_MeshMaterialPipeline->PipeLineLayout, &materialData);
					Renderer::DrawElementIndexed(m_RenderStorage->CommandBuffer, subMesh.GetIndexBuffer()->GetCount(), meshInstances, currentOffset);
				}
				currentOffset += meshInstances;
			}
		});
	}
	void Renderer3DPBR::MeshPass()
	{
		RenderMesh();
		RenderMeshMaterial();
	}
	
	void Renderer3DPBR::DrawContext() {
		SetPasses();
		MeshPass();
	}


	MeshPipeLine::MeshPipeLine(Count<RenderPass> renderPass) {
		{
			auto descriptor = DescriptorSet::Builder(DescriptorSets::Zero)
				.AddBinding((uint32_t)DescriptorSet0::CameraData, DescriptorType::UniformBuffer, ShaderStage::Vertex)
				.AddBinding((uint32_t)DescriptorSet0::DirectionalLight, DescriptorType::UniformBuffer, ShaderStage::Fragment)
				.AddBinding((uint32_t)DescriptorSet0::IrradianceMap, DescriptorType::ImageSampler, ShaderStage::Fragment)
				.AddBinding((uint32_t)DescriptorSet0::PrefilterMap, DescriptorType::ImageSampler, ShaderStage::Fragment)
				.AddBinding((uint32_t)DescriptorSet0::BRDF, DescriptorType::ImageSampler, ShaderStage::Fragment)
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
				.AddBinding((uint32_t)DescriptorSet0::DirectionalLight, DescriptorType::UniformBuffer, ShaderStage::Fragment)
				.AddBinding((uint32_t)DescriptorSet0::IrradianceMap, DescriptorType::ImageSampler, ShaderStage::Fragment)
				.AddBinding((uint32_t)DescriptorSet0::PrefilterMap, DescriptorType::ImageSampler, ShaderStage::Fragment)
				.AddBinding((uint32_t)DescriptorSet0::BRDF, DescriptorType::ImageSampler, ShaderStage::Fragment)
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
}