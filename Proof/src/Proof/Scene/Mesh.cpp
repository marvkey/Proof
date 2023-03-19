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
#include "Proof/Renderer/Buffer.h"
namespace Proof{
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
        ProcessNode(scene->mRootNode, scene);
       
        
        m_MaterialTable = Count<MaterialTable>::Create(scene->mNumMaterials > 0 ? false : true);
        for (uint32_t materialIndex = 0; materialIndex < scene->mNumMaterials; materialIndex++)
        {
            //scene->mMaterials[]
            m_MaterialTable->SetMaterial(materialIndex, GetMaterial(scene->mMaterials[materialIndex]));
        }
    }

    MeshSource::MeshSource(const std::string& name, const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices)
    {
        m_MaterialTable = Count<MaterialTable>::Create();
        SubMesh meh(vertices, indices, name);
        m_SubMeshes.emplace_back(meh);
    }
    void MeshSource::ProcessNode(void* node, const void* scene)
    {
        aiNode* ainode = (aiNode*)node;
        aiScene* aiscene = (aiScene*)scene;
        for (uint32_t i = 0; i < ainode->mNumMeshes; i++)
        {
            aiMesh* mesh = aiscene->mMeshes[ainode->mMeshes[i]];
            m_SubMeshes.emplace_back(ProcessMesh(mesh, aiscene));
        }
        for (uint32_t i = 0; i < ainode->mNumChildren; i++)
        {
            ProcessNode(ainode->mChildren[i], aiscene);
        }
    }

    SubMesh MeshSource::ProcessMesh(void* mesh, const void* scene)
    {
        aiMesh* aimesh = (aiMesh*)mesh;
        aiScene* aiscene = (aiScene*)scene;

        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;
        std::vector<Count<Texture2D>> textures;

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
            vertices.emplace_back(vertex);
        }
        for (uint32_t i = 0; i < aimesh->mNumFaces; i++)
        {
            aiFace& face = aimesh->mFaces[i];
            for (uint32_t  j = 0; j < face.mNumIndices; j++)
            {
                indices.emplace_back(face.mIndices[j]);
            }
        }
        SubMesh temp(vertices, indices, aimesh->mName.C_Str(), aimesh->mMaterialIndex);
        return temp;
    }

    Count<Material> MeshSource::GetMaterial(void* mat)
    {
        aiMaterial* aimat = (aiMaterial*)mat;
        Count<Material> material = Count<Material>::Create();
        aiColor3D color(0.f, 0.f, 0.f);
        float metallness;
        float roghness;
        //aiGetMaterialString(aimat,mat, AI_MATKEY_NAME, &name);
        aiString name;
        aimat->Get(AI_MATKEY_NAME, name);
        material->Name = name.C_Str();
        aimat->Get(AI_MATKEY_COLOR_AMBIENT, color);
        aimat->Get(AI_MATKEY_METALLIC_FACTOR, metallness);
        aimat->Get(AI_MATKEY_ROUGHNESS_FACTOR, roghness);

        material->Colour = Vector(color.r, color.g, color.b);
        material->Metallness = metallness;
        material->Roughness = roghness;

        LoadMaterialTextures(material, aimat);
        return material;
    }

    void MeshSource::LoadMaterialTextures(Count<Material> material, void* aiMat)
    {
        aiMaterial* aimat = (aiMaterial*)aiMat;
        auto LoadTexture = [&](Count<Texture2D>& texture,aiTextureType textureType)->void {
            aiString strFilePath;
            if (aimat->GetTexture(textureType, 0, &strFilePath) == aiReturn_SUCCESS)
            {
                std::string textureFilePath = (m_Path.parent_path() /= strFilePath.C_Str()).string();
                std::string textureName = Utils::FileDialogs::GetFileName(textureFilePath);
                // checking if has the texturer source file

                if (AssetManager::HasAsset(textureFilePath))
                {
                    // changing the path to a proof asset path
                    PF_EC_WARN("Getting mehs texture this way is risky what if the name changes");
                    std::string path = std::filesystem::relative(m_Path.parent_path() /= textureName).string();
                    path += ".Texture.ProofAsset";
                    texture = AssetManager::GetAsset<Texture2D>(path);

                }
                else
                {

                    AssetManager::NewAssetSource(textureFilePath, AssetType::TextureSourceFile);
                    Count<Asset> asset = Texture2D::Create(textureFilePath);
                    AssetManager::NewAsset(asset, textureFilePath);
                    texture = AssetManager::GetAsset<Texture2D>(textureFilePath);
                }
            }
        };

        LoadTexture(material->AlbedoTexture, aiTextureType_DIFFUSE);
        LoadTexture(material->NormalTexture, aiTextureType_NORMALS);
        LoadTexture(material->MetallicTexture, aiTextureType_METALNESS);
        LoadTexture(material->RoughnessTexture, aiTextureType_DIFFUSE_ROUGHNESS);
    }

    Mesh::Mesh(Count<MeshSource> meshSource,const std::vector<uint32_t>& excludeSubMeshes)
        :m_MeshSource(meshSource)
    {
        m_MaterialTable = meshSource->GenerateMaterialTable();
        m_ExcludeMeshes = excludeSubMeshes;
        m_Name = Utils::FileDialogs::GetFileName(m_MeshSource->GetPath());
    }
    Mesh::Mesh(const std::string& name, std::vector<Vertex> vertices, std::vector<uint32_t>indices)
    {
        m_MeshSource = Count<MeshSource>::Create(name,vertices, indices);
        m_MaterialTable = m_MeshSource->GenerateMaterialTable();
    }

    SubMesh::SubMesh(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices, const std::string& name, uint32_t materialIndex)
    {
        Vertices = vertices;
        Indices = indices;
        Name = name;
        MaterialIndex = materialIndex;

        VertexBuffer = VertexBuffer::Create(Vertices.data(), Vertices.size() * sizeof(Vertex));
        IndexBuffer = IndexBuffer::Create(Indices.data(), Indices.size());
    }

}