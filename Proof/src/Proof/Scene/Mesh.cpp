#include "Proofprch.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Component.h"
#include "Proof/Renderer/Texture.h"
#include "Proof/Renderer/Shader.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <string>
#include <fstream>
#include <sstream>
#include "Mesh.h"
#include "Proof/Renderer/3DRenderer/Renderer3DPBR.h"
#include "Proof/Math/Vector.h"
#include "Proof/Renderer/RendererBase.h"
namespace Proof{
    void Mesh::LoadModel(std::string const& path ) {
        PF_PROFILE_FUNC();

        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(path,aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
        //importer.ReadFileFromMemory(); pass a string to read file data
        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
        {
            PF_ENGINE_WARN("ERROR::ASSIMP {}", importer.GetErrorString());
            PF_WARN("ERROR::ASSIMP {}",importer.GetErrorString());
            return;
        }
        ProcessNode(scene->mRootNode,scene);
    }
    void Mesh::ProcessNode(void* node, const void* scene) {
        aiNode* ainode = (aiNode*)node;
        aiScene* aiscene = (aiScene*)scene;
        for (unsigned int i = 0; i < ainode->mNumMeshes; i++) {
            aiMesh* mesh = aiscene->mMeshes[ainode->mMeshes[i]];
            meshes.emplace_back(ProcessMesh(mesh, aiscene));
        }
        for (unsigned int i = 0; i < ainode->mNumChildren; i++) {
            ProcessNode(ainode->mChildren[i], aiscene);
        }
    }
    SubMesh Mesh::ProcessMesh(void* mesh,const void* scene) {
        aiMesh* aimesh = (aiMesh*)mesh;
        aiScene* aiscene = (aiScene*)scene;
        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;
        std::vector<Count<Texture2D>> textures;

        for (unsigned int i = 0; i < aimesh->mNumVertices; i++) {
            Vertex vertex;
            vertex.Vertices = Vector(aimesh->mVertices[i].x, aimesh->mVertices[i].y, aimesh->mVertices[i].z);
            if (aimesh->HasNormals())
                vertex.Normal = Vector(aimesh->mNormals[i].x,aimesh->mNormals[i].y,aimesh->mNormals[i].z);
            if (aimesh->mTextureCoords[0]) {
                vertex.TexCoords = glm::vec2(aimesh->mTextureCoords[0][i].x,aimesh->mTextureCoords[0][i].y);
                vertex.Tangent = Vector(aimesh->mTangents[i].x,aimesh->mTangents[i].y,aimesh->mTangents[i].z);
                vertex.Bitangent = Vector(aimesh->mBitangents[i].x,aimesh->mBitangents[i].y,aimesh->mBitangents[i].z);
            }
            else
                vertex.TexCoords = glm::vec2(0.0f,0.0f);
            vertices.emplace_back(vertex);
        }
        for (unsigned int i = 0; i <aimesh->mNumFaces; i++) {
            aiFace& face =aimesh->mFaces[i];
            for (unsigned int j = 0; j < face.mNumIndices; j++) {
                indices.emplace_back(face.mIndices[j]);
            }
        }
        aiMaterial* material = aiscene->mMaterials[aimesh->mMaterialIndex];
        //if (Renderer::GetAPI() == RendererAPI::API::Vulkan)
        //    return SubMesh(vertices, indices, aimesh->mName.C_Str());
        auto texture = LoadMaterialTextures(material,aiTextureType_DIFFUSE,Texture2D::TextureType::Diffuse);
      //  textures.insert(textures.end(),diffuseMaps.begin(),diffuseMaps.end());

        //std::vector<Count<Texture2D>>  specularMaps = LoadMaterialTextures(material,aiTextureType_SPECULAR,Texture2D::TextureType::Specular);
        //textures.insert(textures.end(),specularMaps.begin(),specularMaps.end());
        //
        //std::vector<Count<Texture2D>> normalMaps = LoadMaterialTextures(material,aiTextureType_NORMALS,Texture2D::TextureType::Normal);
        //textures.insert(textures.end(),normalMaps.begin(),normalMaps.end());
        //
        //std::vector<Count<Texture2D>>  heightMaps = LoadMaterialTextures(material,aiTextureType_HEIGHT,Texture2D::TextureType::Height);
        //textures.insert(textures.end(),heightMaps.begin(),heightMaps.end());
       
        SubMesh temp(vertices,indices,aimesh->mName.C_Str(), texture);
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

    std::vector<uint32_t> Mesh::LoadMaterialTextures(void* mat,int type,Texture2D::TextureType _TextureType) {
        aiMaterial* aimat = (aiMaterial*)mat;
        aiTextureType aitype = (aiTextureType)type;
        std::vector<uint32_t> textures;
        for (unsigned int i = 0; i < aimat->GetTextureCount(aitype); i++) {
            aiString str;
            aimat->GetTexture(aitype,i,&str);
            std::string other = str.C_Str();
            bool addTexture = true;
            for (unsigned int j = 0; j < textures_loaded.size(); j++) {
                if (textures_loaded[j]->GetPath() == other) {
                    textures.emplace_back(j);
                    addTexture = false;
                    break;
                }
            }
            if (addTexture == true) {
                Count<Texture2D> NewTexture = Texture2D::Create(str.C_Str(),_TextureType);
                textures_loaded.emplace_back(NewTexture);
                textures.emplace_back(textures_loaded.size()-1);
            }
        }
        return textures;
    }

    std::vector<Count<Texture2D>> Mesh::LoadMaterial(void* mat) {
        aiMaterial* aimat = (aiMaterial*)mat;
        Material material;
        aiColor3D color(0.f,0.f,0.f);
        float shininess;

        aimat->Get(AI_MATKEY_COLOR_AMBIENT,color);
        material.m_Colour = glm::vec3(color.r,color.b,color.g);

        aimat->Get(AI_MATKEY_SHININESS,shininess);
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

    std::vector<std::pair<std::string, Texture2D::TextureType>> Mesh::LoadMaterialTexturesTest(void* mat, int type, Texture2D::TextureType _TextureType) {
       /*
        aiMaterial* aimat = (aiMaterial*)mat;
        aiTextureType aitype = (aiTextureType)type;
        std::vector<std::pair<std::string, Texture2D::TextureType>>  Textures;
        for (unsigned int i = 0; i < aimat->GetTextureCount(aitype); i++) {
            aiString str;
            aimat->GetTexture(aitype, i, &str);
            bool skip = false;
            for (unsigned int j = 0; j < textures_loaded.size(); j++) {
                if (std::strcmp(textures_loaded[j]->GetPath().data(), str.C_Str()) == 0) {
                    Textures.emplace_back(textures_loaded[j]);
                    skip = true;
                    break;
                }
            }
            if (!skip) {
                std::pair<std::string, Texture2D::TextureType> NewTexture = { str.C_Str(), _TextureType };
                Textures.emplace_back(NewTexture);
                textures_loaded.emplace_back(NewTexture);
            }
        }
        return Textures;
        */
        return {};
    }


    SubMesh::SubMesh(std::vector<Vertex>& Vertices,std::vector<uint32_t>& Indices,const std::string& name, std::vector<uint32_t> diffuseIndex) {
        this->m_Vertices = Vertices;
        this->m_Indices = Indices;

        m_DiffuseIndex = diffuseIndex;
        m_Name = name;
        SetUp();
    }

    SubMesh::SubMesh(std::vector<Vertex>& Vertices, std::vector<uint32_t>& Indices, const std::string& name) {
        this->m_Vertices = Vertices;
        this->m_Indices = Indices;
        m_Name = name;
      
        SetUp();
    }

    void SubMesh::SetUp() {
        m_VertexBufferObject = VertexBuffer::Create(m_Vertices.data(), m_Vertices.size() * sizeof(Vertex));
        m_IndexBufferObject = IndexBuffer::Create(m_Indices.data(), m_Indices.size());
    }
   
}