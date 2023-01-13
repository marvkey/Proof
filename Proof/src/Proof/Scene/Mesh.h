#pragma once
#include <vector>
#include "Proof/Renderer/Vertex.h"
#include "Proof/Renderer/Texture.h"
#include "Proof/Math/Math.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "Material.h"
namespace Proof
{
    struct SubMesh {
    public:
        SubMesh(std::vector<Vertex>& Vertices,std::vector<uint32_t>& Indices,const std::string& name, std::vector<AssetID> diffuseTextures);
        SubMesh(std::vector<Vertex>& Vertices,std::vector<uint32_t>& Indices,const std::string& name);
        bool Enabled = true;
        std::string GetName() {
            return m_Name;
        }
        // WE SHOULD NOT BE STORING THIS TEMPORARY
        std::vector<Vertex> m_Vertices;
        std::vector<uint32_t> m_Indices;
        std::string m_Name;

        const std::vector<AssetID>& GetDiffuseTextures()const {
            return m_DiffuseAssets;
        }
        Count<class VertexBuffer> GetVertexBuffer()const {
            return m_VertexBufferObject;
        }
        Count<class IndexBuffer> GetIndexBuffer()const {
            return m_IndexBufferObject;
        }
    private:
        std::vector<AssetID> m_DiffuseAssets;
        Count<class VertexBuffer> m_VertexBufferObject;
        Count<class IndexBuffer> m_IndexBufferObject;
        void SetUp();
        friend class Renderer3DPBR;
        friend class MeshWorkShop;
    };
    class Mesh {
    public:
      
        bool m_FaceCulling =false;
        Mesh() =default;
        // exclude index is not gonna have that mesh ready to be rendered that specific mesh will be discarded
        // so only use it if u dont want tthe mesh to eexist
        Mesh(std::string const& path ) {
            LoadModel(path);
        }
        virtual ~Mesh() {};
        const std::vector<SubMesh>& GetSubMeshes()const {
            return meshes;
        }
        
       
        const std::string& GetName()const{
            return m_Name;
        }
        bool Enabled=true;

    private:
        std::vector<class SubMesh> meshes;
        std::vector<std::string> m_MaterialsName;
        std::string m_Name;
        struct TextureData {
            AssetID Texture;
            AssetID TextureSource;
        };

        //stores all teh asset source of a given mesh
        std::vector<TextureData> m_Textures;
        std::filesystem::path m_Path;
        void LoadModel(std::string const& path);
        void ProcessNode(void* node, const void* scene);
        SubMesh ProcessMesh(void* mesh,const void* scene);
        //returns the id of  of textures in the texutre loaded
        std::vector<AssetID> LoadMaterialTextures(void* mat,int type);
        std::vector<Material> LoadMaterial(void* mat);
        friend class Renderer3D;
        friend class Editore3D;
        friend class Renderer3DPBR;
        friend class SceneRendererUI;
        friend class MeshAsset;
        friend class MeshWorkShop;
    };
}