#include "Proofprch.h"
#include "MeshWorkShop.h"
#include<vector>
namespace Proof {
    Count<Mesh> MeshWorkShop::m_Cube = CreateCount<Mesh>();
	void MeshWorkShop::Init(){
        InitCube();
	}
	void MeshWorkShop::InitCube(){
        std::vector<Vector<float>> veritces = 
        {
            Vector<float>(-0.5, -0.5, -0.5),
            Vector<float>(0.5, -0.5, -0.5),
            Vector<float>(0.5, 0.5, -0.5),
            Vector<float>(-0.5, 0.5, -0.5),
            Vector<float>(-0.5, -0.5, 0.5),
            Vector<float>(0.5, -0.5, 0.5),
            Vector<float>(0.5, 0.5, 0.5),
            Vector<float>(-0.5, 0.5, 0.5)
        };

        std::vector<glm::vec2> texCoords =
        {
            glm::vec2(0, 0),
            glm::vec2(1, 0),
            glm::vec2(1, 1),
            glm::vec2(0, 1)
        };

        std::vector<Vector<float>> normals =
        {
            Vector<float>(0, 0, 1),
            Vector<float>(1, 0, 0),
            Vector<float>(0, 0, -1),
            Vector<float>(-1, 0, 0),
            Vector<float>(0, 1, 0),
            Vector<float>(0, -1, 0)
        };

        std::vector<uint32_t> indices =
        {
            0, 1, 3, 3, 1, 2,
            1, 5, 2, 2, 5, 6,
            5, 4, 6, 6, 4, 7,
            4, 0, 7, 7, 0, 3,
            3, 2, 7, 7, 2, 6,
            4, 5, 0, 0, 5, 1
        };
        std::vector<Vertex> cubeVertex;
        for (unsigned int i = 0; i < veritces.size(); i++) {
            Vertex vertex;
            vertex.Vertices = Vector(veritces[i].X, veritces[i].Y, veritces[i].Z);
            if (normals.size() > i)
                vertex.Normal = Vector(normals[i].X, normals[i].Y, normals[i].Z);
            if (texCoords.size() >i) {
                vertex.TexCoords = glm::vec2(texCoords[i].x, texCoords[i].y);
              //  vertex.Tangent = Vector(mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z);
                //vertex.Bitangent = Vector(mesh->mBitangents[i].x, mesh->mBitangents[i].y, mesh->mBitangents[i].z);
            }
            cubeVertex.emplace_back(vertex);
        }

        //m_Cube->meshes.emplace_back(SubMesh(cubeVertex,indices,"cube"));
        m_Cube = CreateCount<Mesh>("cube.obj");
	}
}