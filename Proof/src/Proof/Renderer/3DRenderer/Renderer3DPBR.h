#pragma once
#include <unordered_map>
#include <array>
#include <vector>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "Renderer3DCore.h"
#include "Proof/Scene/Material.h"
namespace Proof
{
	class PhysicalBasedRenderer;
	class Renderer3DPBR {
	public:
		static void Init();
		static void BeginContext(class EditorCamera& editorCamera);
		static void BeginContext(const glm::mat4& projection,const glm::mat4& view,const Vector& Position);
		static void Draw(class MeshComponent& meshComponent);
		static void Draw(class LightComponent& lightComponent);
		static PhysicalBasedRenderer* GetRenderer();
		static void EndContext();
		static void Reset();
	private:
		static void RenderLight();
		static void RenderMesh();
		static void Render();
	};
	struct PhysicalBasedRendererVertex;
	struct Proof_API DeferedRendering {
		Count<class FrameBuffer> Gbuffer;
		Count<class Texture2D>GPosition;
		Count<class Texture2D>GNormal;
		Count<class Texture2D>GAlbedo;
		Count<class RenderBuffer>RenderBuffer;
		Count<class Shader>MeshShader;
		Count<class Shader>LightShader;
		DeferedRendering();
	};
	struct Proof_API PhysicalBasedRenderer {
		Count<class VertexBuffer> m_VertexBuffer;
		Count<class Shader> m_Shader;
		std::unordered_map<uint32_t,uint32_t> m_AmountMeshes;
		std::unordered_map<uint32_t,class MeshComponent> m_Meshes;
		std::unordered_map<uint32_t,uint32_t > m_MeshesEndingPositionIndexTransforms;
		std::vector<PhysicalBasedRendererVertex>m_Transforms;
		bool SceneHasAmountMeshes(uint32_t ID) { return m_AmountMeshes.find(ID) != m_AmountMeshes.end(); };
		static Count<class Texture2D>m_WhiteTexture;
		DeferedRendering m_DeferedRendering;
	};
	
	struct Proof_API PhysicalBaseRendererGuide{
	public:
		static const uint32_t s_MaxMesh = 1000;
	};

	struct Proof_API PhysicalBasedRendererVertex{
	
		PhysicalBasedRendererVertex(const glm::mat4& transform, const Material& temp,int usingMaterial):
		m_Transform(transform)
		{
			m_AlbedoColour =temp.m_Colour;
			m_AO= temp.m_AO;
			m_Roughnes =temp.m_Roughness;
			m_Matallness = temp.m_Metallness;
			m_UsingMaterial =usingMaterial;
		}
		glm::mat4 m_Transform;
		Vector m_AlbedoColour;
		float m_Matallness = 0.f;
		float m_Roughnes = 0.f;
		float m_AO = 0.f;
		int m_UsingMaterial =false;
	};

}
