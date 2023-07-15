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
#include "Proof/Renderer/RendererBase.h"
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
        
        ProcessNode(scene->mRootNode, scene, AIMatrixToGLM(scene->mRootNode->mTransformation));
       
        
        m_MaterialTable = Count<MaterialTable>::Create(scene->mNumMaterials > 0 ? false : true);
        for (uint32_t materialIndex = 0; materialIndex < scene->mNumMaterials; materialIndex++)
        {
            m_MaterialTable->SetMaterial(materialIndex, GetMaterial(scene->mMaterials[materialIndex]));
        }
    }
    MeshSource::MeshSource()
    {
        m_MaterialTable = Count<MaterialTable>::Create();

    }
    MeshSource::MeshSource(const std::string& name, const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices)
    {
        m_MaterialTable = Count<MaterialTable>::Create();
        SubMesh meh(vertices, indices, name);
        m_SubMeshes.emplace_back(meh);
    }
    void MeshSource::ProcessNode(void* node, const void* scene, const glm::mat4& parentTransform)
    {
        aiNode* ainode = (aiNode*)node;
        aiScene* aiscene = (aiScene*)scene;
        glm::mat4 transform = parentTransform * AIMatrixToGLM(ainode->mTransformation);

        for (uint32_t i = 0; i < ainode->mNumMeshes; i++)
        {
            aiMesh* mesh = aiscene->mMeshes[ainode->mMeshes[i]];
            m_SubMeshes.emplace_back(ProcessMesh(mesh, aiscene,transform));
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
        temp.Transform = transform;
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

        //material->Colour = Vector(color.r, color.g, color.b);
        //material->Metallness = metallness;
        //material->Roughness = roghness;

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
                //textureFilePath = std::filesystem::(textureFilePath, Application::).string();
                //AssetManager::GetAssetFileSystemPath
                std::string textureName = Utils::FileDialogs::GetFileName(textureFilePath);
                // checking if has the texturer source file

                if (AssetManager::HasAsset(textureFilePath))
                {
                    // changing the path to a proof asset path
                   // PF_EC_WARN("Getting mehs texture this way is risky what if the name changes");
                    std::string path = std::filesystem::relative(m_Path.parent_path() /= textureName).string();
                    path += ".Texture.ProofAsset";
                    if(AssetManager::HasAsset(path))
                        texture = AssetManager::GetAsset<Texture2D>(path);
                    else
                    {
                        
                        AssetManager::NewAssetSource(textureFilePath, AssetType::TextureSourceFile);
                        Count<Asset> asset = Texture2D::Create(TextureConfiguration(Utils::FileDialogs::GetFileName(path)), textureFilePath);
                        AssetManager::NewAsset(asset, path);
                        texture = AssetManager::GetAsset<Texture2D>(path);
                       // PF_EC_CRITICAL("Proof Should not be using raw texture file to create meshes causes an error for invalid descript set");
                        //texture = Texture2D::Create(textureFilePath);
                    }
                }
                else
                {
                    std::string path = std::filesystem::relative(m_Path.parent_path() /= textureName).string();
                    path += ".Texture.ProofAsset";
                    AssetManager::NewAssetSource(textureFilePath, AssetType::TextureSourceFile);
                    Count<Asset> asset = Texture2D::Create(TextureConfiguration(Utils::FileDialogs::GetFileName(textureFilePath)),textureFilePath);
                    AssetManager::NewAsset(asset, path);
                    texture = AssetManager::GetAsset<Texture2D>(path);
                }
            }
        };

        //LoadTexture(material->AlbedoTexture, aiTextureType_DIFFUSE);
        //LoadTexture(material->NormalTexture, aiTextureType_NORMALS);
        //LoadTexture(material->MetallicTexture, aiTextureType_METALNESS);
        //LoadTexture(material->RoughnessTexture, aiTextureType_DIFFUSE_ROUGHNESS);
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

    SubMesh::SubMesh(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices, const std::string& name, uint32_t materialIndex)
    {
        Name = name;
        MaterialIndex = materialIndex;
        Vertices = vertices;
        Indices = indices;
        VertexBuffer = VertexBuffer::Create(vertices.data(), vertices.size() * sizeof(Vertex));
        IndexBuffer = IndexBuffer::Create(indices.data(), indices.size());
    }

}