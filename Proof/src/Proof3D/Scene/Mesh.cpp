#include "Proofprch.h"
#include "Mesh.h"
#include "Proof/Renderer/Texture.h"
#include "Proof/Renderer/Shader.h"
#include "Proof/Renderer/RendererCommand.h"
#include <unordered_map>

#include "Component.h"


namespace Proof{

    Mesh::Mesh(std::vector<Vertex>& Vertices,std::vector<uint32_t>& Indices,std::vector<Proof::Count<Proof::Texture2D>>& Textures) {
        this->m_Vertices = Vertices;
        this->m_Indices = Indices;
        this->m_Textures = Textures;
        SetupMesh();
        InitilizeMapTextures();
    }

    Mesh::Mesh(std::vector<Vertex>& Vertices,std::vector<uint32_t>& Indices) {
        this->m_Vertices = Vertices;
        this->m_Indices = Indices;
        SetupMesh();
    }

    void Mesh::InitilizeMapTextures() {
        unsigned int diffuseNr = 1;
        unsigned int specularNr = 1;
        unsigned int normalNr = 1;
        unsigned int heightNr = 1;
        for (unsigned int i = 0; i < m_Textures.size(); i++) {
            Proof::Texture2D::TextureType Texture_Type = m_Textures[i]->GetTextureType();
            if (Texture_Type == Proof::Texture2D::TextureType::Diffuse) {
                diffuseNr++;
                m_MapTextures.insert({"texture_diffuse",diffuseNr});
            }
            else if (Texture_Type == Proof::Texture2D::TextureType::Specular) {
                specularNr++;
                m_MapTextures.insert({"texture_specular",specularNr});
            }
            else if (Texture_Type == Proof::Texture2D::TextureType::Normal) {
                normalNr++;
                m_MapTextures.insert({"texture_normal",normalNr});
            }
            else if (Texture_Type == Proof::Texture2D::TextureType::Height) {
                heightNr++;
                m_MapTextures.insert({"texture_height",heightNr});
            }
        }
    }

    void Mesh::Draw(const Proof::Count<Proof::Shader>& shader) {
        shader->UseShader();
        shader->SetInt("texture_diffuse",0);
        m_Textures[0]->BindTexture(0);

        //uint8_t iterator = 0;
        /*
        for (const auto& element : m_MapTextures) {
            shader->SetInt(element.first,element.second);
            m_Textures[iterator]->BindTexture(iterator);
            iterator++;
        }
        */

        //InitMatrix();
        //shader->SetMat4("Model",ModelMatrix);
        //Proof::RendererCommand::DrawIndexed(VertexArrayObject);
        //VertexArrayObject->UnBind();
    }

    void Mesh::SetUpTransform() {
        
    }

    void Mesh::SetupMesh() {
        VertexArrayObject = Proof::VertexArray::Create();
        VertexBufferObject = Proof::VertexBuffer::Create(&m_Vertices[0],m_Vertices.size() * sizeof(Vertex));
        IndexBufferObject = Proof::IndexBuffer::Create(&m_Indices[0],m_Indices.size());
        VertexBufferObject->Bind();
        IndexBufferObject->Bind();
        VertexArrayObject->AttachIndexBuffer(IndexBufferObject);
        VertexArrayObject->AddData(0,3,sizeof(Vertex),(void*)offsetof(Vertex,Vertices));
        VertexArrayObject->AddData(1,3,sizeof(Vertex),(void*)offsetof(Vertex,Normal));
        VertexArrayObject->AddData(2,2,sizeof(Vertex),(void*)offsetof(Vertex,TexCoords));
        //VertexArrayObject->AddData(3,3,sizeof(Vertex),(void*)offsetof(Vertex,Tangent));
        //VertexArrayObject->AddData(4,3,sizeof(Vertex),(void*)offsetof(Vertex,Bitangent));
        VertexArrayObject->UnBind();
    }

    void Mesh::InitMatrix() {
        /*
        ModelMatrix = glm::mat4(1.0f);
        ModelMatrix = glm::translate(ModelMatrix,{MeshTransform.Location});
        ModelMatrix = glm::rotate(ModelMatrix,glm::radians(MeshTransform.Rotation.X),{MeshTransform.Rotation});
        ModelMatrix = glm::rotate(ModelMatrix,glm::radians(MeshTransform.Rotation.Y),{MeshTransform.Rotation});
        ModelMatrix = glm::rotate(ModelMatrix,glm::radians(MeshTransform.Rotation.Z),{MeshTransform.Rotation});
        ModelMatrix = glm::scale(ModelMatrix,{MeshTransform.Scale});
        */
    }
}