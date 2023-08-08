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
#include "Material.h"
#include "Proof/Renderer/3DRenderer/Renderer3DPBR.h"
#include "Proof/Math/Vector.h"
#include "Proof/Renderer/Renderer.h"
#include "Proof/Renderer/Buffer.h"
namespace Proof{
    static glm::mat4 AIMatrixToGLM(const aiMatrix4x4& aiMatrix) {
        glm::mat4 glmMatrix;
        // glm::mat4 and aiMatrix4x4 are both column-major matrices,
        // so we can copy the values directly
        glmMatrix[0][0] = aiMatrix.a1;
        glmMatrix[1][0] = aiMatrix.a2;
        glmMatrix[2][0] = aiMatrix.a3;
        glmMatrix[3][0] = aiMatrix.a4;

        glmMatrix[0][1] = aiMatrix.b1;
        glmMatrix[1][1] = aiMatrix.b2;
        glmMatrix[2][1] = aiMatrix.b3;
        glmMatrix[3][1] = aiMatrix.b4;

        glmMatrix[0][2] = aiMatrix.c1;
        glmMatrix[1][2] = aiMatrix.c2;
        glmMatrix[2][2] = aiMatrix.c3;
        glmMatrix[3][2] = aiMatrix.c4;

        glmMatrix[0][3] = aiMatrix.d1;
        glmMatrix[1][3] = aiMatrix.d2;
        glmMatrix[2][3] = aiMatrix.d3;
        glmMatrix[3][3] = aiMatrix.d4;

        return glmMatrix;
    }
    static aiMatrix4x4 GLMToAIMatrix(const glm::mat4& glmMatrix) {
        aiMatrix4x4 aiMatrix;
        // glm::mat4 and aiMatrix4x4 are both column-major matrices,
        // so we can copy the values directly
        aiMatrix.a1 = glmMatrix[0][0];
        aiMatrix.a2 = glmMatrix[1][0];
        aiMatrix.a3 = glmMatrix[2][0];
        aiMatrix.a4 = glmMatrix[3][0];

        aiMatrix.b1 = glmMatrix[0][1];
        aiMatrix.b2 = glmMatrix[1][1];
        aiMatrix.b3 = glmMatrix[2][1];
        aiMatrix.b4 = glmMatrix[3][1];

        aiMatrix.c1 = glmMatrix[0][2];
        aiMatrix.c2 = glmMatrix[1][2];
        aiMatrix.c3 = glmMatrix[2][2];
        aiMatrix.c4 = glmMatrix[3][2];

        aiMatrix.d1 = glmMatrix[0][3];
        aiMatrix.d2 = glmMatrix[1][3];
        aiMatrix.d3 = glmMatrix[2][3];
        aiMatrix.d4 = glmMatrix[3][3];

        return aiMatrix;
    }
    MeshSource::MeshSource(const std::string & path)
    {
        PF_PROFILE_FUNC();
        m_Path = path;
        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
        {
            PF_EC_WARN("ERROR::ASSIMP {}", importer.GetErrorString());
            return;
        }
        PF_ENGINE_INFO("MeshSource:{}", path);
        ProcessNode(scene->mRootNode, scene, AIMatrixToGLM(scene->mRootNode->mTransformation));
       
        
        m_MaterialTable = Count<MaterialTable>::Create(scene->mNumMaterials > 0 ? false : true);
        for (uint32_t materialIndex = 0; materialIndex < scene->mNumMaterials; materialIndex++)
        {
            m_MaterialTable->SetMaterial(materialIndex, GetMaterial(scene->mMaterials[materialIndex]));
        }

        m_VertexBuffer = VertexBuffer::Create(m_Vertices.data(), m_Vertices.size() * sizeof(Vertex));
        m_IndexBuffer = IndexBuffer::Create(m_Indices.data(), m_Indices.size());

        m_Indices.clear();
        m_Vertices.clear();
    }
    MeshSource::MeshSource()
    {
        m_MaterialTable = Count<MaterialTable>::Create();

    }
    MeshSource::MeshSource(const std::string& name, const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices) :
        m_Vertices(vertices), m_Indices(indices)
    {
        m_MaterialTable = Count<MaterialTable>::Create();
        SubMesh subMesh;
        subMesh.BaseVertex = 0;
        subMesh.BaseIndex = 0;
        subMesh.IndexCount = indices.size();
        subMesh.MaterialIndex = 0;
        subMesh.Name = name;
        subMesh.Transform = glm::translate(glm::mat4(1.0f), ProofToglmVec(0)) *
            glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), { 1,0,0 }) *
            glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), { 0,1,0 }) *
            glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), { 0,0,1 }) *
            glm::scale(glm::mat4(1.0f), ProofToglmVec({ 1,1,1 }));

        m_SubMeshes.emplace_back(subMesh);
        m_VertexBuffer = VertexBuffer::Create(m_Vertices.data(), m_Vertices.size() * sizeof(Vertex));
        m_IndexBuffer = IndexBuffer::Create(m_Indices.data(), m_Indices.size());

        m_Indices.clear();
        m_Vertices.clear();
    }
    void MeshSource::ProcessNode(void* node, const void* scene, const glm::mat4& parentTransform)
    {
        aiNode* ainode = (aiNode*)node;
        aiScene* aiscene = (aiScene*)scene;
        glm::mat4 transform = parentTransform * AIMatrixToGLM(ainode->mTransformation);

        for (uint32_t i = 0; i < ainode->mNumMeshes; i++)
        {
            aiMesh* mesh = aiscene->mMeshes[ainode->mMeshes[i]];
            m_SubMeshes.emplace_back(ProcessMesh(mesh, aiscene, transform));
        }
        for (uint32_t i = 0; i < ainode->mNumChildren; i++)
        {
            ProcessNode(ainode->mChildren[i], aiscene, transform);
        }
    }

    SubMesh MeshSource::ProcessMesh(void* mesh, const void* scene, const glm::mat4& transform)
    {
        aiMesh* aimesh = (aiMesh*)mesh;
        aiScene* aiscene = (aiScene*)scene;
        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;

        SubMesh subMesh;
        subMesh.BaseIndex = m_Indices.size();
        subMesh.BaseVertex = m_Vertices.size();

        for (uint32_t i = 0; i < aimesh->mNumVertices; i++)
        {
            Vertex vertex;
            vertex.Vertices = Vector(aimesh->mVertices[i].x, aimesh->mVertices[i].y, aimesh->mVertices[i].z);
            if (aimesh->HasNormals())
                vertex.Normal = Vector(aimesh->mNormals[i].x, aimesh->mNormals[i].y, aimesh->mNormals[i].z);
            if (aimesh->mTextureCoords[0])
            {
                vertex.TexCoords = Vector2(aimesh->mTextureCoords[0][i].x, aimesh->mTextureCoords[0][i].y);
                vertex.Tangent = Vector(aimesh->mTangents[i].x, aimesh->mTangents[i].y, aimesh->mTangents[i].z);
                vertex.Bitangent = Vector(aimesh->mBitangents[i].x, aimesh->mBitangents[i].y, aimesh->mBitangents[i].z);
            }
            else
                vertex.TexCoords = Vector2(0.0f, 0.0f);
            m_Vertices.emplace_back(vertex);
        }
        uint32_t indexCount = 0;
        for (uint32_t i = 0; i < aimesh->mNumFaces; i++)
        {
            aiFace& face = aimesh->mFaces[i];
            for (uint32_t j = 0; j < face.mNumIndices; j++)
            {
                indexCount++;
                m_Indices.emplace_back(face.mIndices[j]);
            }
        }

        subMesh.Name = aimesh->mName.C_Str();
        subMesh.Transform = transform;
        subMesh.IndexCount = indexCount;
        subMesh.MaterialIndex = aimesh->mMaterialIndex;
        return subMesh;
    }

    Count<Material> MeshSource::GetMaterial(void* mat)
    {
        aiMaterial* aimat = (aiMaterial*)mat;
        Count<Material> material = Count<Material>::Create();
        float metallness = 0.0;
        float roghness = 0.4;
        //aiGetMaterialString(aimat,mat, AI_MATKEY_NAME, &name);
        aiString name;
        aimat->Get(AI_MATKEY_NAME, name);
        material->Name = name.C_Str();
        if (material->Name.empty())
            material->Name = "UnnamedMaterial";
        aiColor3D color;
        if (aimat->Get(AI_MATKEY_COLOR_AMBIENT, color) == aiReturn_SUCCESS)
        {
            material->SetAlbedo(Vector(color.r, color.g, color.b));
        }
        if (aimat->Get(AI_MATKEY_METALLIC_FACTOR, metallness) == aiReturn_SUCCESS)
        {
            material->GetMetalness() = metallness;
        }
        if (aimat->Get(AI_MATKEY_ROUGHNESS_FACTOR, roghness) == aiReturn_SUCCESS)
        {
            material->GetRoughness() = roghness;
        }
        PF_ENGINE_TRACE("   Loaded Material:{} Color:{} Roughness:{} Metalness:{}", material->Name, material->GetAlbedoColor().ToString(), material->GetRoughness(), material->GetMetalness());
        LoadMaterialTextures(material, aimat);
        return material;
    }
    static Count<Texture2D> LoadTextures(aiTextureType textureType, aiMaterial* aimat,MeshSource* meshSource)
    {
        aiString strFilePath;
        if (aimat->GetTexture(textureType, 0, &strFilePath) == aiReturn_SUCCESS)
        {
            std::string textureFilePath = (meshSource->GetPath().parent_path() /= strFilePath.C_Str()).string();
            //textureFilePath = std::filesystem::(textureFilePath, Application::).string();
            //AssetManager::GetAssetFileSystemPath
            std::string textureName = Utils::FileDialogs::GetFileName(textureFilePath);
            // checking if has the texturer source file

            if (AssetManager::HasAsset(textureFilePath))
            {
                // changing the path to a proof asset path
               // PF_EC_WARN("Getting mehs texture this way is risky what if the name changes");
                std::string path = std::filesystem::relative(meshSource->GetPath().parent_path() /= textureName).string();
                path += ".Texture.ProofAsset";
                if (AssetManager::HasAsset(path))
                    return AssetManager::GetAsset<Texture2D>(path);
                else
                {

                    AssetManager::NewAssetSource(textureFilePath, AssetType::TextureSourceFile);
                    Count<Asset> asset = Texture2D::Create(TextureConfiguration(Utils::FileDialogs::GetFileName(path)), textureFilePath);
                    AssetManager::NewAsset(asset, path);
                    return AssetManager::GetAsset<Texture2D>(path);
                   // PF_EC_CRITICAL("Proof Should not be using raw texture file to create meshes causes an error for invalid descript set");
                    //texture = Texture2D::Create(textureFilePath);
                }
            }
            else
            {
                std::string path = std::filesystem::relative(meshSource->GetPath().parent_path() /= textureName).string();
                path += ".Texture.ProofAsset";
                AssetManager::NewAssetSource(textureFilePath, AssetType::TextureSourceFile);
                Count<Asset> asset = Texture2D::Create(TextureConfiguration(Utils::FileDialogs::GetFileName(textureFilePath)), textureFilePath);
                AssetManager::NewAsset(asset, path);
                return AssetManager::GetAsset<Texture2D>(path);
            }
        }
        return Renderer::GetWhiteTexture();
    }
    void MeshSource::LoadMaterialTextures(Count<Material> material, void* mat)
    {
        aiMaterial* aiMaterialCast = (aiMaterial*)mat;
        material->SetAlbedoMap(LoadTextures(aiTextureType_DIFFUSE, aiMaterialCast, this));
        material->SetNormalMap(LoadTextures(aiTextureType_NORMALS, aiMaterialCast, this));
        material->SetMetalnessMap(LoadTextures(aiTextureType_METALNESS, aiMaterialCast, this));
        material->SetRoughnessMap(LoadTextures(aiTextureType_DIFFUSE_ROUGHNESS, aiMaterialCast, this));

        if (material->GetAlbedoMap() != Renderer::GetWhiteTexture())
            PF_ENGINE_TRACE("       Albedo Map:{} ",material->GetAlbedoMap()->GetPath().string());
        
        if (material->GetNormalMap() != Renderer::GetWhiteTexture())
        {
            material->SetNormalTextureToggle(true);
            PF_ENGINE_TRACE("       Normal Map:{} ", material->GetNormalMap()->GetPath().string());
        }
        
        if (material->GetRoughnessMap() != Renderer::GetWhiteTexture())
        {
            material->SetRoughnessTextureToggle(true);
            PF_ENGINE_TRACE("       Roughness Map:{} ", material->GetRoughnessMap()->GetPath().string());
        }
       
        if (material->GetMetalnessMap() != Renderer::GetWhiteTexture())
        {
            material->SetMetalnessTextureToggle(true);
            PF_ENGINE_TRACE("       Metalness Map:{} ", material->GetMetalnessMap()->GetPath().string());
        }

    }

    Mesh::Mesh(Count<MeshSource> meshSource,const std::vector<uint32_t>& submeshes)
        :m_MeshSource(meshSource)
    {
        m_MaterialTable = meshSource->GenerateMaterialTable();
        m_Name = Utils::FileDialogs::GetFileName(m_MeshSource->GetPath());
        SetSubMeshes(submeshes);
    }
    Mesh::Mesh(const std::string& name, std::vector<Vertex> vertices, std::vector<uint32_t>indices)
    {
        m_MeshSource = Count<MeshSource>::Create(name,vertices, indices);
        m_MaterialTable = m_MeshSource->GenerateMaterialTable();
        SetSubMeshes({});
    }

    void Mesh::SetSubMeshes(const std::vector<uint32_t>& submesh)
    {
        if (!submesh.empty())
        {
            m_SubMeshes = submesh;
            return;
        }
        const auto& submeshes = m_MeshSource->GetSubMeshes();
        m_SubMeshes.resize(submeshes.size());
        for (uint32_t i = 0; i < submeshes.size(); i++)
            m_SubMeshes[i] = i;
    }
}