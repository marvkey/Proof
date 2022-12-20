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
        SubMesh(std::vector<Vertex>& Vertices,std::vector<uint32_t>& Indices,const std::string& name, std::vector<uint32_t> diffuseIndex);
        SubMesh(std::vector<Vertex>& Vertices,std::vector<uint32_t>& Indices,const std::string& name);
        bool m_Enabled = true;
        std::string GetName() {
            return m_Name;
        }
        // WE SHOULD NOT BE STORING THIS TEMPORARY
        std::vector<Vertex> m_Vertices;
        std::vector<uint32_t> m_Indices;
        std::string m_Name;

        const std::vector<uint32_t>& GetDiffuseIndex()const {
            return m_DiffuseIndex;
        }
        Count<class VertexBuffer> GetVertexBuffer()const {
            return m_VertexBufferObject;
        }
        Count<class IndexBuffer> GetIndexBuffer()const {
            return m_IndexBufferObject;
        }
    private:
        std::vector<uint32_t> m_DiffuseIndex;
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
        Mesh(std::string const& path){
            LoadModel(path);
        }
        const std::vector<class SubMesh>& GetSubMeshes()const{
            return meshes;
        }
        std::vector<Count<Texture2D>> textures_loaded;
        std::vector<Material> Materials_loaded;
       
        const std::string& GetName()const{
            return m_Name;
        }
        bool m_Enabled=true;

    private:
        std::vector<class SubMesh> meshes;
        std::string m_Name;
        void LoadModel(std::string const& path);
        void ProcessNode(void* node,const void* scene);
        SubMesh ProcessMesh(void* mesh,const void* scene);
        //returns the index of textures in the texutre loaded
        std::vector<uint32_t> LoadMaterialTextures(void* mat,int type,Texture2D::TextureType _TextureType);
        std::vector<Count<Texture2D>> LoadMaterial(void* mat);
        //path, texture type
        std::vector<std::pair<std::string, Texture2D::TextureType>> LoadMaterialTexturesTest(void* mat, int type, Texture2D::TextureType _TextureType);
        friend class Renderer3D;
        friend class Editore3D;
        friend class Renderer3DPBR;
        friend class SceneRendererUI;
        friend class MeshAsset;
        friend class MeshWorkShop;
    };
}