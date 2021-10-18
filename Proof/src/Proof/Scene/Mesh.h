#pragma once
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <vector>
#include "Proof/Renderer/Vertex.h"
#include "Proof/Renderer/Texture.h"
#include "Proof/Resources/Math/Math.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "Mesh.h"
namespace Proof
{
    class SubMesh {
    public:
        SubMesh(std::vector<Vertex>& Vertices,std::vector<uint32_t>& Indices,const std::string& name,std::vector<Proof::Count<class Texture2D>>& Textures);
        SubMesh(std::vector<Vertex>& Vertices,std::vector<uint32_t>& Indices,const std::string& name);
        bool m_Enabled = true;
        std::string GetName() {
            return m_Name;
        }
    private:
        void SetUp();
        std::vector<Vertex> m_Vertices;
        std::vector<uint32_t> m_Indices;
        std::string m_Name;
        Count<class VertexArray> m_VertexArrayObject;
        Count<class VertexBuffer> m_VertexBufferObject;
        Count<class IndexBuffer> m_IndexBufferObject;
        std::vector<Count<class Texture2D>>m_Textures;
        friend class Renderer3DPBR;
    };
    class Mesh {
    public:
        Mesh() =default;
        Mesh(std::string const& path){
            std::vector<uint32_t>::iterator it;
            it = std::find(AllID.begin(),AllID.end(),m_ID);
a:
            m_ID = Math::RandUINT(1,10000000000000);
            if (it == AllID.end()) {
                AllID.emplace_back(m_ID);
            }
            else {
                goto a;
            }
            LoadModel(path);
        }
        static std::vector<uint32_t> AllID;
        std::vector<class SubMesh> GetSubMeshes();
        std::vector<Count<Texture2D>> textures_loaded;
        uint32_t GetID(){
            return m_ID;
        }
        std::string GetName(){
            return m_Name;
        }
        bool m_Enabled=true;
    private:
        std::vector<class SubMesh> meshes;
        std::string m_Name;
        uint32_t m_ID;
        void LoadModel(std::string const& path);
        void ProcessNode(aiNode* node,const aiScene* scene);
        SubMesh ProcessMesh(aiMesh* mesh,const aiScene* scene);
        std::vector<Count<Texture2D>> LoadMaterialTextures(aiMaterial* mat,aiTextureType type,Texture2D::TextureType _TextureType);
        friend class Renderer3D;
        friend class Editore3D;
        friend class Renderer3DPBR;
    };


}