#pragma once
#include <vector>
#include "Proof/Renderer/Vertex.h"
#include "Proof/Renderer/Texture.h"
#include "Proof/Math/Math.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "Material.h"
#include "Proof/Math/AABB.h"
#include <algorithm>
namespace Proof
{
    class Material;
    class MaterialTable;
    struct SubMesh {
        std::string Name,NodeName;
        uint32_t BaseVertex;
        uint32_t BaseIndex;
        uint32_t IndexCount;
        uint32_t VertexCount;
        uint32_t MaterialIndex;
        glm::mat4 LocalTransform;
        glm::mat4 Transform;
        AABB BoundingBox;
        friend class Renderer3DPBR;
        friend class MeshWorkShop;
    };

    struct MeshNode
    {
        uint32_t Parent = 0xffffffff;
        std::vector<uint32_t> Children;
        std::vector<uint32_t> Submeshes;

        std::string Name;
        glm::mat4 LocalTransform;

        inline bool IsRoot() const { return Parent == 0xffffffff; }

        //static void Serialize(StreamWriter* serializer, const MeshNode& instance)
        //{
        //    serializer->WriteRaw(instance.Parent);
        //    serializer->WriteArray(instance.Children);
        //    serializer->WriteArray(instance.Submeshes);
        //    serializer->WriteString(instance.Name);
        //    serializer->WriteRaw(instance.LocalTransform);
        //}
        //
        //static void Deserialize(StreamReader* deserializer, MeshNode& instance)
        //{
        //    deserializer->ReadRaw(instance.Parent);
        //    deserializer->ReadArray(instance.Children);
        //    deserializer->ReadArray(instance.Submeshes);
        //    deserializer->ReadString(instance.Name);
        //    deserializer->ReadRaw(instance.LocalTransform);
        //}
    };
    class MeshSource : public Asset{
    public:
        MeshSource();
        MeshSource(const std::string& name, const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices);
        MeshSource(const std::string& name, const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices, const std::vector<SubMesh>& subMeshes,const std::vector<MeshNode>& nodes, Count<MaterialTable>, AABB boundingBox);

        void Reset(const std::string& name, const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices);
        void Reset(const std::string& name, const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices, const std::vector<SubMesh>& subMeshes, const std::vector<MeshNode>& nodes, Count<MaterialTable> materials,AABB boundingBox);
        AABB GetBoundingBox() {return m_BoundingBox;}
        const std::vector<SubMesh>& GetSubMeshes()const  
        {
            return m_SubMeshes;
        }

        ASSET_CLASS_TYPE(MeshSourceFile);
        Count<class VertexBuffer> GetVertexBuffer() { return m_VertexBuffer; }
        Count<class IndexBuffer> GetIndexBuffer() { return m_IndexBuffer; }

        const std::string& GetName()const { return m_Name; };
        Count<MaterialTable> GetMaterials()const { return m_Materials; };
    private:
        std::string m_Name;
        AABB m_BoundingBox;

        std::vector<MeshNode> m_Nodes;
        Count<class VertexBuffer> m_VertexBuffer = nullptr;
        Count<class IndexBuffer> m_IndexBuffer = nullptr;
        std::vector<SubMesh> m_SubMeshes;   
        Count<MaterialTable> m_Materials;
        friend class MeshImporter;
    };
    
    class Mesh : public Asset
    {
    public:
        ASSET_CLASS_TYPE(Mesh);
        // can leave submeshes empty automaticallly goign to get all meshSOurce aset
        Mesh(Count<MeshSource> meshSource, const std::vector<uint32_t>& subMeshes = {});
        Mesh(const std::string& name,std::vector<Vertex> vertices, std::vector<uint32_t>indices);
        ~Mesh() {};
        const std::string& GetName()const{
            return m_Name;
        }
        void Reset(Count<MeshSource> meshSource, const std::vector<uint32_t>& subMeshes = {});
        void Reset(const std::string& name, std::vector<Vertex> vertices, std::vector<uint32_t>indices);

        void SetName(const std::string& name)
        {
            m_Name = name;
        }
        UUID GetMeshSpecificID() {
            return m_UiuqeMeshID;
        }
        Count<MaterialTable> GetMaterialTable() {
            return m_MaterialTable;
        }
        Count<MeshSource> GetMeshSource() {
            return m_MeshSource;
        }
        const std::vector<uint32_t>& GetSubMeshes()const { return m_SubMeshes; };
        void SetSubMeshes(const std::vector<uint32_t>& submeshes);
    private:
        Count<MeshSource> m_MeshSource;
        std::string m_Name; 
        Count<MaterialTable> m_MaterialTable;
        std::vector<uint32_t> m_SubMeshes;
        const UUID m_UiuqeMeshID = UUID();
        friend class Renderer3D;
        friend class Editore3D;
        friend class Renderer3DPBR;
        friend class SceneRendererUI;
        friend class MeshWorkShop;

    };
}