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
#include "Proof/Utils/ContainerUtils.h"
#include "Proof/Asset/AssetManager.h"
#include "Proof/Asset/MeshImpoter.h"

#include "Proof/Animation/Skeleton.h"
#include "Proof/Animation/Animation.h"
namespace Proof
{
    static glm::mat4 AIMatrixToGLM(const aiMatrix4x4& aiMatrix) 
    {
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
    MeshSource::~MeshSource()
    {
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
        if(!vertices.empty())
            m_VertexBuffer = VertexBuffer::Create(vertices.data(),(vertices.size() * sizeof(Vertex)));
		if (!indices.empty())
            m_IndexBuffer = IndexBuffer::Create(indices.data(), (indices.size() * sizeof(Index)));

        m_Vertices = vertices;
        m_Indices = indices;

        Count<MeshSource> instance = this;
        Renderer::Submit([instance]() mutable
            {
               // instance->m_Vertices.clear();
               // instance->m_Indices.clear();
            });
    }
    inline Count<MeshSource> MeshSource::CombineMeshes(const std::string& name,const std::vector<Count<MeshSource>>& source)
    {
        std::vector<Vertex> combinedVertices;
        std::vector<Index> combinedIndices;
        std::vector<SubMesh> combinedSubMeshes;

        uint32_t vertexOffset = 0;
        uint32_t subMeshIndex = 0;

        for (auto& mesh : source)
        {
            const auto& vertices = mesh->GetVertices();
            const auto& indices = mesh->GetIndices();
            const auto& subMeshes = mesh->GetSubMeshes();

            // Append vertices
            combinedVertices.insert(combinedVertices.end(), vertices.begin(), vertices.end());

            // Adjust and append indices
            for (const auto& idx : indices)
            {
                combinedIndices.push_back({
                    idx.V1 + vertexOffset,
                    idx.V2 + vertexOffset,
                    idx.V3 + vertexOffset
                    });
            }

            // Adjust and append submeshes
            for (const auto& sm : subMeshes)
            {
                SubMesh newSubMesh = sm;
                newSubMesh.BaseVertex += vertexOffset;
                newSubMesh.BaseIndice += static_cast<uint32_t>(combinedIndices.size()) - indices.size();
                newSubMesh.SubMeshIndex = subMeshIndex++;
                combinedSubMeshes.push_back(newSubMesh);
            }

            vertexOffset += static_cast<uint32_t>(vertices.size());
        }

        return Count<MeshSource>::Create(name, combinedVertices, combinedIndices, combinedSubMeshes);
    }
    void MeshSource::Reset(const std::string& name, const std::vector<Vertex>& vertices, const std::vector<Index>& indices)
    {
        SubMesh subMesh;
        subMesh.BaseVertex = 0;
        subMesh.BaseIndice = 0;
        subMesh.IndiceCount = indices.size() * 3u;
        subMesh.VertexCount = vertices.size();
        subMesh.MaterialIndex = 0;
        subMesh.Name = name;
        subMesh.SubMeshIndex = 0;
        subMesh.Transform = glm::mat4(1.0f);
        subMesh.LocalTransform = glm::mat4(1.0f);

        m_SubMeshes = {};
        m_SubMeshes.emplace_back(subMesh);

        if (!vertices.empty())
            m_VertexBuffer = VertexBuffer::Create(vertices.data(), (vertices.size() * sizeof(Vertex)));
        if (!indices.empty())
            m_IndexBuffer = IndexBuffer::Create(indices.data(), (indices.size() * sizeof(Index)));

        m_Vertices = vertices;
        m_Indices = indices;

        Count<MeshSource> instance = this;
        Renderer::Submit([instance]() mutable
            {
            });
        m_Materials = Count<MaterialTable>::Create();

        MeshNode node;
        node.Name = "name";
        node.Submeshes.emplace_back(0);
        node.LocalTransform = glm::mat4(1.0f);
        node.Index = 0;
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
            AssetManager::CreateRuntimeAsset(AssetManager::CreateID(), material, materialname);
        }

        m_SubMeshes.front().BoundingBox = m_BoundingBox;
    }

    void MeshSource::Reset(const std::string& name, const std::vector<Vertex>& vertices, const std::vector<Index>& indices, const std::vector<SubMesh>& subMeshes, const std::vector<MeshNode>& nodes, Count<MaterialTable> materials,AABB boundingBox)
    {
        m_Name = name;

        if (!vertices.empty())
            m_VertexBuffer = VertexBuffer::Create(vertices.data(), (vertices.size() * sizeof(Vertex)));
        if (!indices.empty())
            m_IndexBuffer = IndexBuffer::Create(indices.data(), (indices.size() * sizeof(Index)));
        m_SubMeshes = subMeshes;
        m_Nodes = nodes;
        m_BoundingBox = boundingBox;
        m_Materials = Count<MaterialTable>::CreateFrom(materials);

        m_Vertices = vertices;
        m_Indices = indices;
        // maybe each mesh might have its own material asset maybe
        for (auto& [index, material] : m_Materials->GetMaterials())
        {
            if (AssetManager::HasAsset(material))
                continue;
            Count<Asset> asset = material;
            const std::string materialname = material->Name.empty() ? "UnnamedMaterial" : material->Name;
            AssetManager::CreateRuntimeAsset(AssetManager::CreateID(),asset, materialname);
        }

        m_SubMeshes.front().BoundingBox = m_BoundingBox;
    }

    bool MeshSource::NodeHasAnySubMesh(uint32_t nodeIndex)
    {
        if (m_Nodes.size() <= nodeIndex)
            return false;

        MeshNode& node = m_Nodes.at(nodeIndex);

        if (!node.Submeshes.empty())
            return true;

        for (auto& childNode : node.Children)
        {
            if (NodeHasAnySubMesh(childNode))
                return true;
        }

        return false;
    }

    bool MeshSource::NodeHasSubMesh(uint32_t nodeIndex, uint32_t subMeshIndex)
    {
        if (m_Nodes.size() <= nodeIndex)
            return false;

        MeshNode& node = m_Nodes.at(nodeIndex);

        if (Utils::Contains(node.Submeshes, subMeshIndex))
            return true;

        for (auto& childNodes : node.Children)
        {
            if (NodeHasSubMesh(childNodes, subMeshIndex))
                return true;
        }

        return false;
    }

    bool MeshSource::NodeHasSubAnyMesh(uint32_t nodeIndex, const std::vector<uint32_t>& subMeshMap)
    {
        if (m_Nodes.size() <= nodeIndex)
            return false;

        MeshNode& node = m_Nodes.at(nodeIndex);

        if (Utils::ContainsAny(node.Submeshes, subMeshMap))
            return true;

        for (auto& childNodes : node.Children)
        {
            if (NodeHasSubAnyMesh(childNodes, subMeshMap))
                return true;
        }

        return false;
    }

    void MeshSource::DisableNodeSubMeshes(uint32_t nodeIndex, std::vector<uint32_t>& subMeshMap)
    {
        if (m_Nodes.size() <= nodeIndex)
            return;
        MeshNode& node = m_Nodes.at(nodeIndex);

        for (auto& subMeshIndex : node.Submeshes)
            Utils::Remove(subMeshMap, subMeshIndex);

        for (auto& childNodes : node.Children)
        {
            DisableNodeSubMeshes(childNodes, subMeshMap);
        }

    }

    void MeshSource::EnableNodeSubMeshes(uint32_t nodeIndex, std::vector<uint32_t>& subMeshMap)
    {
        if (m_Nodes.size() <= nodeIndex)
            return;
        MeshNode& node = m_Nodes.at(nodeIndex);

        for (auto& subMeshIndex : node.Submeshes)
        {
            if (!Utils::Contains(subMeshMap, subMeshIndex))
                subMeshMap.emplace_back(subMeshIndex);
        }

        for (auto& childNodes : node.Children)
        {
            EnableNodeSubMeshes(childNodes, subMeshMap);
        }
    }

    std::vector<Vertex> MeshSource::GetVertices() const
    {

        if (!m_Vertices.empty())
            return m_Vertices;
        return m_VertexBuffer->GetDataAs<Vertex>();
    }

    std::vector<Index> MeshSource::GetIndices() const
    {
        if (!m_Indices.empty())
            return m_Indices;
        return m_IndexBuffer->GetDataAs<Index>();
    }


    bool MeshSource::IsCompatibleSkeleton(const uint32_t animationIndex, const SkeletonData& skeleton) const
    {
        if (!m_Skeleton)
        {
            PF_CORE_ASSERT(!m_Runtime);
            if (AssetManager::HasAsset(GetID()))
            {
                auto path = AssetManager::GetAssetFileSystemPath(AssetManager::GetAssetInfo(GetID()).Path);
                MeshImporter importer(path);
                return importer.IsCompatibleSkeleton(animationIndex, skeleton);
            }
            else
            {
                return false;
            }
        }
        return m_Skeleton->GetBoneNames() == skeleton.GetBoneNames();
    }

    uint32_t MeshSource::GetAnimationCount() const
    {
        if (m_Runtime)
            return (uint32_t)m_Animations.size();
        if (AssetManager::HasAsset(GetID()))
        {
            auto path = AssetManager::GetAssetFileSystemPath(AssetManager::GetAssetInfo(GetID()).Path);
            MeshImporter importer(path);
            return importer.GetAnimationCount();
        }
        else
        {
            return false;
        }
    }

    const InternalAnimation& MeshSource::GetAnimation(const uint32_t animationIndex, const SkeletonData& skeleton) const
    {
        if (!m_Skeleton)
        {
            PF_CORE_ASSERT(!m_Runtime);
            if (AssetManager::HasAsset(GetID()))
            {
                auto path = AssetManager::GetAssetFileSystemPath(AssetManager::GetAssetInfo(GetID()).Path);
                MeshImporter importer(path);
                m_Skeleton = CreateSpecial<SkeletonData>(skeleton);
                importer.ImportAnimations(animationIndex, *m_Skeleton, m_Animations);

            }

        }

        PF_CORE_ASSERT(animationIndex < m_Animations.size(), "Animation index out of range!");
        PF_CORE_ASSERT(m_Animations[animationIndex], "Attempted to access null animation!");
        return *m_Animations[animationIndex];
    }

    std::vector<BoneInfluence> MeshSource::GetBoneInfluences() const
    {
        if (!m_BoneInfluences.empty())
            return m_BoneInfluences;

        if (m_BoneInfluenceBuffer == nullptr )
            return std::vector<BoneInfluence>();

        return m_BoneInfluenceBuffer->GetDataAs<BoneInfluence>();
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
        m_MaterialTable = Count<MaterialTable>::CreateFrom(meshSource->GetMaterials());
        SetSubMeshes(subMeshes);
    }
    void Mesh::Reset(const std::string& name, std::vector<Vertex> vertices, std::vector<Index>indices)
    {
        m_MeshSource = Count<MeshSource>::Create(name, vertices, indices);
        m_MaterialTable = Count<MaterialTable>::Create();
        m_MaterialTable = Count<MaterialTable>::CreateFrom(m_MeshSource->GetMaterials());
        SetSubMeshes({});
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

        std::sort(m_SubMeshes.begin(), m_SubMeshes.end());
        ArrangeMaterialTable();
    }

    bool Mesh::HasSubMesh(uint32_t subMeshIndex)
    {
        return std::find(m_SubMeshes.begin(), m_SubMeshes.end(), subMeshIndex) != m_SubMeshes.end();
    }

    AABB Mesh::GetBoundingBox() const
    {
        PF_PROFILE_FUNC();

        const auto& sourceSubMeshes = m_MeshSource->GetSubMeshes();

        PF_CORE_ASSERT(m_SubMeshes[0] < sourceSubMeshes.size());
        PF_CORE_ASSERT(m_MeshSource);

        if (m_SubMeshes.size()  == sourceSubMeshes.size())
        {
            return m_MeshSource->GetBoundingBox();
        }
        AABB boundingBox{};
        bool foundSubMesh = false;

        for (uint32_t subMeshIndex = 0; subMeshIndex < sourceSubMeshes.size(); subMeshIndex++)
        {
            if (!Utils::Contains(m_SubMeshes, subMeshIndex))
                continue;

            const AABB& currentBoundingBox = sourceSubMeshes[subMeshIndex].BoundingBox;

            if (!foundSubMesh)
            {
                boundingBox = currentBoundingBox;
                foundSubMesh = true;
                continue;
            }

            boundingBox.Min = glm::min(boundingBox.Min, currentBoundingBox.Min);
            boundingBox.Max = glm::max(boundingBox.Max, currentBoundingBox.Max);
        }

        PF_CORE_ASSERT(foundSubMesh, "Mesh contains no valid submeshes");

        return boundingBox;
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
        m_MaterialTable = Count<MaterialTable>::CreateFrom(meshSource->GetMaterials());
        SetSubMeshes(subMeshes);
    }

    void DynamicMesh::SetSubMeshesExact(const std::vector<uint32_t>& submeshes)
    {
        m_SubMeshes = submeshes;
        ArrangeMaterialTable();
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

        std::sort(m_SubMeshes.begin(), m_SubMeshes.end());
        ArrangeMaterialTable();

    }
    Count<MaterialTable> DynamicMesh::GetMaterialTableBasedOnSubMeshIndex(uint32_t index)
    {
        if (!HasSubMesh(index))
            return m_MaterialTable;

        auto newTable = Count<MaterialTable>::Create(false);
        newTable->SetMaterial(GetMeshSource()->GetSubMesh(index).MaterialIndex, m_MaterialTable->GetMaterial(GetMeshSource()->GetSubMesh(index).MaterialIndex));

        return newTable;
    }
    bool DynamicMesh::HasSubMesh(uint32_t subMeshIndex)
    {
        return std::find(m_SubMeshes.begin(), m_SubMeshes.end(), subMeshIndex) != m_SubMeshes.end();
    }

    AABB DynamicMesh::GetBoundingBox() const
    {
        PF_PROFILE_FUNC();

        const auto& sourceSubMeshes = m_MeshSource->GetSubMeshes();

        PF_CORE_ASSERT(m_SubMeshes[0] < sourceSubMeshes.size());
        PF_CORE_ASSERT(m_MeshSource);

        if (m_SubMeshes.size()  == sourceSubMeshes.size())
        {
            return m_MeshSource->GetBoundingBox();
        }
        AABB boundingBox{};
        bool foundSubMesh = false;

        for (uint32_t subMeshIndex = 0; subMeshIndex < sourceSubMeshes.size(); subMeshIndex++)
        {
            if (!Utils::Contains(m_SubMeshes, subMeshIndex))
                continue;

            const AABB& currentBoundingBox = sourceSubMeshes[subMeshIndex].BoundingBox;

            if (!foundSubMesh)
            {
                boundingBox = currentBoundingBox;
                foundSubMesh = true;
                continue;
            }

            boundingBox.Min = glm::min(boundingBox.Min, currentBoundingBox.Min);
            boundingBox.Max = glm::max(boundingBox.Max, currentBoundingBox.Max);
        }

        PF_CORE_ASSERT(foundSubMesh, "Mesh contains no valid submeshes");

        return boundingBox;
    }


    std::vector<SubMesh> MeshBase::GetSubMeshesAsSubMesh() 
    {
        std::vector<SubMesh> subMeshes;
        subMeshes.resize(GetSubMeshes().size());

        int index = 0;
        for (auto subMeshIndex : GetSubMeshes())
        {
            subMeshes[index] = GetMeshSource()->GetSubMesh(subMeshIndex);
            index++;
        }
        return subMeshes;
    }

    Count<Mesh> Mesh::CombineMeshes(const std::string& name, const std::vector<Count<Mesh>>& sources)
    {
        std::vector<Vertex> combinedVertices;
        std::vector<Index> combinedIndices;
        std::vector<SubMesh> combinedSubmeshes;

        uint32_t baseVertex = 0;
        uint32_t baseIndex = 0;
        uint32_t subMeshIndex = 0;

        for (auto& source : sources)
        {
            glm::mat4 transform = source->GetTransform();
            auto meshSource = source->GetMeshSource();
            auto& submeshIndices = source->GetSubMeshes();
            auto vertices = meshSource->GetVertices();
            auto indices = meshSource->GetIndices();

            // Apply transform to each vertex
            for (auto& v : vertices)
            {
                Vertex transformed = v;
                transformed.Position = glm::vec3(transform * glm::vec4(v.Position, 1.0f));
                transformed.Normal = glm::normalize(glm::mat3(transform) * v.Normal); // only rotate
                combinedVertices.push_back(transformed);
            }

            for (auto subIndex : submeshIndices)
            {
                const SubMesh& sub = meshSource->GetSubMesh(subIndex);
                SubMesh newSub = sub;
                newSub.BaseVertex = baseVertex;
                newSub.BaseIndice = combinedIndices.size();
                newSub.SubMeshIndex = subMeshIndex++;
                newSub.Transform = transform * sub.Transform;
                newSub.BoundingBox = sub.BoundingBox.ScaleAABB(transform);
                combinedSubmeshes.push_back(newSub);

                for (uint32_t i = 0; i < sub.IndiceCount/3; i++)
                {
                   // PF_ENGINE_INFO(" indices size{}, subMeshIndexCount {}, iterator {}", indices.size(), sub.IndexCount, i);
                    Index oldIndex = indices[sub.BaseIndice + i];
                    Index newIndex = {
                        oldIndex.V1 + baseVertex,
                        oldIndex.V2 + baseVertex,
                        oldIndex.V3 + baseVertex
                    };
                    combinedIndices.push_back(newIndex);
                }
            }

            baseVertex = combinedVertices.size();
            baseIndex = combinedIndices.size();
        }

        return Count<Mesh>::Create(name, combinedVertices, combinedIndices);
    }

    void Mesh::SetSubMeshesExact(const std::vector<uint32_t>& submeshes)
    {
        m_SubMeshes = submeshes;
        ArrangeMaterialTable();
    }

    void MeshBase::SetTransform(const glm::mat4& transform)
    {
        m_MeshMatrix = transform;

        glm::vec3 scale;
        MathResource::DecomposeTransform(transform, m_Translation, m_RotationDeg, scale);
        m_RotationDeg = glm::degrees(m_RotationDeg);

        m_Scale = glm::max(scale.x, glm::max(scale.y, scale.z));
    }

    void MeshBase::ArrangeMaterialTable()
    {
        auto materialTable = GetMaterialTable();
        std::vector<SubMesh> subMeshes = GetSubMeshesAsSubMesh();

        if (materialTable == nullptr)
            return;

        std::unordered_set<uint32_t> materialIndexes;
        for (auto& subMesh : subMeshes)
            materialIndexes.insert({ subMesh.MaterialIndex });

        std::vector<uint32_t> unusedIndex;
        for (auto& [materialIndex, material] : materialTable->GetMaterials())
        {
            if (!materialIndexes.contains(materialIndex))
                unusedIndex.emplace_back(materialIndex);
        }

        for (auto& index : unusedIndex)
        {
            materialTable->RemoveMaterial(index);
        }
    }

}