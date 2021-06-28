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

		static void Draw(MeshComponent& meshComponent);
		static void EndContext();
		struct Data{
			static const uint32_t MaxMeshes =1000;
		};
	};

	struct Proof_API InstancedRenderer3D {
		Count<class VertexArray> m_VertexArray;
		Count<class VertexBuffer> m_VertexBuffer;
		Count<class Shader>m_Shader;
		std::unordered_map<uint32_t,uint32_t> m_AmountMeshes;
		std::unordered_map<uint32_t,MeshComponent> m_Meshes;
		bool SceneHasAmountMeshes(uint32_t ID) { return m_AmountMeshes.find(ID) != m_AmountMeshes.end(); };
	};
}