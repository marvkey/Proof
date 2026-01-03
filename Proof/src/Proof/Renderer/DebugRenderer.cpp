#include "Proofprch.h"
#include "DebugRenderer.h"
#include "Proof/Scene/Mesh.h"
#include "Proof/Renderer/WorldRenderer.h"
namespace Proof
{
	void DebugRenderer::DrawLine(const glm::vec3& p0, const glm::vec3& p1, const glm::vec4& color)
	{
		m_RenderQueue.emplace_back([p0, p1, color](Count<Renderer2D> renderer)
			{
				renderer->DrawLine(p0, p1, color);
			});
	}
	void DebugRenderer::DrawRay(const glm::vec3& origin, const glm::vec3& direction, float length, const glm::vec4& color)
	{
		m_RenderQueue.emplace_back([origin, direction, length,color](Count<Renderer2D> renderer)
			{
				renderer->DrawRay(origin, direction, length, color);
			});
	}
	void DebugRenderer::DrawRay(const glm::vec3& origin, const glm::vec3& direction, const glm::vec4& color)
	{
		m_RenderQueue.emplace_back([origin, direction, color](Count<Renderer2D> renderer)
			{
				renderer->DrawRay(origin, direction, color);
			});
	}

	void ImmediateRenderer::SubmitMesh(Count<Mesh> mesh, Count<RenderMaterial> renderMaterial,const glm::mat4& transform, bool CastShadowws)
	{
		m_RenderQueue.emplace_back([mesh,renderMaterial,transform,CastShadowws](Count<WorldRenderer> renderer)
		{
			renderer->SubmitMesh(mesh, renderMaterial, transform, CastShadowws);
		});
	}
	void ImmediateRenderer::SubmitDynamicMesh(Count<DynamicMesh> mesh, Count<RenderMaterial> material,uint32_t subMeshIndex, const glm::mat4& transform, bool CastShadowws,const std::vector<glm::mat4>& boneTransforms)
	{
		m_RenderQueue.emplace_back([mesh, material, subMeshIndex,transform, CastShadowws,boneTransforms](Count<WorldRenderer> renderer)
			{
				renderer->SubmitDynamicMesh(mesh, material, subMeshIndex,transform, CastShadowws,boneTransforms);
			});
	}
}
