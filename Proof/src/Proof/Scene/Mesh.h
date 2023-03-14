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
    struct SubMesh {
        SubMesh() {
     
        }
        SubMesh(const std::vector<Vertex>& Vertices, const std::vector<uint32_t>& Indices, const std::string& name, uint32_t materialIndex=0);
        uint32_t MaterialIndex=0;
        std::vector<Vertex> Vertices;
        std::vector<uint32_t> Indices;
        std::string Name;

        Count<class VertexBuffer> VertexBuffer;
        Count<class IndexBuffer> IndexBuffer;

        friend class Renderer3DPBR;
        friend class MeshWorkShop;
    };
    class MeshSource : public Asset{
    public:
        MeshSource() {
            m_MaterialTable = Count<MaterialTable>::Create();
        };
        MeshSource(const std::string& path);
        MeshSource(const std::string& name, const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices);

        std::filesystem::path GetPath() {
            return m_Path;
        }
        Count<MaterialTable> GenerateMaterialTable(){
            return m_MaterialTable->Copy();
        }

        const std::vector<SubMesh>& GetSubMeshes()const  {
            return m_SubMeshes;
        }
        ASSET_CLASS_TYPE(MeshSourceFile);

    private:

        void ProcessNode(void* node, const void* scene);
        SubMesh ProcessMesh(void* mesh, const void* scene);

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
        Mesh(Count<MeshSource> meshSource, const std::vector<uint32_t>& excludeSubMeshes = {});
        Mesh(const std::string& name,std::vector<Vertex> vertices, std::vector<uint32_t>indices);
        ~Mesh() {};
        const std::string& GetName()const{
            return m_Name;
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
        const std::vector<uint32_t>& GetExcludeMeshes() {
            return m_ExcludeMeshes;
        };

        bool IsMeshExcluded(uint32_t index) {
            return std::find(m_ExcludeMeshes.begin(), m_ExcludeMeshes.end(), index) != m_ExcludeMeshes.end();
        }

    private:
        Count<MeshSource> m_MeshSource;
        std::string m_Name; 
        Count<MaterialTable> m_MaterialTable;
        std::vector<uint32_t> m_ExcludeMeshes;
        const UUID m_UiuqeMeshID = UUID();
        friend class Renderer3D;
        friend class Editore3D;
        friend class Renderer3DPBR;
        friend class SceneRendererUI;
        friend class MeshWorkShop;

    };
}