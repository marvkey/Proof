#pragma once
#include <unordered_map>
#include <array>
#include <vector>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
namespace Proof {
	class MeshComponent;

	class Proof_API Renderer3D {
	public:
		static void Init();
		static void BeginContext(const class PerspectiveCamera& camera);
		static void BeginContext(glm::mat4 Projection,class EditorCamera& EditorCamera);

		static void BeginContext(const class OrthagraphicCamera& camera);

		static void Draw(class MeshComponent& meshComponent);
		static void RenderLight(class LightComponent& lightComponent);
		static void EndContext();
		static void Reset();
		struct Render3DStats{
			static const uint32_t MaxMeshes =1000;
			static uint32_t DrawCalls;
			static uint32_t NumberOfInstances;
			static uint32_t AmountDrawn;
		};
	private:
		static void SetMeshComponentData(class MeshComponent& meshComponent);
		static void LightingErrorChecks();
	};

	struct Proof_API InstancedRenderer3D {
		Count<class VertexBuffer> m_VertexBuffer;
		Count<class Shader>m_Shader;
		std::unordered_map<uint32_t,uint32_t> m_AmountMeshes;
		std::unordered_map<uint32_t,class MeshComponent> m_Meshes;
		std::unordered_map<uint32_t,uint32_t > m_MeshesEndingPositionIndexTransforms;
		bool SceneHasAmountMeshes(uint32_t ID) { return m_AmountMeshes.find(ID) != m_AmountMeshes.end(); };
		static  Count<class Texture2D>m_WhiteTexture;
	};


}