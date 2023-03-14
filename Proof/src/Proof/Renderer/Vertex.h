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
        Vertex(Vector vertices ={0}, Vector normal ={0}, Vector2 texcorrds ={0,0}, Vector tangent = {0}, Vector bitangent = {0}) {
            Vertices = vertices;
            Normal = normal;
            TexCoords = texcorrds;
            Tangent = tangent;
            Bitangent = bitangent;
        }
        Vector Vertices{0,0,0};
        Vector Normal{0,0,0};
        Vector2 TexCoords{0,0};
        Vector Tangent{0,0,0};
        Vector Bitangent{0,0,0};
    }; 

    struct Vertex2D {
        Vector Position;
        glm::vec4 Color;
        glm::vec2 TexCoords;
        //float TexSlot;
    };

    struct CameraData {
        CameraData() {};
        CameraData(const glm::mat4& projection, const glm::mat4& view, const Vector& pos) :
            m_Projection(projection), m_View(view), m_Positon(pos) {
        };
        glm::mat4 m_Projection;
        glm::mat4 m_View;
        Vector m_Positon;
    };

    struct MeshInstance {
        Count<class Mesh> Mesh;
        //number of instance count
        uint32_t Count;
    };

    struct MaterialMeshInstance
    {
        Count<class Mesh> Mesh;
              //number of instance count
        Count<class MaterialTable> MaterialTables;
        uint32_t Count;
    };
}