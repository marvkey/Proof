#include "Proofprch.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Component.h"
#include "Proof/Renderer/Texture.h"
#include "Proof/Renderer/Shader.h"
#include "Proof/Renderer/RendererCommand.h"

#include <string>
#include <fstream>
#include <sstream>
#include "Mesh.h"
#include "Proof/Renderer/3DRenderer/Renderer3DPBR.h"
#include "Proof/Resources/Math/Vector.h"


namespace Proof{
    void Mesh::LoadModel(std::string const& path) {
        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(path,aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
        {
            PF_WARN("ERROR::ASSIMP %s",importer.GetErrorString());
            return;
        }
        ProcessNode(scene->mRootNode,scene);
    }
    void Mesh::ProcessNode(aiNode* node,const aiScene* scene) {
        for (unsigned int i = 0; i < node->mNumMeshes; i++) {
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            //PF_ENGINE_INFO("%s",mesh->mName.C_Str()); // works
            meshes.emplace_back(ProcessMesh(mesh,scene));
        }
        for (unsigned int i = 0; i < node->mNumChildren; i++) {
            ProcessNode(node->mChildren[i],scene);
        }
    }
    SubMesh Mesh::ProcessMesh(aiMesh* mesh,const aiScene* scene) {
        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;
        std::vector<Count<Texture2D>> textures;

        for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
            Vertex vertex;
            vertex.Vertices = Vector(mesh->mVertices[i].x,mesh->mVertices[i].y,mesh->mVertices[i].z);
            if (mesh->HasNormals())
                vertex.Normal = Vector(mesh->mNormals[i].x,mesh->mNormals[i].y,mesh->mNormals[i].z);
            if (mesh->mTextureCoords[0]) {
                vertex.TexCoords = glm::vec2(mesh->mTextureCoords[0][i].x,mesh->mTextureCoords[0][i].y);
                vertex.Tangent = Vector(mesh->mTangents[i].x,mesh->mTangents[i].y,mesh->mTangents[i].z);
                vertex.Bitangent = Vector(mesh->mBitangents[i].x,mesh->mBitangents[i].y,mesh->mBitangents[i].z);
            }
            else
                vertex.TexCoords = glm::vec2(0.0f,0.0f);
            vertices.emplace_back(vertex);
        }
        for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
            aiFace& face = mesh->mFaces[i];
            for (unsigned int j = 0; j < face.mNumIndices; j++) {
                indices.emplace_back(face.mIndices[j]);
            }
        }
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

        std::vector<Count<Texture2D>>  diffuseMaps = LoadMaterialTextures(material,aiTextureType_DIFFUSE,Texture2D::TextureType::Diffuse);
        textures.insert(textures.end(),diffuseMaps.begin(),diffuseMaps.end());

        std::vector<Count<Texture2D>>  specularMaps = LoadMaterialTextures(material,aiTextureType_SPECULAR,Texture2D::TextureType::Specular);
        textures.insert(textures.end(),specularMaps.begin(),specularMaps.end());

        std::vector<Count<Texture2D>> normalMaps = LoadMaterialTextures(material,aiTextureType_NORMALS,Texture2D::TextureType::Normal);
        textures.insert(textures.end(),normalMaps.begin(),normalMaps.end());

        std::vector<Count<Texture2D>>  heightMaps = LoadMaterialTextures(material,aiTextureType_HEIGHT,Texture2D::TextureType::Height);
        textures.insert(textures.end(),heightMaps.begin(),heightMaps.end());
       
        SubMesh temp(vertices,indices,mesh->mName.C_Str(),textures);
        return temp;
        //aiTextureType_METALNESS
        //aiTextureType_DIFFUSE_ROUGHNESS
        //aiTextureType_AMBIENT_OCCLUSION AO
        //aiTextureType_NORMALS

        //aiTextureType_DIFFUSE
        //aiTextureType_SPECULAR

        // What we are gonna support
        //aiTextureType_DIFFUSE
        //aiTextureType_NORMALS
        //aiTextureType_SPECULAR
    }

    std::vector<Count<Texture2D>> Mesh::LoadMaterialTextures(aiMaterial* mat,aiTextureType type,Texture2D::TextureType _TextureType) {
        std::vector<Count<Texture2D>>  Textures;
        for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
            aiString str;
            mat->GetTexture(type,i,&str);
            bool skip = false;
            for (unsigned int j = 0; j < textures_loaded.size(); j++) {
                if (std::strcmp(textures_loaded[j]->GetPath().data(),str.C_Str()) == 0) {
                    Textures.emplace_back(textures_loaded[j]);
                    skip = true;
                    break;
                }
            }
            if (!skip) {
                Count<Texture2D> NewTexture = Texture2D::Create(str.C_Str(),_TextureType);
                Textures.emplace_back(NewTexture);
                textures_loaded.emplace_back(NewTexture);
            }
        }
        return Textures;
    }

    std::vector<Count<Texture2D>> Mesh::LoadMaterial(aiMaterial* mat) {
        Material material;
        aiColor3D color(0.f,0.f,0.f);
        float shininess;

        mat->Get(AI_MATKEY_COLOR_AMBIENT,color);
        material.m_Colour = glm::vec3(color.r,color.b,color.g);

        mat->Get(AI_MATKEY_SHININESS,shininess);
        material.m_Metallness = shininess;

        /*
        mat->Get(AI_MATKEY_COLOR_AMBIENT,color);
        material.Ambient = glm::vec3(color.r,color.b,color.g);

        mat->Get(AI_MATKEY_COLOR_SPECULAR,color);
        material.Specular = glm::vec3(color.r,color.b,color.g);

        mat->Get(AI_MATKEY_SHININESS,shininess);
        material.Shininess = shininess;
        */
        return std::vector<Count<Texture2D>>();
    }


    SubMesh::SubMesh(std::vector<Vertex>& Vertices,std::vector<uint32_t>& Indices,const std::string& name,std::vector<Proof::Count<Proof::Texture2D>>& Textures) {
        //this->m_Vertices = Vertices;
        //this->m_Indices = Indices;
        this->m_Textures = Textures;
        m_Name = name;
        m_VertexArrayObject = VertexArray::Create();
        m_VertexBufferObject = VertexBuffer::Create(Vertices.data(), Vertices.size() * sizeof(Vertex));
        m_IndexBufferObject = IndexBuffer::Create(Indices.data(), Indices.size());
        m_VertexBufferObject->Bind();
        m_IndexBufferObject->Bind();
        m_VertexArrayObject->AttachIndexBuffer(m_IndexBufferObject);
        m_VertexArrayObject->Bind();

        m_VertexArrayObject->AddData(0, 3, sizeof(Vertex), (void*)offsetof(Vertex, Vertex::Vertices));
        m_VertexArrayObject->AddData(1, 3, sizeof(Vertex), (void*)offsetof(Vertex, Vertex::Normal));
        m_VertexArrayObject->AddData(2, 2, sizeof(Vertex), (void*)offsetof(Vertex, Vertex::TexCoords));
        m_VertexArrayObject->AddData(3, 3, sizeof(Vertex), (void*)offsetof(Vertex, Vertex::Tangent));
        m_VertexArrayObject->AddData(4, 3, sizeof(Vertex), (void*)offsetof(Vertex, Vertex::Bitangent));
        m_VertexArrayObject->UnBind();

        Renderer3DPBR::GetRenderer()->m_VertexBuffer->Bind(); // we are gonna have to find a way to deal with this 
        m_VertexArrayObject->AddData(5, 4, sizeof(PhysicalBasedRendererVertex), (void*)0);
        m_VertexArrayObject->AddData(6, 4, sizeof(PhysicalBasedRendererVertex), (void*)(sizeof(glm::vec4) * 1));
        m_VertexArrayObject->AddData(7, 4, sizeof(PhysicalBasedRendererVertex), (void*)(sizeof(glm::vec4) * 2));
        m_VertexArrayObject->AddData(8, 4, sizeof(PhysicalBasedRendererVertex), (void*)(sizeof(glm::vec4) * 3));
        m_VertexArrayObject->AddData(9, 3, sizeof(PhysicalBasedRendererVertex), (void*)offsetof(PhysicalBasedRendererVertex, PhysicalBasedRendererVertex::m_AlbedoColour));
        m_VertexArrayObject->AddData(10, 1, sizeof(PhysicalBasedRendererVertex), (void*)offsetof(PhysicalBasedRendererVertex, PhysicalBasedRendererVertex::m_Matallness));
        m_VertexArrayObject->AddData(11, 1, sizeof(PhysicalBasedRendererVertex), (void*)offsetof(PhysicalBasedRendererVertex, PhysicalBasedRendererVertex::m_Roughnes));
        m_VertexArrayObject->AddData(12, 1, sizeof(PhysicalBasedRendererVertex), (void*)offsetof(PhysicalBasedRendererVertex, PhysicalBasedRendererVertex::m_AO));
        m_VertexArrayObject->AddData(13, 1, sizeof(PhysicalBasedRendererVertex), (void*)offsetof(PhysicalBasedRendererVertex, PhysicalBasedRendererVertex::m_UsingMaterial));
        m_VertexArrayObject->AttributeDivisor(5, 1);
        m_VertexArrayObject->AttributeDivisor(6, 1);
        m_VertexArrayObject->AttributeDivisor(7, 1);
        m_VertexArrayObject->AttributeDivisor(8, 1);
        m_VertexArrayObject->AttributeDivisor(9, 1);// Material
        m_VertexArrayObject->AttributeDivisor(10, 1);// Material
        m_VertexArrayObject->AttributeDivisor(11, 1);// MaterialMaterial
        m_VertexArrayObject->AttributeDivisor(12, 1);// MaterialMaterial
        m_VertexArrayObject->AttributeDivisor(13, 1);// MaterialMaterial
        m_VertexArrayObject->UnBind();
    }

    SubMesh::SubMesh(std::vector<Vertex>& Vertices,std::vector<uint32_t>& Indices,const std::string& name) {
        //this->m_Vertices = Vertices;
        //this->m_Indices = Indices;
        m_Name = name;
       // SetUp();
        m_VertexArrayObject = VertexArray::Create();
        m_VertexBufferObject = VertexBuffer::Create(Vertices.data(), Vertices.size() * sizeof(Vertex));
        m_IndexBufferObject = IndexBuffer::Create(Indices.data(), Indices.size());
        m_VertexBufferObject->Bind();
        m_IndexBufferObject->Bind();
        m_VertexArrayObject->AttachIndexBuffer(m_IndexBufferObject);
        m_VertexArrayObject->Bind();

        m_VertexArrayObject->AddData(0, 3, sizeof(Vertex), (void*)offsetof(Vertex, Vertex::Vertices));
        m_VertexArrayObject->AddData(1, 3, sizeof(Vertex), (void*)offsetof(Vertex, Vertex::Normal));
        m_VertexArrayObject->AddData(2, 2, sizeof(Vertex), (void*)offsetof(Vertex, Vertex::TexCoords));
        m_VertexArrayObject->AddData(3, 3, sizeof(Vertex), (void*)offsetof(Vertex, Vertex::Tangent));
        m_VertexArrayObject->AddData(4, 3, sizeof(Vertex), (void*)offsetof(Vertex, Vertex::Bitangent));
        m_VertexArrayObject->UnBind();

        Renderer3DPBR::GetRenderer()->m_VertexBuffer->Bind(); // we are gonna have to find a way to deal with this 
        m_VertexArrayObject->AddData(5, 4, sizeof(PhysicalBasedRendererVertex), (void*)0);
        m_VertexArrayObject->AddData(6, 4, sizeof(PhysicalBasedRendererVertex), (void*)(sizeof(glm::vec4) * 1));
        m_VertexArrayObject->AddData(7, 4, sizeof(PhysicalBasedRendererVertex), (void*)(sizeof(glm::vec4) * 2));
        m_VertexArrayObject->AddData(8, 4, sizeof(PhysicalBasedRendererVertex), (void*)(sizeof(glm::vec4) * 3));
        m_VertexArrayObject->AddData(9, 3, sizeof(PhysicalBasedRendererVertex), (void*)offsetof(PhysicalBasedRendererVertex, PhysicalBasedRendererVertex::m_AlbedoColour));
        m_VertexArrayObject->AddData(10, 1, sizeof(PhysicalBasedRendererVertex), (void*)offsetof(PhysicalBasedRendererVertex, PhysicalBasedRendererVertex::m_Matallness));
        m_VertexArrayObject->AddData(11, 1, sizeof(PhysicalBasedRendererVertex), (void*)offsetof(PhysicalBasedRendererVertex, PhysicalBasedRendererVertex::m_Roughnes));
        m_VertexArrayObject->AddData(12, 1, sizeof(PhysicalBasedRendererVertex), (void*)offsetof(PhysicalBasedRendererVertex, PhysicalBasedRendererVertex::m_AO));
        m_VertexArrayObject->AddData(13, 1, sizeof(PhysicalBasedRendererVertex), (void*)offsetof(PhysicalBasedRendererVertex, PhysicalBasedRendererVertex::m_UsingMaterial));
        m_VertexArrayObject->AttributeDivisor(5, 1);
        m_VertexArrayObject->AttributeDivisor(6, 1);
        m_VertexArrayObject->AttributeDivisor(7, 1);
        m_VertexArrayObject->AttributeDivisor(8, 1);
        m_VertexArrayObject->AttributeDivisor(9, 1);// Material
        m_VertexArrayObject->AttributeDivisor(10, 1);// Material
        m_VertexArrayObject->AttributeDivisor(11, 1);// MaterialMaterial
        m_VertexArrayObject->AttributeDivisor(12, 1);// MaterialMaterial
        m_VertexArrayObject->AttributeDivisor(13, 1);// MaterialMaterial
        m_VertexArrayObject->UnBind();
      //  m_Vertices.clear();
      //  m_Indices.clear();
    }


    void SubMesh::SetUp() {

    }
}