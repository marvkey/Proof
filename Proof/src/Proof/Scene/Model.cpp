#include "Proofprch.h"
#include "Model.h"
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

namespace Proof{
    std::vector<uint32_t> Model::AllID;
    void Model::Draw(const Count<Shader>& shader) {
        for (Mesh& Mesh : meshes)
            Mesh.Draw(shader);
    }
    std::vector<Mesh> Model::GetMesh() const {
        return meshes;
    }
    void Model::LoadModelOneMesh() {
        
    }
    void Model::LoadModel(std::string const& path) {
        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(path,aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
        {
            std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
            return;
        }
        ProcessNode(scene->mRootNode,scene);
    }
    void Model::ProcessNode(aiNode* node,const aiScene* scene) {
        for (unsigned int i = 0; i < node->mNumMeshes; i++) {
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            meshes.emplace_back(ProcessMesh(mesh,scene));
        }
        for (unsigned int i = 0; i < node->mNumChildren; i++) {
            ProcessNode(node->mChildren[i],scene);
        }
    }
    Mesh Model::ProcessMesh(aiMesh* mesh,const aiScene* scene) {
        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;
        std::vector<Count<Texture2D>> textures;
        uint32_t StartINdexMesh = m_Vertices.size();
        Offset += 1;

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
            m_Vertices.emplace_back(vertex);
        }
        for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
            aiFace& face = mesh->mFaces[i];
            for (unsigned int j = 0; j < face.mNumIndices; j++) {
                indices.emplace_back(face.mIndices[j]);
                m_Indices.emplace_back(face.mIndices[j]);
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

        Mesh temp(vertices,indices,textures);
        temp.StartIndex = StartINdexMesh;
        return temp;
    }

    std::vector<Count<Texture2D>> Model::LoadMaterialTextures(aiMaterial* mat,aiTextureType type,Texture2D::TextureType _TextureType) {
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
    void Model::SetUpModel() {
        m_VertexArrayObject = VertexArray::Create();
        m_VertexBufferObject = VertexBuffer::Create(&m_Vertices[0],m_Vertices.size() * sizeof(Vertex));
        m_IndexBufferObject = IndexBuffer::Create(&m_Indices[0],m_Indices.size());
        m_VertexBufferObject->Bind();
        m_IndexBufferObject->Bind();
        m_VertexArrayObject->AttachIndexBuffer(m_IndexBufferObject);


        uint32_t Vertices1= offsetof(Vertex,Vertices);
        uint32_t Vertices2= offsetof(Vertex,TexCoords);
        uint32_t Vertices3= offsetof(Vertex,Normal);
        uint32_t Vertices4= offsetof(Vertex,Tangent);
        uint32_t Vertices5= offsetof(Vertex,Bitangent);

        m_VertexArrayObject->AddData(0,3,sizeof(Vertex),(void*)Vertices1);
        m_VertexArrayObject->AddData(1,2,sizeof(Vertex),(void*)Vertices2);
        m_VertexArrayObject->AddData(2,3,sizeof(Vertex),(void*)Vertices3);
        m_VertexArrayObject->AddData(3,3,sizeof(Vertex),(void*)Vertices4);
        m_VertexArrayObject->AddData(4,3,sizeof(Vertex),(void*)Vertices5);

        m_VertexArrayObject->UnBind();
    }
   
}