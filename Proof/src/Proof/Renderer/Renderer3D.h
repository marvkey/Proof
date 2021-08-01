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
		static void BeginContext(const class PerspectiveCamera& Camera);
		static void BeginContext(glm::mat4 Projection,class EditorCamera3D& EditorCamera);

		static void BeginContext(const class OrthagraphicCamera& Camera);

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
	};

	struct Proof_API InstancedRenderer3D {
		Count<class VertexBuffer> m_VertexBuffer;
		Count<class Shader>m_Shader;
		std::unordered_map<uint32_t,uint32_t> m_AmountMeshes;
		std::unordered_map<uint32_t,class MeshComponent> m_Meshes;
		bool SceneHasAmountMeshes(uint32_t ID) { return m_AmountMeshes.find(ID) != m_AmountMeshes.end(); };
		static  Count<class Texture2D>m_WhiteTexture;
	};
}