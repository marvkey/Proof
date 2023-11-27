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
    MeshSource::MeshSource(const std::string& name, const std::vector<Vertex>& vertices, const std::vector<Index>& indices)
    {
        Reset(name, vertices, indices);
    }
    MeshSource::MeshSource(const std::string& name, const std::vector<Vertex>& vertices, const std::vector<Index>& indices, const std::vector<SubMesh>& subMeshes, const std::vector<MeshNode>& nodes, Count<MaterialTable> materials, AABB boundingBox)
    {
        Reset(name, vertices, indices, subMeshes, nodes,materials, boundingBox);
    }
    MeshSource::MeshSource(const std::string& name, const std::vector<Vertex>& vertices, const std::vector<Index>& indices, const std::vector<SubMesh>& submeshes)
    {
        m_Name = name;
        m_SubMeshes = submeshes;
        m_VertexBuffer = VertexBuffer::Create(vertices.data(), (uint32_t)(vertices.size() * sizeof(Vertex)));
        m_IndexBuffer = IndexBuffer::Create(indices.data(), (uint32_t)(indices.size() * sizeof(Index)));
    }
    void MeshSource::Reset(const std::string& name, const std::vector<Vertex>& vertices, const std::vector<Index>& indices)
    {
        SubMesh subMesh;
        subMesh.BaseVertex = 0;
        subMesh.BaseIndex = 0;
        subMesh.IndexCount = indices.size() * 3u;
        subMesh.VertexCount = vertices.size();
        subMesh.MaterialIndex = 0;
        subMesh.Name = name;
        subMesh.Transform = glm::mat4(1.0f);
        subMesh.LocalTransform = glm::mat4(1.0f);

        m_SubMeshes = {};
        m_SubMeshes.emplace_back(subMesh);
        m_VertexBuffer = VertexBuffer::Create(vertices.data(), vertices.size() * sizeof(Vertex));
        m_IndexBuffer = IndexBuffer::Create(indices.data(), indices.size() * sizeof(Index));

        m_Materials = Count<MaterialTable>::Create();

        MeshNode node;
        node.Name = "name";
        node.Submeshes.emplace_back(0);
        node.LocalTransform = glm::mat4(1.0f);
        m_Nodes = {};
        m_Nodes.emplace_back(node);

        

        m_BoundingBox.Min= { FLT_MAX, FLT_MAX, FLT_MAX };
        m_BoundingBox.Max = { -FLT_MAX, -FLT_MAX, -FLT_MAX };

        for (size_t i = 0; i < vertices.size(); i++)
        {
            const Vertex& vertex = vertices[i];
            m_BoundingBox.Min.x = glm::min(vertex.Position.x, m_BoundingBox.Min.x);
            m_BoundingBox.Min.y = glm::min(vertex.Position.y, m_BoundingBox.Min.y);
            m_BoundingBox.Min.z = glm::min(vertex.Position.z, m_BoundingBox.Min.z);
            m_BoundingBox.Max.x = glm::max(vertex.Position.x, m_BoundingBox.Max.x);
            m_BoundingBox.Max.y = glm::max(vertex.Position.y, m_BoundingBox.Max.y);
            m_BoundingBox.Max.z = glm::max(vertex.Position.z, m_BoundingBox.Max.z);
        }
        for (auto& [index, material] : m_Materials->GetMaterials())
        {
            if (AssetManager::HasAsset(material))
                continue;
            Count<Asset> asset = material;
            const std::string materialname = material->Name.empty() ? "UnnamedMaterial" : material->Name;
            AssetManager::CreateRuntimeAsset(AssetManager::CreateID(), asset, materialname);
        }
    }

    void MeshSource::Reset(const std::string& name, const std::vector<Vertex>& vertices, const std::vector<Index>& indices, const std::vector<SubMesh>& subMeshes, const std::vector<MeshNode>& nodes, Count<MaterialTable> materials,AABB boundingBox)
    {
        m_Name = name;

        m_VertexBuffer = VertexBuffer::Create(vertices.data(), vertices.size() * sizeof(Vertex));
        m_IndexBuffer = IndexBuffer::Create(indices.data(), indices.size() * sizeof(Index));
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
            const std::string materialname = material->Name.empty() ? "UnnamedMaterial" : material->Name;
            AssetManager::CreateRuntimeAsset(AssetManager::CreateID(),asset, materialname);
        }
    }

    std::vector<Vertex> MeshSource::GetVertices() const
    {
        if(m_VertexBuffer ==nullptr)
            return std::vector<Vertex>();

        return m_VertexBuffer->GetDataAs<Vertex>();
    }

    std::vector<Index> MeshSource::GetIndices() const
    {
        if(m_IndexBuffer ==nullptr)
            return std::vector<Index>();

        return m_IndexBuffer->GetDataAs<Index>();
    }

    Mesh::Mesh(Count<MeshSource> meshSource,const std::vector<uint32_t>& submeshes)
        :m_MeshSource(meshSource)
    {
        Reset(meshSource, submeshes);
    }
    Mesh::Mesh(const std::string& name, std::vector<Vertex> vertices, std::vector<Index>indices)
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
    void Mesh::Reset(const std::string& name, std::vector<Vertex> vertices, std::vector<Index>indices)
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
            /*
            // Use std::remove_if to move elements that meet the condition to the end
            auto removeIt = std::remove_if(m_SubMeshes.begin(), m_SubMeshes.end(), [&](uint32_t num) {
                if (num >= m_MeshSource->GetSubMeshes().size())return true;
                return false;
            });
            // Erase elements from the end to remove them
            m_SubMeshes.erase(removeIt, m_SubMeshes.end());

            auto uniqueEnd = std::unique(m_SubMeshes.begin(), m_SubMeshes.end());
            // Erase the duplicates from the vector
            m_SubMeshes.erase(uniqueEnd, m_SubMeshes.end());

            std::sort(m_SubMeshes.begin(), m_SubMeshes.end());
            */

            return;
        }
        const auto& submeshes = m_MeshSource->GetSubMeshes();
        m_SubMeshes.resize(submeshes.size());
        for (uint32_t i = 0; i < submeshes.size(); i++)
            m_SubMeshes[i] = i;
    }

    bool Mesh::HasSubMesh(uint32_t subMeshIndex)
    {
        return std::find(m_SubMeshes.begin(), m_SubMeshes.end(), subMeshIndex) != m_SubMeshes.end();
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
            /*
            // Use std::remove_if to move elements that meet the condition to the end
            auto removeIt = std::remove_if(m_SubMeshes.begin(), m_SubMeshes.end(), [&](uint32_t num) {
                if (num >= m_MeshSource->GetSubMeshes().size())return true;
                return false;
            });
            // Erase elements from the end to remove them
            m_SubMeshes.erase(removeIt, m_SubMeshes.end());

            auto uniqueEnd = std::unique(m_SubMeshes.begin(), m_SubMeshes.end());
            // Erase the duplicates from the vector
            m_SubMeshes.erase(uniqueEnd, m_SubMeshes.end());

            std::sort(m_SubMeshes.begin(), m_SubMeshes.end());
            */
            return;
        }
        const auto& submeshes = m_MeshSource->GetSubMeshes();
        m_SubMeshes.resize(submeshes.size());
        for (uint32_t i = 0; i < submeshes.size(); i++)
            m_SubMeshes[i] = i;
    }
    bool DynamicMesh::HasSubMesh(uint32_t subMeshIndex)
    {
        return std::find(m_SubMeshes.begin(), m_SubMeshes.end(), subMeshIndex) != m_SubMeshes.end();
    }
}