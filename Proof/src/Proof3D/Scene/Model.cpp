#include "Proofprch.h"
#include "Model.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include "Mesh.h"
namespace Proof {
    void Model::Draw(const Count<Shader>& shader) {
        for (Mesh& Mesh:meshes)
            Mesh.Draw(shader);
    }
    void Model::LoadModel(std::string const& path) {
        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(path,aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
        {
            std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
            return;
        }
        ProcessNode(scene->mRootNode,scene);
    }
    void Model::ProcessNode(aiNode* node,const aiScene* scene) {
        for (unsigned int i = 0; i < node->mNumMeshes; i++) {
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            meshes.emplace_back(ProcessMesh(mesh,scene));
        }
        for (unsigned int i = 0; i < node->mNumChildren; i++) {
            ProcessNode(node->mChildren[i],scene);
        }
    }
    Mesh Model::ProcessMesh(aiMesh* mesh,const aiScene* scene) {
        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;
        std::vector<Count<Texture2D>> textures;
        for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
            Vertex vertex;
            vertex.Vertices = Vector(mesh->mVertices[i].x,mesh->mVertices[i].y,mesh->mVertices[i].z);
            if (mesh->HasNormals())
                vertex.Normal = Vector(mesh->mNormals[i].x,mesh->mNormals[i].y,mesh->mNormals[i].z);
            if (mesh->mTextureCoords[0]) {
                vertex.TexCoords = glm::vec2(mesh->mTextureCoords[0][i].x,mesh->mTextureCoords[0][i].y);
                vertex.Tangent = Vector(mesh->mTangents[i].x,mesh->mTangents[i].y,mesh->mTangents[i].z);
                vertex.Bitangent = Vector(mesh->mBitangents[i].x,mesh->mBitangents[i].y,mesh->mBitangents[i].z);
            }
            else
                vertex.TexCoords = glm::vec2(0.0f,0.0f);
            vertices.emplace_back(vertex);
        }
        for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
            aiFace face = mesh->mFaces[i];
            for (unsigned int j = 0; j < face.mNumIndices; j++)
                indices.emplace_back(face.mIndices[j]);
        }
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

        std::vector<Count<Texture2D>>  diffuseMaps = LoadMaterialTextures(material,aiTextureType_DIFFUSE,Texture2D::TextureType::Diffuse);
        textures.insert(textures.end(),diffuseMaps.begin(),diffuseMaps.end());

        std::vector<Count<Texture2D>>  specularMaps = LoadMaterialTextures(material,aiTextureType_SPECULAR,Texture2D::TextureType::Specular);
        textures.insert(textures.end(),specularMaps.begin(),specularMaps.end());

        std::vector<Count<Texture2D>> normalMaps = LoadMaterialTextures(material,aiTextureType_NORMALS,Texture2D::TextureType::Normal);
        textures.insert(textures.end(),normalMaps.begin(),normalMaps.end());

        std::vector<Count<Texture2D>>  heightMaps = LoadMaterialTextures(material,aiTextureType_HEIGHT,Texture2D::TextureType::Height);
        textures.insert(textures.end(),heightMaps.begin(),heightMaps.end());

        return Mesh(vertices,indices,textures);
    }

    std::vector<Count<Texture2D>> Model::LoadMaterialTextures(aiMaterial* mat,aiTextureType type,Texture2D::TextureType _TextureType) {
        std::vector<Count<Texture2D>>  Textures;
        for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
            aiString str;
            mat->GetTexture(type,i,&str);
            bool skip = false;
            for (unsigned int j = 0; j < textures_loaded.size(); j++) {
                if (std::strcmp(textures_loaded[j]->GetPath().data(),str.C_Str()) == 0) {
                    Textures.emplace_back(textures_loaded[j]);
                    skip = true;
                    break;
                }
            }
            if (!skip) {
                Count<Texture2D> NewTexture = Texture2D::Create(str.C_Str(),_TextureType);
                Textures.emplace_back(NewTexture);
                textures_loaded.emplace_back(NewTexture);
            }
        }
        return Textures;
    }
}