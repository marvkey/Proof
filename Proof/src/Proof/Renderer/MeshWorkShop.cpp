#include "Proofprch.h"
#include "Proof/Core/Core.h"

#include "MeshWorkShop.h"
#include<vector>
namespace Proof {
    // code was referenced from this article
    //http://www.songho.ca/opengl/gl_sphere.html#sphere
    Count<Mesh> MeshWorkShop::GenerateCube()
    {
        //https://pastebin.com/DXKEmvap
        std::vector<Vector> veritces =
        {

            Vector(-1, -1, -1),
            Vector(1, -1, -1),
            Vector(1, 1, -1),
            Vector(-1, 1, -1),
            Vector(-1, -1, 1),
            Vector(1, -1, 1),
            Vector(1, 1, 1),
            Vector(-1, 1, 1)
        };

        std::vector<Vector2> texCoords =
        {
            Vector2(0, 0),
            Vector2(1, 0),
            Vector2(1, 1),
            Vector2(0, 1)
        };

        std::vector<Vector> normals =
        {
            Vector(0, 0, 1),
            Vector(1, 0, 0),
            Vector(0, 0, -1),
            Vector(-1, 0, 0),
            Vector(0, 1, 0),
            Vector(0, -1, 0)
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
        for (uint32_t i = 0; i < veritces.size(); i++)
        {
            Vertex vertex;
            vertex.Vertices = Vector(veritces[i].X, veritces[i].Y, veritces[i].Z);
            if (normals.size() > i)
                vertex.Normal = Vector(normals[i].X, normals[i].Y, normals[i].Z);
            if (texCoords.size() > i)
            {
                vertex.TexCoords = texCoords[i];
                //  vertex.Tangent = Vector(mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z);
                //vertex.Bitangent = Vector(mesh->mBitangents[i].x, mesh->mBitangents[i].y, mesh->mBitangents[i].z);
            }
            cubeVertex.emplace_back(vertex);
        }
        return Count<Mesh>::Create("Cube", cubeVertex, indices);
    }
    constexpr int MIN_SECTOR_COUNT = 3;
    constexpr int MIN_STACK_COUNT = 2;
    Count<Mesh> MeshWorkShop::GenerateUVSphere(float radius, float sectors, float stacks)
    {
        if (sectors < MIN_SECTOR_COUNT)
        {
            PF_CORE_ASSERT(false, "secto count cannot be less than 3");
            sectors = MIN_SECTOR_COUNT;
        }
        if (stacks < MIN_STACK_COUNT)
        {
            PF_CORE_ASSERT(false, "stack  count cannot be less than 2");
            stacks = MIN_STACK_COUNT;
        }
        // code was referenced from this article
        //http://www.songho.ca/opengl/gl_sphere.html#sphere

        // this is generating a smooth surace sphere 
        // in the future go to the site to generate a sphere wich surface is not smooth
        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;
        float sectorStep = 2 * Math::PIE() / sectors;
        float stackStep = Math::PIE() / stacks;
        float sectorAngle, stackAngle;
        float xy;
        float lengthInv = 1.0f / radius;
        for (int i = 0; i <= stacks; ++i)
        {
            Vertex vertex;
            stackAngle = Math::PIE() / 2 - i * stackStep;        // starting from pi/2 to -pi/2
            xy = radius * cosf(stackAngle);             // r * cos(u)
            vertex.Vertices.Z = radius * sinf(stackAngle);              // r * sin(u)

            for (int j = 0; j <= sectors; ++j)
            {
                sectorAngle = j * sectorStep;           // starting from 0 to 2pi

                // vertex position
                vertex.Vertices.X = xy * cosf(sectorAngle);             // r * cos(u) * cos(v)
                vertex.Vertices.Y = xy * sinf(sectorAngle);             // r * cos(u) * sin(v)

                // normalized vertex normal
                vertex.Normal.X = vertex.Vertices.X * lengthInv;
                vertex.Normal.Y = vertex.Vertices.Y * lengthInv;
                vertex.Normal.Z = vertex.Vertices.Z * lengthInv;

                // vertex tex coord between [0, 1]
                vertex.TexCoords.X = (float)j / sectors;
                vertex.TexCoords.Y = (float)i / stacks;
                vertices.emplace_back(vertex);
            }
        }
        //indices
        {
            uint32_t k1, k2;
            for (int i = 0; i < stacks; ++i)
            {
                k1 = i * (sectors + 1);     // beginning of current stack
                k2 = k1 + sectors + 1;      // beginning of next stack

                for (int j = 0; j < sectors; ++j, ++k1, ++k2)
                {
                    // 2 triangles per sector excluding 1st and last stacks
                    if (i != 0)
                    {
                        indices.emplace_back(k1);
                        indices.emplace_back(k2);
                        indices.emplace_back(k1 + 1);
                    }

                    if (i != (stacks - 1))
                    {
                        indices.emplace_back(k1 + 1);
                        indices.emplace_back(k2);
                        indices.emplace_back(k1 + 1);
                    }
                }
            }
        }
        return Count<Mesh>::Create("Sphere", vertices, indices);
    }
    Count<Mesh> MeshWorkShop::GenerateCapsule(float radius, float height, uint32_t numSides)
    {
        /*
        // generate the texture coordinates for the two circular endcaps
        for (int i = 0; i <= numSides; i++)
        {
            texCoords.push_back((float)i / numSides);
            texCoords.push_back(1);
        }
        for (int i = 0; i <= numSides; i++)
        {
            texCoords.push_back((float)i / numSides);
            texCoords.push_back(0);
        }
        */
        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;
        auto GenerateSemiSphere = [&](float x,float y, float z, bool topSphere) {
            Vertex vertex;
            for (int i = 0; i <= numSides; i++)
            {
                float angle = 2 * Math::PIE() * i / numSides;
                vertex.Vertices.X = x + radius * cos(angle);
                vertex.Vertices.Y = y;
                vertex.Vertices.Z = x + radius * sin(angle);
                vertex.TexCoords.X = (float)i / numSides;
                if (topSphere)
                    vertex.TexCoords.Y = 1;
                else
                    vertex.TexCoords.Y = 0;
                vertices.emplace_back(vertex);
            }
        };

        GenerateSemiSphere(0, height / 2, 0,true);
        GenerateSemiSphere(0, -height / 2, 0,false);
        // indices for 2 circular parts
        for (int i = 0; i < numSides; i++)
        {
            indices.emplace_back(i);
            indices.emplace_back(i + 1);
            indices.emplace_back(i + numSides + 1);

            indices.emplace_back(i + numSides + 1);
            indices.emplace_back(i + 1);
            indices.emplace_back(i + numSides + 2);
        }

            // generate the indices for the cylinder part
        for (int i = 0; i < numSides; i++)
        {
            indices.emplace_back(i);
            indices.emplace_back(i + numSides + 1);
            indices.emplace_back((i + 1) % numSides);

            indices.emplace_back((i + 1) % numSides);
            indices.emplace_back(i + numSides + 1);
            indices.emplace_back((i + 1) % numSides + numSides + 1);
        }
        return Count<Mesh>::Create("Capsule",vertices,indices);
    }
}