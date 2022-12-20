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
		Count<class GraphicsPipeline> GraphicsPipeline;
		Count<class Shader> Shader;
		Count <class PipeLineLayout> PipeLineLayout;
		Count <class RenderPass > RenderPass;
		MeshPipeLine();
		struct MeshVertex {

			MeshVertex(const glm::mat4& transform) :
				m_Transform(transform) {
			}
			glm::mat4 m_Transform;
		};
	};
	
	struct RenderStorage {
		std::vector<Count<CommandBuffer>> CommandBuffers;
		Count<CommandBuffer> CommandBuffer;
		Count<UniformBuffer> CameraBuffer = nullptr;
		Count<ScreenFrameBuffer> CurrentFrameBuffer = nullptr;
		Count<VertexBuffer> MeshesVertexBuffer;
		std::unordered_map<DescriptorSets, Count<DescriptorSet>> Descriptors;

		//MESH ASSET ID, meshNumber
		std::unordered_map<AssetID, uint64_t> AmountMeshPerMeshAssetID;
		
		//this is not begening index
		// but its the beggeninig index of when the mesh was added to the vetor of transfomrs
		// with this we can use math to find the location of the first index of that specific index
		// of when it was first added
		std::unordered_map<AssetID, uint32_t> MeshesPositionAddedIndexTransforms;
		std::vector<MeshPipeLine::MeshVertex>Transforms;
		//we need this because when addign a id to a unordered map
		// treversing we could go in a different direction
		// therefore our transfomrs will be misplaced
		//with this we can add mesh id in tthe order there were added to the list 
		//and use the offset to treverse trhought them in the correct order the transforms were added
		std::vector<uint64_t> MeshesID;
		uint32_t NumberMeshes;
		const uint32_t MaxMesh = 1000;

	};
	
	
	struct RenderInfo {
		Count<ScreenFrameBuffer> FrameBuffer;
		Count<CommandBuffer> CommandBuffer;
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
		static void Reset();
		static void InitDescriptors();
		static bool s_InContext;

	};
}
