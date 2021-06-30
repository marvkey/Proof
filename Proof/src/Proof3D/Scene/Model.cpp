#include "Proofprch.h"
#include "Model.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Component.h"
#include "Proof/Renderer/Texture.h"
#include "Proof/Renderer/Shader.h"
#include "Proof/Renderer/RendererCommand.h"

#include <string>
#include <fstream>
#include <sstream>
#include "Mesh.h"

namespace Proof{
    void Model::Draw(const Count<Shader>& shader) {
        for (Mesh& Mesh : meshes)
            Mesh.Draw(shader);
    }
    std::vector<Mesh> Model::GetMesh() const {
        return meshes;
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
            m_Vertices.emplace_back(vertex);
        }
        for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
            aiFace& face = mesh->mFaces[i];
            for (unsigned int j = 0; j < face.mNumIndices; j++) {
                indices.emplace_back(face.mIndices[j]);
                m_Indices.emplace_back(face.mIndices[j]);
            }
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
    void Model::SetUpModel() {
        m_VertexArrayObject = VertexArray::Create();
        m_VertexBufferObject = VertexBuffer::Create(&m_Vertices[0],m_Vertices.size() * sizeof(Vertex));
        m_IndexBufferObject = IndexBuffer::Create(&m_Indices[0],m_Indices.size());
        m_VertexBufferObject->Bind();
        m_IndexBufferObject->Bind();
        m_VertexArrayObject->AttachIndexBuffer(m_IndexBufferObject);
        m_VertexArrayObject->AddData(0,3,sizeof(Vertex),(void*)offsetof(Vertex,Vertices));
        m_VertexArrayObject->AddData(1,3,sizeof(Vertex),(void*)offsetof(Vertex,Normal));
        m_VertexArrayObject->AddData(2,2,sizeof(Vertex),(void*)offsetof(Vertex,TexCoords));
        m_VertexArrayObject->AddData(3,3,sizeof(Vertex),(void*)offsetof(Vertex,Tangent));
        m_VertexArrayObject->AddData(4,3,sizeof(Vertex),(void*)offsetof(Vertex,Bitangent));
        m_VertexArrayObject->UnBind();
    }
    void Model::load_obj(const std::string& filename,std::vector<Vertex>& vertices,std::vector<uint32_t>& Indices,std::vector<Count<Texture2D>> Textures) {
        std::string Line;
        std::ifstream OBJFile(filename);
        std::stringstream ss;
        std::vector<Vector> v_Vertices;
        std::vector<Vector> Normals;
        std::vector<glm::vec2> TexCoords;
        std::vector<uint32_t>v_Indices;

        if (OBJFile.is_open() == true) {
            while (std::getline(OBJFile,Line)) {
                ss.clear();
                ss.str(Line);
                ss >> Line[0];
                Vector TempVertices{0,0,0};
                Vector TempNormals{0,0,0};
                glm::vec2 TempTexCoords{0,0};
                if (Line[0] == 'v') {
                    ss >> TempVertices.X >> TempVertices.Y >> TempVertices.Z;
                    v_Vertices.emplace_back(TempVertices);
                }
                if (Line[0] == 'v' && Line[1] == 't') {
                    ss.clear();
                    ss.str(Line);
                    ss >> Line[0];
                    ss >> Line[1];
                    ss >> TempTexCoords.x >> TempTexCoords.y;
                    TexCoords.emplace_back(TempTexCoords);
                }

                if (Line[0] == 'v' && Line[1] == 'n') {
                    ss.clear();
                    ss.str(Line);
                    ss >> Line[0];
                    ss >> Line[1];
                    ss >> TempNormals.X >> TempNormals.Y >> TempNormals.Z;
                    Normals.emplace_back(TempNormals);
                }

                if(Line[0] =='f'){
                    std::istringstream s(Line.substr(2));
                    char Temp;
                    uint32_t Element1 = 0;
                    uint32_t Element2 = 0;
                    uint32_t Element3 = 0;

                    uint32_t Element4 = 0;
                    uint32_t Element5 = 0;
                    uint32_t Element6 = 0;

                    uint32_t Element7 = 0;
                    uint32_t Element8 = 0;
                    uint32_t Element9 = 0;
                    // THIS CODE DOES NOT MAKE SENSE BUT IT WORKS LEAVE IT 
                    s>>Element1>>Temp>>Element2>>Temp>>Element3>>Element4>>Temp>>Element5>>Temp>>Element6>>Element7>>Temp>>Element8>>Temp>>Element9;

                    Indices.emplace_back(Element1);
                    Indices.emplace_back(Element2);
                    Indices.emplace_back(Element3);

                    Indices.emplace_back(Element4);
                    Indices.emplace_back(Element5);
                    Indices.emplace_back(Element6);

                    Indices.emplace_back(Element7);
                    Indices.emplace_back(Element8);
                    Indices.emplace_back(Element9);

                }
            }
        }
    }
    void Model::ProcessVertex(uint32_t Element1,uint32_t Element2,uint32_t Element3,std::vector<uint32_t>& Indices,std::vector<glm::vec2>& textureCoords,std::vector<Vector> Normals,std::vector<Vertex> vertex) {
        int CurrenteVertexPointer =Element1-1;
        Indices.emplace_back(CurrenteVertexPointer);
        glm::vec2 CurrentTex =textureCoords[Element2-1];

        //vertex[CurrenteVertexPointer*2].TexCoords = CurrentTex.x;
        //vertex[CurrenteVertexPointer * 2] = CurrentTex.x;
    }
}