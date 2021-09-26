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
    class Shader;
    class Model {
    public:
        bool gammaCorrection;
        Model() =default;
        Model(std::string const& path,bool gamma = false): gammaCorrection(gamma) {
            LoadModel(path);
           std::vector<uint32_t>::iterator it;
           it = std::find(AllID.begin(),AllID.end(),ID);
           a:
           ID = Math::RandUINT(1,100000);
           if(it ==AllID.end()){
               AllID.emplace_back(ID);
           }
           else{
               goto a;
           }
           PF_ENGINE_INFO("my ID IS %i",ID);
           SetUpModel();

        }
        ~Model(){
            PF_ENGINE_INFO("DELETE");
        }
        void Draw(const Count<Shader>& shader);
        std::vector<class Mesh> GetMesh()const;
        std::vector<Vertex> m_Vertices;
        std::vector<uint32_t> m_Indices;

        Count<class VertexArray> m_VertexArrayObject;
        Count<class VertexBuffer> m_VertexBufferObject;
        Count<class IndexBuffer> m_IndexBufferObject;
        std::vector<Count<Texture2D>> textures_loaded;

        uint32_t GetID(){return ID;};
        
        void LoadModelOneMesh();
        uint32_t Offset =0;
    private:
        std::vector<class Mesh> meshes;
        static std::vector<uint32_t> AllID;
        void LoadModel(std::string const& path);
        void ProcessNode(aiNode* node,const aiScene* scene);
        Mesh ProcessMesh(aiMesh* mesh,const aiScene* scene);
        std::vector<Count<Texture2D>> LoadMaterialTextures(aiMaterial* mat,aiTextureType type,Texture2D::TextureType _TextureType);
        void SetUpModel();
        uint32_t ID =0;
    };
}