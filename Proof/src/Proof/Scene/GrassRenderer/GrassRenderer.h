#pragma once
#include "Proof/Core/Core.h"
#include "Proof/Utils/MultiUse.h"
namespace Proof
{
    constexpr static unsigned int NUM_BLADES = 1 << 13;
    constexpr static float MIN_HEIGHT = 1.3f;
    constexpr static float MAX_HEIGHT = 2.5f;
    constexpr static float MIN_WIDTH = 0.1f;
    constexpr static float MAX_WIDTH = 0.14f;
    constexpr static float MIN_BEND = 7.0f;
    constexpr static float MAX_BEND = 13.0f;

    struct GrassBladeDefaultSettings
    {
        float MinHeight = MIN_HEIGHT;
        float MaxHeight = MAX_HEIGHT;

        float MinWidth = MIN_WIDTH;
        float MaxWidth = MAX_WIDTH;

        float MinBend = MIN_BEND;
        float MaxBend = MAX_BEND;

    };

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
        UBGrassBlade() = default;

        UBGrassBlade(glm::vec3 position, GrassBladeDefaultSettings settings = GrassBladeDefaultSettings());

    };

    struct GrassBladeDrawIndirect 
    {
        uint32_t VertexCount;
        uint32_t InstanceCount;
        uint32_t FirstVertex;
        uint32_t FirstInstance;
    };
  
    struct GrassBladePlane : public RefCounted
    {
        GrassBladePlane(float planeSize = 15.0f, uint32_t numBlades = NUM_BLADES);
        GrassBladePlane(const std::vector<UBGrassBlade>& blades);
        Count< class StorageBufferSet>  BladesBuffer;
        Count< class StorageBufferSet>  CulledBladesBuffer;
        Count< class StorageBufferSet>  NumBladesBuffer;

        float GetPlaneSize() { return m_PlaneSize;};
        uint32_t GetNumBlades() { return m_NumBlades; }
        UUID GetUUID() { return m_UUID; }
    private:
        UUID m_UUID = UUID();
        float m_PlaneSize;
        uint32_t m_NumBlades;

        friend class GrassRenderer;
    };

	class GrassRenderer : public RefCounted
	{
	public:
		GrassRenderer(Count<class WorldRenderer> worldRendere);
		void Update(float deltaTime);
        void Render(Count<class WorldRenderer> renderer);

        Count<class RenderPass> m_GrassRenderPass;
        Count<class ComputePass> m_GrassGenerator;

        Count<class WorldRenderer> m_WorldRenderer;
	};
}