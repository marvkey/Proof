#pragma once
#include <vector>
#include "Proof/Renderer/Vertex.h"
#include "Proof/Renderer/Texture.h"
#include "Proof/Math/Math.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "Material.h"
#include <algorithm>
namespace Proof
{
    class Material;
    class MaterialTable;
    struct SubMesh {
        std::string Name;
        uint32_t BaseVertex;
        uint32_t BaseIndex;
        uint32_t IndexCount;
        uint32_t MaterialIndex;
        glm::mat4 Transform;
        friend class Renderer3DPBR;
        friend class MeshWorkShop;
    };
    class MeshSource : public Asset{
    public:
        MeshSource();
        MeshSource(const std::string& path);
        MeshSource(const std::string& name, const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices);

        std::filesystem::path GetPath() {
            return m_Path;
        }
        Count<MaterialTable> GenerateMaterialTable(){
            return Count<MaterialTable>::CreateFrom(m_MaterialTable);
        }

        const std::vector<SubMesh>& GetSubMeshes()const  {
            return m_SubMeshes;
        }
        ASSET_CLASS_TYPE(MeshSourceFile);
        Count<class VertexBuffer> GetVertexBuffer() { return m_VertexBuffer; }
        Count<class IndexBuffer> GetIndexBuffer() { return m_IndexBuffer; }
    private:
        // gets cleard after mesh has been created
        std::vector<Vertex> m_Vertices;
        // gets cleard after mesh has been created
        std::vector<uint32_t> m_Indices;

        Count<class VertexBuffer> m_VertexBuffer = nullptr;
        Count<class IndexBuffer> m_IndexBuffer = nullptr;
        void ProcessNode(void* node, const void* scene, const glm::mat4& parentTransform);
        SubMesh ProcessMesh(void* mesh, const void* scene, const glm::mat4& transform) ;

        Count<Material> GetMaterial(void* material);
        void LoadMaterialTextures(Count<Material> material, void* aiMat);
        std::vector<SubMesh> m_SubMeshes;   
        Count<MaterialTable> m_MaterialTable;
        std::filesystem::path m_Path;
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