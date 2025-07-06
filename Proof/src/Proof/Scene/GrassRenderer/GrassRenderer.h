#pragma once
#include "Proof/Core/Core.h"
#include "Proof/Utils/MultiUse.h"
namespace Proof
{
    struct UBGrassBlade 
    {
        // Base of the blade (root position on the surface)
        // v0.xyz = base position (world space)
        // v0.w   = direction angle (rotation around up vector)
        glm::vec4 V0;

        // Control point for curvature (middle of the Bézier curve)
        // v1.xyz = control point position
        // v1.w   = blade height
        glm::vec4 V1;

        // Tip of the blade (moved by physics simulation)
        // v2.xyz = tip position (world space)
        // v2.w   = blade width
        glm::vec4 V2;

        // Blade's up direction and bend stiffness
        // up.xyz = up vector (usually terrain normal)
        // up.w   = stiffness coefficient (resistance to bending)
        glm::vec4 Up;
    };

    struct GrassBladeDrawIndirect 
    {
        uint32_t VertexCount;
        uint32_t InstanceCount;
        uint32_t FirstVertex;
        uint32_t FirstInstance;
    };
    constexpr static unsigned int NUM_BLADES = 1 << 13;
    constexpr static float MIN_HEIGHT = 1.3f;
    constexpr static float MAX_HEIGHT = 2.5f;
    constexpr static float MIN_WIDTH = 0.1f;
    constexpr static float MAX_WIDTH = 0.14f;
    constexpr static float MIN_BEND = 7.0f;
    constexpr static float MAX_BEND = 13.0f;


    struct GrassBladePlane
    {
        GrassBladePlane(float planeSize = 100.0f, uint32_t numBlades = NUM_BLADES);
        Count< class StorageBufferSet>  BladesBuffer;
        Count< class StorageBufferSet>  CulledBladesBuffer;
        Count< class StorageBufferSet>  NumBladesBuffer;

        float GetPlaneSize() { return m_PlaneSize;};
        uint32_t GetNumBlades() { return m_NumBlades; }
    private:
        float m_PlaneSize;
        uint32_t m_NumBlades;
    };
	class GrassRenderer : public RefCounted
	{
	public:
		GrassRenderer(Count<class WorldRenderer> worldRendere);
		void Update(float deltaTime, const glm::mat4& transform);
		void Render(Count<class WorldRenderer> renderer);

        Count<class RenderPass> m_GrassRenderPass;
        Count<class ComputePass> m_GrassGenerator;

        Count<class UniformBufferSet> m_UBModelMatrix;
        std::vector<GrassBladePlane> m_Planes;
        Count<class RenderCommandBuffer> m_RenderCommandBuffer;
        Count<class WorldRenderer> m_WorldRenderer;
	};
}