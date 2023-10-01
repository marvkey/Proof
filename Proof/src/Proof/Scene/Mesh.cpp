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
#include "Proof/Math/Vector.h"
#include "Proof/Renderer/Renderer.h"
#include "Proof/Renderer/Buffer.h"
#include "Proof/Asset/AssetManager.h"
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
    MeshSource::MeshSource()
    {
        m_Materials = Count<MaterialTable>::Create();
    }
    MeshSource::MeshSource(const std::string& name, const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices) 
    {
        Reset(name, vertices, indices);
    }
    MeshSource::MeshSource(const std::string& name, const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices, const std::vector<SubMesh>& subMeshes, const std::vector<MeshNode>& nodes, Count<MaterialTable> materials, AABB boundingBox)
    {
        Reset(name, vertices, indices, subMeshes, nodes,materials, boundingBox);
    }
    void MeshSource::Reset(const std::string& name, const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices)
    {
        SubMesh subMesh;
        subMesh.BaseVertex = 0;
        subMesh.BaseIndex = 0;
        subMesh.IndexCount = indices.size();
        subMesh.VertexCount = vertices.size();
        subMesh.MaterialIndex = 0;
        subMesh.Name = name;
        subMesh.Transform = glm::translate(glm::mat4(1.0f), ProofToglmVec(0)) *
            glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), { 1,0,0 }) *
            glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), { 0,1,0 }) *
            glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), { 0,0,1 }) *
            glm::scale(glm::mat4(1.0f), ProofToglmVec({ 1,1,1 }));

        m_SubMeshes.emplace_back(subMesh);
        m_VertexBuffer = VertexBuffer::Create(vertices.data(), vertices.size() * sizeof(Vertex));
        m_IndexBuffer = IndexBuffer::Create(indices.data(), indices.size());

        m_Materials = Count<MaterialTable>::Create();

        for (auto& [index, material] : m_Materials->GetMaterials())
        {
            if (AssetManager::HasAsset(material))
                continue;
            Count<Asset> asset = material;
            AssetManager::CreateRuntimeAsset(asset);
        }
    }

    void MeshSource::Reset(const std::string& name, const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices, const std::vector<SubMesh>& subMeshes, const std::vector<MeshNode>& nodes, Count<MaterialTable> materials,AABB boundingBox)
    {
        m_Name = name;

        m_VertexBuffer = VertexBuffer::Create(vertices.data(), vertices.size() * sizeof(Vertex));
        m_IndexBuffer = IndexBuffer::Create(indices.data(), indices.size());
        m_SubMeshes = subMeshes;
        m_Nodes = nodes;
        m_BoundingBox = boundingBox;
        m_Materials = Count<MaterialTable>::CreateFrom(materials);

        // maybe each mesh might have its own material asset maybe
        for (auto& [index, material] : m_Materials->GetMaterials())
        {
            if (AssetManager::HasAsset(material))
                continue;
            Count<Asset> asset = material;
            AssetManager::CreateRuntimeAsset(asset);
        }
    }

    Mesh::Mesh(Count<MeshSource> meshSource,const std::vector<uint32_t>& submeshes)
        :m_MeshSource(meshSource)
    {
        Reset(meshSource, submeshes);
    }
    Mesh::Mesh(const std::string& name, std::vector<Vertex> vertices, std::vector<uint32_t>indices)
    {
        Reset(name,vertices,indices);
    }
    void Mesh::Reset(Count<MeshSource> meshSource, const std::vector<uint32_t>& subMeshes)
    {
        m_MeshSource = meshSource;
        m_Name = m_MeshSource->GetName();
        SetSubMeshes(subMeshes);
        m_MaterialTable = Count<MaterialTable>::CreateFrom(meshSource->GetMaterials());
    }
    void Mesh::Reset(const std::string& name, std::vector<Vertex> vertices, std::vector<uint32_t>indices)
    {
        m_MeshSource = Count<MeshSource>::Create(name, vertices, indices);
        m_MaterialTable = Count<MaterialTable>::Create();
        SetSubMeshes({});
        m_MaterialTable = Count<MaterialTable>::CreateFrom(m_MeshSource->GetMaterials());
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

    DynamicMesh::DynamicMesh(Count<MeshSource> meshSource, const std::vector<uint32_t>& subMeshes)
        :m_MeshSource(meshSource)

    {
        Reset(meshSource, subMeshes);
    }
    void DynamicMesh::Reset(Count<MeshSource> meshSource, const std::vector<uint32_t>& subMeshes)
    {

        m_MeshSource = meshSource;
        m_Name = m_MeshSource->GetName();
        SetSubMeshes(subMeshes);
        m_MaterialTable = Count<MaterialTable>::CreateFrom(meshSource->GetMaterials());
    }
    void DynamicMesh::SetSubMeshes(const std::vector<uint32_t>& submesh)
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