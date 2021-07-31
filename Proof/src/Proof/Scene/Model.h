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
            //load_obj(path,m_Vertices,m_Indices,textures_loaded);
            //PF_ENGINE_INFO("Number of Vertices %i",m_Vertices.size());
            //PF_ENGINE_INFO("Number of Indices %i",m_Indices.size());


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
        /*
        void load_obje(const char* filename,std::vector<Vertex>& vertices,std::vector<uint32_t>& elements) {
            std::ifstream in(filename,std::ios::in);
            if (!in)     {
                std::cerr << "Cannot open " << filename << std::endl; exit(1);
            }

            std::string line;
            while (std::getline(in,line))     {
                if (line.substr(0,2) == "v ")         {
                    std::istringstream s(line.substr(2));
                    Vertex v; s >> v.Vertices.X; s >> v.Vertices.Y; s >> v.Vertices.Z;
                    vertices.push_back(v);
                }
                else if (line.substr(0,2) == "f ")         {
                    std::istringstream s(line.substr(2));
                    GLushort a,b,c;
                    s >> a; s >> b; s >> c;
                    a--; b--; c--;
                    elements.push_back(a); elements.push_back(b); elements.push_back(c);
                }
            }
        }
        */
        void Draw(const Count<Shader>& shader);
        std::vector<class Mesh> GetMesh()const;
        std::vector<Vertex> m_Vertices;
        std::vector<uint32_t> m_Indices;

        Count<class VertexArray> m_VertexArrayObject;
        Count<class VertexBuffer> m_VertexBufferObject;
        Count<class IndexBuffer> m_IndexBufferObject;
        std::vector<Count<Texture2D>> textures_loaded;

        uint32_t Offset =0;
        uint32_t GetID(){return ID;};

        void LoadModelOneMesh();
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