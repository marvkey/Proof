#pragma once
#include "Proof/Math/Math.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "Proof/Math/Vector.h"

namespace Proof
{
 
    // Rename MESH Vertex
    struct Vertex {
        Vertex(Vector vertices ={0}, Vector normal ={0}, glm::vec2 texcorrds ={0,0}, Vector tangent = {0}, Vector bitangent = {0}) {
            Vertices = vertices;
            Normal = normal;
            TexCoords = texcorrds;
            Tangent = tangent;
            Bitangent = bitangent;
        }
        Vector Vertices{0,0,0};
        Vector Normal{0,0,0};
        glm::vec2 TexCoords{0,0};
        Vector Tangent{0,0,0};
        Vector Bitangent{0,0,0};

        static class VulkanVertexInput GetVulkanDescription();
    };

    struct Vertex2D {
        Vector Position;
        glm::vec4 Color;
        glm::vec2 TexCoords;
        float TexSlot;
    };
}