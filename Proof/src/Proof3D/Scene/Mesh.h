#pragma once
#include "Proof/Core/Core.h"
#include "Proof3D/Math/Vector.h"
#include <vector>
#include <unordered_map>
#include "Vertex.h"
#include "Component.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
namespace Proof{

    class Mesh {
    public:
        
        Mesh(std::vector<Vertex>& Vertices,std::vector<uint32_t>& Indices,std::vector<Proof::Count<class Texture2D>>& Textures);
        Mesh(std::vector<Vertex>& Vertices,std::vector<uint32_t>& Indices);
        void Draw(const Count<class Shader>& shader);
        std::vector<Vertex> m_Vertices;
        std::vector<uint32_t> m_Indices;
        void SetUpTransform();
        void SetupMesh();
        TransformComponent MeshTransform;
        TagComponent MeshTags;
    private:
        Count<class VertexArray> VertexArrayObject;
        Count<class VertexBuffer> VertexBufferObject;
        Count<class IndexBuffer> IndexBufferObject;
        glm::mat4 ModelMatrix = glm::mat4(1.0f);
        void InitMatrix();
        std::vector<Count<class Texture2D>>m_Textures;
        std::unordered_map<std::string,uint8_t>m_MapTextures;
        void InitilizeMapTextures();
        friend class Renderer3D;
    };
}