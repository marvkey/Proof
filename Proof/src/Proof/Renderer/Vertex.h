#pragma once
#include "Proof/Math/Math.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "Proof/Math/Vector.h"

namespace Proof
{

    struct TextParams
    {
        glm::vec4 Color{ 1.0f };
        // horizontal distnace between each character

        float Kerning = 0.0f;
        // spacing of new line 
        float LineSpacing = 1.0f;

        glm::vec4 OutlineColor = glm::vec4(0.0f,0.0f,0.0f,1.0f);
        float OutlineThickness = 0.0f;
    };
    // Rename MESH Vertex
    struct Vertex {
        glm::vec3 Position{0,0,0};
        glm::vec3 Normal{0,0,0};
        glm::vec2 TexCoord{0,0};
        glm::vec3 Tangent{0,0,0};
        glm::vec3 Bitangent{0,0,0};
    }; 

    struct AnimationVertex
    {
        glm::uvec4 BoneIndices = glm::uvec4(0);
        glm::vec4 BoneWieghts = glm::vec4(0.0f);
    };


    struct Index
    {
        uint32_t V1, V2, V3;
    };
    struct Vertex2D {
        Vector Position;
        glm::vec4 Color;
        glm::vec2 TexCoords;
        float TexSlot;
    };

    //primarily used for fullscreen quad
    struct QuadVertex
    {
        glm::vec3 Position;
        glm::vec2 TexCoord;
    };

    struct CameraData {
        glm::mat4 Projection;
        glm::mat4 ProjectionView;
        Vector Position;
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