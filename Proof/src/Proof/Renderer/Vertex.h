#pragma once
#include "Proof/Resources/Math/Math.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "Proof/Resources/Math/Vector.h"

namespace Proof
{
 
    // Rename MESH Vertex
    struct Vertex {
        Vertex(Vector<float>vertices ={0}, Vector<float>normal ={0}, glm::vec2 texcorrds ={0,0}, Vector<float>tangent = {0}, Vector<float>bitangent = {0}) {
            Vertices = vertices;
            Normal = normal;
            TexCoords = texcorrds;
            Tangent = tangent;
            Bitangent = bitangent;
        }
        Vector<float> Vertices{0,0,0};
        Vector<float> Normal{0,0,0};
        glm::vec2 TexCoords{0,0};
        Vector<float> Tangent{0,0,0};
        Vector<float> Bitangent{0,0,0};

        static class VulkanVertexInput GetVulkanDescription();
    };

    struct Vertex2D {
        Vector<float> Position;
        glm::vec4 Color;
        glm::vec2 TexCoords;
        float TexSlot;
    };
}