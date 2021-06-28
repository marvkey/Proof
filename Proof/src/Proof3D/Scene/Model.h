#pragma once
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Proof/Renderer/Texture.h"
#include <vector>
#include "Mesh.h"
namespace Proof {
    class Shader;
    class Model {
    public:
        bool gammaCorrection;
        Model(std::string const& path,bool gamma = false): gammaCorrection(gamma) {
            LoadModel(path);
        }
        void Draw(const Count<Shader>& shader);
        std::vector<class Mesh> GetMesh()const { return meshes; }
    private:
        std::vector<class Mesh> meshes;
        std::vector<Count<Texture2D>> textures_loaded;
        void LoadModel(std::string const& path);
        void ProcessNode(aiNode* node,const aiScene* scene);
        Mesh ProcessMesh(aiMesh* mesh,const aiScene* scene);
        std::vector<Count<Texture2D>> LoadMaterialTextures(aiMaterial* mat,aiTextureType type,Texture2D::TextureType _TextureType);
    };
}