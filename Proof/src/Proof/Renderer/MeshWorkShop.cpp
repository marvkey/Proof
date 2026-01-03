#include "Proofprch.h"
#include "Proof/Core/Core.h"
#include "Proof/Scene/Mesh.h"
#include "MeshWorkShop.h"
#include "Proof/Renderer/Vertex.h"

#include<vector>
namespace Proof {
    //https://danielsieger.com/blog/2021/05/03/generating-primitive-shapes.html (//View in the fute to develo better)
    //https://github.com/SurgeTechnologies/Electro/blob/8a7dfb6aabdf5d2e6fe31a8d9c976607fc2f1394/Electro/src/Renderer/MeshFactory.cpp
    // code was referenced from this article
    //http://www.songho.ca/opengl/gl_sphere.html#sphere

    //https://www.songho.ca/opengl/gl_cylinder.html (//for cone and cylinder, sphere) download the zip file  com

    //TODO ADD ALready have smooth shading implement flat shading https://www.songho.ca/opengl/gl_cylinder.html  already has flat shaidng for cylinder, cone and sphere
    Count<Mesh> MeshWorkShop::GenerateCube(const glm::vec3& size)
    {
    #if 0
        //https://github.com/kidrigger/Blaze/blob/7e76de71e2e22f3b5e8c4c2c50c58e6d205646c6/Blaze/Primitives.cpp
        std::vector<Vertex> vertices;
        vertices.resize(8);
        vertices[0].Position = { -size.x , -size.y ,  size.z  };
        vertices[1].Position = { size.x , -size.y ,  size.z  };
        vertices[2].Position = { size.x ,  size.y ,  size.z  };
        vertices[3].Position = { -size.x ,  size.y ,  size.z  };
        vertices[4].Position = { -size.x , -size.y , -size.z  };
        vertices[5].Position = { size.x , -size.y , -size.z  };
        vertices[6].Position = { size.x ,  size.y , -size.z  };
        vertices[7].Position = { -size.x ,  size.y , -size.z  };

        vertices[0].Normal = { -1.0F, -1.0F,  1.0F };
        vertices[1].Normal = { 1.0F, -1.0F,  1.0F };
        vertices[2].Normal = { 1.0F,  1.0F,  1.0F };
        vertices[3].Normal = { -1.0F,  1.0F,  1.0F };
        vertices[4].Normal = { -1.0F, -1.0F, -1.0F };
        vertices[5].Normal = { 1.0F, -1.0F, -1.0F };
        vertices[6].Normal = { 1.0F,  1.0F, -1.0F };
        vertices[7].Normal = { -1.0F,  1.0F, -1.0F };

        // Texture Coordinates (UVs)
        vertices[0].TexCoord = { 0.0f, 0.0f };
        vertices[1].TexCoord = { 1.0f, 0.0f };
        vertices[2].TexCoord = { 1.0f, 1.0f };
        vertices[3].TexCoord = { 0.0f, 1.0f };
        vertices[4].TexCoord = { 0.0f, 0.0f };
        vertices[5].TexCoord = { 1.0f, 0.0f };
        vertices[6].TexCoord = { 1.0f, 1.0f };
        vertices[7].TexCoord = { 0.0f, 1.0f };

        std::vector<Index> indices =
        {
            {0, 1, 2,}, {2, 3, 0,},
            {1, 5, 6,}, { 6, 2, 1,},
            {7, 6, 5,}, {5, 4, 7,},
            {4, 0, 3,}, {3, 7, 4,},
            {4, 5, 1,}, {1, 0, 4,},
            {3, 2, 6,}, {6, 7, 3},
        };
    #else
        const glm::vec3& s = size;

        std::vector<Vertex> vertices = {
           {{-s.x, -s.y,  s.z}, {0, 0, 1}, {0, 0}, {1, 0, 0}, {0, 1, 0}},
        {{ s.x, -s.y,  s.z}, {0, 0, 1}, {1, 0}, {1, 0, 0}, {0, 1, 0}},
        {{ s.x,  s.y,  s.z}, {0, 0, 1}, {1, 1}, {1, 0, 0}, {0, 1, 0}},
        {{-s.x,  s.y,  s.z}, {0, 0, 1}, {0, 1}, {1, 0, 0}, {0, 1, 0}},

        // BACK (-Z)
        {{ s.x, -s.y, -s.z}, {0, 0, -1}, {0, 0}, {-1, 0, 0}, {0, 1, 0}},
        {{-s.x, -s.y, -s.z}, {0, 0, -1}, {1, 0}, {-1, 0, 0}, {0, 1, 0}},
        {{-s.x,  s.y, -s.z}, {0, 0, -1}, {1, 1}, {-1, 0, 0}, {0, 1, 0}},
        {{ s.x,  s.y, -s.z}, {0, 0, -1}, {0, 1}, {-1, 0, 0}, {0, 1, 0}},

        // LEFT (-X)
        {{-s.x, -s.y, -s.z}, {-1, 0, 0}, {0, 0}, {0, 0, 1}, {0, 1, 0}},
        {{-s.x, -s.y,  s.z}, {-1, 0, 0}, {1, 0}, {0, 0, 1}, {0, 1, 0}},
        {{-s.x,  s.y,  s.z}, {-1, 0, 0}, {1, 1}, {0, 0, 1}, {0, 1, 0}},
        {{-s.x,  s.y, -s.z}, {-1, 0, 0}, {0, 1}, {0, 0, 1}, {0, 1, 0}},

        // RIGHT (+X)
        {{ s.x, -s.y,  s.z}, {1, 0, 0}, {0, 0}, {0, 0, -1}, {0, 1, 0}},
        {{ s.x, -s.y, -s.z}, {1, 0, 0}, {1, 0}, {0, 0, -1}, {0, 1, 0}},
        {{ s.x,  s.y, -s.z}, {1, 0, 0}, {1, 1}, {0, 0, -1}, {0, 1, 0}},
        {{ s.x,  s.y,  s.z}, {1, 0, 0}, {0, 1}, {0, 0, -1}, {0, 1, 0}},

        // TOP (+Y)
        {{-s.x,  s.y,  s.z}, {0, 1, 0}, {0, 0}, {1, 0, 0}, {0, 0, -1}},
        {{ s.x,  s.y,  s.z}, {0, 1, 0}, {1, 0}, {1, 0, 0}, {0, 0, -1}},
        {{ s.x,  s.y, -s.z}, {0, 1, 0}, {1, 1}, {1, 0, 0}, {0, 0, -1}},
        {{-s.x,  s.y, -s.z}, {0, 1, 0}, {0, 1}, {1, 0, 0}, {0, 0, -1}},

        // BOTTOM (-Y)
        {{-s.x, -s.y, -s.z}, {0, -1, 0}, {0, 0}, {1, 0, 0}, {0, 0, 1}},
        {{ s.x, -s.y, -s.z}, {0, -1, 0}, {1, 0}, {1, 0, 0}, {0, 0, 1}},
        {{ s.x, -s.y,  s.z}, {0, -1, 0}, {1, 1}, {1, 0, 0}, {0, 0, 1}},
        {{-s.x, -s.y,  s.z}, {0, -1, 0}, {0, 1}, {1, 0, 0}, {0, 0, 1}},
        };

        std::vector<Index> indices = {
            // Front
            {0, 1, 2}, {2, 3, 0},
            // Back
            {4, 5, 6}, {6, 7, 4},
            // Left
            {8, 9, 10}, {10, 11, 8},
            // Right
            {12, 13, 14}, {14, 15, 12},
            // Top
            {16, 17, 18}, {18, 19, 16},
            // Bottom
            {20, 21, 22}, {22, 23, 20}
        };
    #endif
        return Count<Mesh>::Create("Cube", vertices, indices);

    }
  
    Count<Mesh> MeshWorkShop::GenerateSphere(float radius, float sectors, float stacks)
    {
        constexpr int MIN_SECTOR_COUNT = 3;
        constexpr int MIN_STACK_COUNT = 2;
        if (sectors < MIN_SECTOR_COUNT)
        {
            PF_CORE_ASSERT(false, "sector count cannot be less than 3");
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
        std::vector<Index> indices;

        float x, y, z, xy;                              // vertex position
        float nx, ny, nz, lengthInv = 1.0f / radius;    // normal
        float s, t;                                     // texCoord

        float sectorStep = 2 * Math::PIE() / sectors;
        float stackStep = Math::PIE() / stacks;
        float sectorAngle, stackAngle;

        for (int i = 0; i <= stacks; ++i)
        {
            stackAngle = Math::PIE() / 2 - i * stackStep;        // starting from pi/2 to -pi/2
            xy = radius * cosf(stackAngle);             // r * cos(u)
            z = radius * sinf(stackAngle);              // r * sin(u)

            // add (sectorCount+1) vertices per stack
            // the first and last vertices have same position and normal, but different tex coords
            for (int j = 0; j <= sectors; ++j)
            {
                sectorAngle = j * sectorStep;           // starting from 0 to 2pi
                Vertex& vertex = vertices.emplace_back();
                // vertex position
                x = xy * cosf(sectorAngle);             // r * cos(u) * cos(v)
                y = xy * sinf(sectorAngle);             // r * cos(u) * sin(v)
                vertex.Position = { x,y,z };
                // normalized vertex normal
                nx = x * lengthInv;
                ny = y * lengthInv;
                nz = z * lengthInv;
                vertex.Normal = { nx, ny, nz };
                // vertex tex coord between [0, 1]
                s = (float)j / sectors;
                t = (float)i / stacks;
                vertex.TexCoord = { s,t };
            }
        }

        // indices
        //  k1--k1+1
        //  |  / |
        //  | /  |
        //  k2--k2+1
        unsigned int k1, k2;
        for (int i = 0; i < stacks; ++i)
        {
            k1 = i * (sectors + 1);     // beginning of current stack
            k2 = k1 + sectors + 1;      // beginning of next stack

            for (int j = 0; j < sectors; ++j, ++k1, ++k2)
            {
                // 2 triangles per sector excluding 1st and last stacks
                if (i != 0)
                {
                    indices.insert(indices.end(), Index{ k1, k2, k1 + 1 });// k1---k2---k1+1
                }

                if (i != (stacks - 1))
                {
                    indices.insert(indices.end(), Index{ k1 + 1, k2, k2 + 1 });  // k1+1---k2---k2+1
                }
            }
        }

        return Count<Mesh>::Create("Sphere", vertices, indices);
    }

    static void CalculateRing(size_t segments, float radius, float y, float dy, float height, float actualRadius, std::vector<Vertex>& vertices)
    {
        float segIncr = 1.0f / (float)(segments - 1);
        for (size_t s = 0; s < segments; s++)
        {
            float x = glm::cos(float(Math::PIE() * 2) * s * segIncr) * radius;
            float z = glm::sin(float(Math::PIE() * 2) * s * segIncr) * radius;

            Vertex& vertex = vertices.emplace_back();
            vertex.Position = glm::vec3(actualRadius * x, actualRadius * y + height * dy, actualRadius * z);
            float texCoordX = static_cast<float>(s) / (segments - 1); // Calculate texture coordinate X
            float texCoordY = static_cast<float>(y + dy + 0.5f) / height; // Calculate texture coordinate Y

            vertex.TexCoord = glm::vec2(texCoordX, texCoordY); // Store texture coordinates

       // Calculate normals using the vertex position
            glm::vec3 normal = glm::normalize(glm::vec3(x, y, z));
            vertex.Normal= glm::vec3(normal.x, normal.y, normal.z);
        }
    }

    enum class UVProfile : int
    {
        Fixed = 0,
        Aspect = 1,
        Uniform = 2
    };
    /*
    static void CalculateRing(size_t segments, float radius, float y, float dy, float height, float actualRadius, std::vector<Vertex>& vertices)
    {
        float segIncr = 1.0f / (float)(segments - 1);
        for (size_t s = 0; s < segments; s++)
        {
            float x = glm::cos(float(Math::PIE() * 2) * s * segIncr) * radius;
            float z = glm::sin(float(Math::PIE() * 2) * s * segIncr) * radius;

            Vertex& vertex = vertices.emplace_back();
            vertex.Position = glm::vec3(actualRadius * x, actualRadius * y + height * dy, actualRadius * z);
        }
    }
    */


    Count<class Mesh> MeshWorkShop::GenerateCapsule(float radius, float height, uint32_t segments, uint32_t subdivisionsHeighfdssdfdsfsad)
    {
        constexpr size_t subdivisionsHeight = 8;
        constexpr size_t ringsBody = subdivisionsHeight + 1;
        constexpr size_t ringsTotal = subdivisionsHeight + ringsBody;
        constexpr size_t numSegments = 12;
        //constexpr float radiusModifier = 0.021f; // Needed to ensure that the wireframe is always visible
        constexpr float radiusModifier = 0.0f; // Needed to ensure that the wireframe is always visible

        std::vector<Vertex> vertices;
        std::vector<Index> indices;

        vertices.reserve(numSegments * ringsTotal);
        indices.reserve((numSegments - 1) * (ringsTotal - 1) * 2);

        float bodyIncr = 1.0f / (float)(ringsBody - 1);
        float ringIncr = 1.0f / (float)(subdivisionsHeight - 1);

        for (int r = 0; r < subdivisionsHeight / 2; r++)
            CalculateRing(numSegments, glm::sin(float(Math::PIE()) * r * ringIncr), glm::sin(float(Math::PIE()) * (r * ringIncr - 0.5f)), -0.5f, height, radius + radiusModifier, vertices);

        for (int r = 0; r < ringsBody; r++)
            CalculateRing(numSegments, 1.0f, 0.0f, r * bodyIncr - 0.5f, height, radius + radiusModifier, vertices);

        for (int r = subdivisionsHeight / 2; r < subdivisionsHeight; r++)
            CalculateRing(numSegments, glm::sin(float(Math::PIE()) * r * ringIncr), glm::sin(float(Math::PIE()) * (r * ringIncr - 0.5f)), 0.5f, height, radius + radiusModifier, vertices);

        for (int r = 0; r < ringsTotal - 1; r++)
        {
            for (int s = 0; s < numSegments - 1; s++)
            {
                Index& index1 = indices.emplace_back();
                index1.V1 = (uint32_t)(r * numSegments + s + 1);
                index1.V2 = (uint32_t)(r * numSegments + s + 0);
                index1.V3 = (uint32_t)((r + 1) * numSegments + s + 1);

                Index& index2 = indices.emplace_back();
                index2.V1 = (uint32_t)((r + 1) * numSegments + s + 0);
                index2.V2 = (uint32_t)((r + 1) * numSegments + s + 1);
                index2.V3 = (uint32_t)(r * numSegments + s);
            }
        }


        return Count<Mesh>::Create("Capsule", vertices, indices);
    }
    //https://www.songho.ca/opengl/gl_cylinder.html (//for cone and cylinder) download the zip file
    Count<class Mesh> MeshWorkShop::GenerateCone(uint32_t sectorCount, uint32_t stackCount,float radius, float height)
    {
        Count<Mesh> mesh = GenerateCylinder(sectorCount, stackCount, 0, radius, height);
        mesh->SetName("Cone");
        return mesh;
    }
    struct CylinderGeneratorSpecificVertex
    {
        glm::vec2 Position;
        glm::vec3 Normal;

    };
    ///////////////////////////////////////////////////////////////////////////////
// transform vertex/normal (x,y,z) coords
// assume from/to values are validated: 1~3 and from != to
///////////////////////////////////////////////////////////////////////////////
    // this is a very very very useful funciton
    void ChangeUpAxis(int from, int to,std::vector<Vertex>& vertices)
    {
        // initial transform matrix cols
        float tx[] = { 1.0f, 0.0f, 0.0f };    // x-axis (left)
        float ty[] = { 0.0f, 1.0f, 0.0f };    // y-axis (up)
        float tz[] = { 0.0f, 0.0f, 1.0f };    // z-axis (forward)

        // X -> Y
        if (from == 1 && to == 2)
        {
            tx[0] = 0.0f; tx[1] = 1.0f;
            ty[0] = -1.0f; ty[1] = 0.0f;
        }
        // X -> Z
        else if (from == 1 && to == 3)
        {
            tx[0] = 0.0f; tx[2] = 1.0f;
            tz[0] = -1.0f; tz[2] = 0.0f;
        }
        // Y -> X
        else if (from == 2 && to == 1)
        {
            tx[0] = 0.0f; tx[1] = -1.0f;
            ty[0] = 1.0f; ty[1] = 0.0f;
        }
        // Y -> Z
        else if (from == 2 && to == 3)
        {
            ty[1] = 0.0f; ty[2] = 1.0f;
            tz[1] = -1.0f; tz[2] = 0.0f;
        }
        //  Z -> X
        else if (from == 3 && to == 1)
        {
            tx[0] = 0.0f; tx[2] = -1.0f;
            tz[0] = 1.0f; tz[2] = 0.0f;
        }
        // Z -> Y
        else
        {
            ty[1] = 0.0f; ty[2] = -1.0f;
            tz[1] = 1.0f; tz[2] = 0.0f;
        }

        std::size_t i, j;
        std::size_t count = vertices.size();
        float vx, vy, vz;
        float nx, ny, nz;
        for (i = 0; i < count; i++)
        {
        #if 0
            // transform vertices
            vx = vertices[i].Position.x;
            vy = vertices[i].Position.y;
            vz = vertices[i].Position.z;

            vertices[i].Position.x = tx[0] * vx + ty[0] * vy + tz[0] * vz;   // x
            vertices[i].Position.y = tx[1] * vx + ty[1] * vy + tz[1] * vz;   // y
            vertices[i].Position.z = tx[2] * vx + ty[2] * vy + tz[2] * vz;   // z

            // transform normals
            nx = vertices[i].Normal.x;
            ny = vertices[i].Normal.y;
            nz = vertices[i].Normal.z;

            vertices[i].Normal.x = tx[0] * nx + ty[0] * ny + tz[0] * nz;   // nx
            vertices[i].Normal.y = tx[1] * nx + ty[1] * ny + tz[1] * nz;   // ny
            vertices[i].Normal.z = tx[2] * nx + ty[2] * ny + tz[2] * nz;   // nz
        #else
            // transform vertices
            glm::vec3 position = vertices[i].Position;
            vertices[i].Position.x = tx[0] * position.x + ty[0] * position.y + tz[0] * position.z;   // x
            vertices[i].Position.y = tx[1] * position.x + ty[1] * position.y + tz[1] * position.z;   // y
            vertices[i].Position.z = tx[2] * position.x + ty[2] * position.y + tz[2] * position.z;   // z

            // transform normals
            glm::vec3 normal = vertices[i].Normal;
            vertices[i].Normal.x = tx[0] * normal.x + ty[0] * normal.y + tz[0] * normal.z;   // nx
            vertices[i].Normal.y = tx[1] * normal.x + ty[1] * normal.y + tz[1] * normal.z;   // ny
            vertices[i].Normal.z = tx[2] * normal.x + ty[2] * normal.y + tz[2] * normal.z;   // nz

            // transform tangents
            glm::vec3 tangent = vertices[i].Tangent;
            vertices[i].Tangent.x = tx[0] * tangent.x + ty[0] * tangent.y + tz[0] * tangent.z;   // tx
            vertices[i].Tangent.y = tx[1] * tangent.x + ty[1] * tangent.y + tz[1] * tangent.z;   // ty
            vertices[i].Tangent.z = tx[2] * tangent.x + ty[2] * tangent.y + tz[2] * tangent.z;   // tz

            // transform bitangents
            glm::vec3 bitangent = vertices[i].Bitangent;
            vertices[i].Bitangent.x = tx[0] * bitangent.x + ty[0] * bitangent.y + tz[0] * bitangent.z;   // btx
            vertices[i].Bitangent.y = tx[1] * bitangent.x + ty[1] * bitangent.y + tz[1] * bitangent.z;   // bty
            vertices[i].Bitangent.z = tx[2] * bitangent.x + ty[2] * bitangent.y + tz[2] * bitangent.z;   // btz

            // Note: Texture coordinates usually don't need to be transformed in this context
            // as they are 2D and mapping on the plane doesn't change with axis swaps.
        #endif
        }
    }
    std::vector<CylinderGeneratorSpecificVertex> GetUnitCircleVertices(uint32_t sectorCount, float baseRadius, float topRadius, float height)
    {
        const float PI = 3.1415926f;
        float sectorStep = 2 * PI / sectorCount;
        float sectorAngle;  // radian

        //vertices
        float zAngle = atan2(baseRadius - topRadius, height);
        float x0 = cos(zAngle);     // nx
        float y0 = 0;               // ny
        float z0 = sin(zAngle);     // nz
        std::vector<CylinderGeneratorSpecificVertex> unitCircleVertices;
        for (int i = 0; i <= sectorCount; ++i)
        {
            sectorAngle = i * sectorStep;
            CylinderGeneratorSpecificVertex vertex;
            vertex.Position.x = cos(sectorAngle); // x
            vertex.Position.y = sin(sectorAngle); // y

            vertex.Normal.x = cos(sectorAngle) * x0 - sin(sectorAngle) * y0;
            vertex.Normal.y = sin(sectorAngle) * x0 + cos(sectorAngle) * y0;
            vertex.Normal.z = z0;
            unitCircleVertices.push_back(vertex);
        }
        return unitCircleVertices;
    }
    Count<class Mesh> MeshWorkShop::GenerateCylinder(uint32_t sectorCount, uint32_t stackCount ,float topRadius,float baseRadius, float height)
    {
           //chatgpt
        std::vector<Vertex> vertices; std::vector<Index> indices;

        float radius;
        std::vector<CylinderGeneratorSpecificVertex> unitCircleVertices = GetUnitCircleVertices(sectorCount, baseRadius, topRadius, height);
        for (uint32_t i = 0; i <= stackCount; ++i)
        {
            float z = -(height * 0.5f) + (float)i / stackCount * height;
            radius = baseRadius + (float)i / stackCount * (topRadius - baseRadius);
            float t = 1.0f - static_cast<float>(i) / stackCount; // vertical texture coordinate
            for (uint32_t j = 0; j <= sectorCount; j++)
            {

                float x = unitCircleVertices[j].Position.x;
                float y = unitCircleVertices[j].Position.y;
                Vertex vertex;
                vertex.Position.x = x * radius;
                vertex.Position.y = y * radius;
                vertex.Position.z = z;

                vertex.Normal.x = unitCircleVertices[j].Normal.x;
                vertex.Normal.y = unitCircleVertices[j].Normal.y;
                vertex.Normal.z = unitCircleVertices[j].Normal.z;

                vertex.TexCoord.x = j / sectorCount;
                vertex.TexCoord.y = t;

                vertices.push_back(vertex);
            }
        }


        uint32_t baseVertexIndex = vertices.size();

            float z = -height * 0.5f;
        {

            Vertex& vertex = vertices.emplace_back();
            vertex.Position = { 0,0,z };
            vertex.Normal = { 0,0,-1 };
            vertex.TexCoord = { 0.5f,0.5f };
        }
        for (int i = 0; i < sectorCount; ++i)
        {
            float x = unitCircleVertices[i].Position.x;
            float y = unitCircleVertices[i].Position.y;
            Vertex& vertex = vertices.emplace_back();
            vertex.Position = { x * baseRadius, y * baseRadius, z };
            vertex.Normal = { 0, 0, -1 };
            vertex.TexCoord = { -x * 0.5f + 0.5f, -y * 0.5f + 0.5f }; // flip horizontal
        }
        // put indices for base
        for (uint32_t i = 0, k = baseVertexIndex + 1; i < sectorCount; ++i, ++k)
        {
            if (i < sectorCount - 1)
                indices.insert(indices.end(), Index{ baseVertexIndex, k + 1, k });
            else
                indices.insert(indices.end(), Index{ baseVertexIndex, baseVertexIndex + 1, k });
        }
        // put vertices of top of cylinder
        unsigned int topVertexIndex = (unsigned int)vertices.size();
        z = height * 0.5f;

        {

            Vertex& vertex = vertices.emplace_back();
            vertex.Position = { 0,0,z };
            vertex.Normal = { 0,0,1 };
            vertex.TexCoord = { 0.5f,0.5f };
        }
        for (int i = 0; i < sectorCount; ++i)
        {
            float x = unitCircleVertices[i].Position.x;
            float y = unitCircleVertices[i].Position.y;
            Vertex& vertex = vertices.emplace_back();
            vertex.Position = { x * topRadius, y * topRadius, z };
            vertex.Normal = { 0, 0, 1 };
            vertex.TexCoord = { x * 0.5f + 0.5f, -y * 0.5f + 0.5f }; // flip horizontal
        }

         // put indices for sides
        uint32_t k1, k2;
        for (int i = 0; i < stackCount; ++i)
        {
            k1 = i * (sectorCount + 1);     // bebinning of current stack
            k2 = k1 + sectorCount + 1;      // beginning of next stack

            for (int j = 0; j < sectorCount; ++j, ++k1, ++k2)
            {
                // 2 trianles per sector
                indices.insert(indices.end(), Index{ k1, k1 + 1, k2 });
                indices.insert(indices.end(), Index{ k2, k1 + 1, k2 + 1 });

            }
        }

        // remember where the base indices start

        // put indices for base
        for (uint32_t i = 0, k = baseVertexIndex + 1; i < sectorCount; ++i, ++k)
        {
            if (i < (sectorCount - 1))
                indices.insert(indices.end(), Index{ baseVertexIndex, k + 1, k });
            else    // last triangle
                indices.insert(indices.end(), Index{ baseVertexIndex, baseVertexIndex + 1, k });
        }


        for (uint32_t i = 0, k = topVertexIndex + 1; i < sectorCount; ++i, ++k)
        {
            if (i < (sectorCount - 1))
                indices.insert(indices.end(), Index{ topVertexIndex, k, k + 1 });
            else
                indices.insert(indices.end(), Index{ topVertexIndex, k, topVertexIndex + 1 });
        }
        ChangeUpAxis(3, 2,vertices);
        return Count<class Mesh>::Create("Cylinder", vertices, indices);
    }

    Count<class Mesh> MeshWorkShop::GenerateTorus(uint32_t numSegments, uint32_t numRings, float majorRadius, float minorRadius)
    {
             //chatgpt
        std::vector<Vertex> vertices; std::vector<Index> indices;
        for (int ring = 0; ring <= numRings; ++ring)
        {
            float phi = static_cast<float>(ring) * (2.0f * 3.1415926f) / static_cast<float>(numRings);
            float sinPhi = sin(phi);
            float cosPhi = cos(phi);

            for (int segment = 0; segment <= numSegments; ++segment)
            {
                float theta = static_cast<float>(segment) * (2.0f * 3.1415926f) / static_cast<float>(numSegments);
                float sinTheta = sin(theta);
                float cosTheta = cos(theta);

                Vertex vertex;
                vertex.Position.x = (majorRadius + minorRadius * cosTheta) * cosPhi;
                vertex.Position.y = (majorRadius + minorRadius * cosTheta) * sinPhi;
                vertex.Position.z = minorRadius * sinTheta;

                bool smoothNormals = true;
                if (smoothNormals)
                {
                    // Calculate smooth normals
                    float nx = cosTheta * cosPhi;
                    float ny = cosTheta * sinPhi;
                    float nz = sinTheta;
                    vertex.Normal.x = nx;
                    vertex.Normal.y = ny;
                    vertex.Normal.z = nz;
                }
                else
                {
                    // Use a constant normal (flat shading)
                    vertex.Normal.x = 0.0f;
                    vertex.Normal.y = 0.0f;
                    vertex.Normal.z = 1.0f;
                }

                vertex.TexCoord.x = static_cast<float>(segment) / static_cast<float>(numSegments);
                vertex.TexCoord.y = static_cast<float>(ring) / static_cast<float>(numRings);

                // You may compute tangent and bitangent vectors later

                vertices.push_back(vertex);
            }
        }

        // Create indices for the torus
        for (int ring = 0; ring < numRings; ++ring)
        {
            for (int segment = 0; segment < numSegments; ++segment)
            {
                int nextSegment = (segment + 1) % (numSegments + 1);

                int top1 = ring * (numSegments + 1) + segment;
                int top2 = ring * (numSegments + 1) + nextSegment;
                int bottom1 = (ring + 1) * (numSegments + 1) + segment;
                int bottom2 = (ring + 1) * (numSegments + 1) + nextSegment;

                Index index1{ top1,bottom1,bottom2 };
                indices.push_back(index1);
                //indices.push_back(top1);
                //indices.push_back(bottom1);
                //indices.push_back(bottom2);

                Index index2{ top1,bottom2,top2 };
                indices.push_back(index2);
                //indices.push_back(top1);
                //indices.push_back(bottom2);
                //indices.push_back(top2);
            }
        }
        return Count<class Mesh>::Create("Torus", vertices, indices);
    }

    Count<class Mesh> MeshWorkShop::GeneratePlane(uint32_t numSegments, float planeSize, bool doubleSided)
    {
        //chatgpt
        std::vector<Vertex> vertices; std::vector<Index> indices;
        float segmentSize = planeSize / static_cast<float>(numSegments);

        for (int i = 0; i <= numSegments; ++i)
        {
            for (int j = 0; j <= numSegments; ++j)
            {
                Vertex vertex;

                // Vertex positions
                vertex.Position.x = static_cast<float>(i) * segmentSize - (planeSize / 2.0f);
                vertex.Position.y = static_cast<float>(j) * segmentSize - (planeSize / 2.0f);
                vertex.Position.z = 0;; // Adjust the Z-coordinate as needed

                // Normals (for a one-sided plane, all normals point in the same direction)
                vertex.Normal.x = 0.0f;
                vertex.Normal.y = 0.0f;// Normal points along the positive Y-axis
                vertex.Normal.z = 1.0f;

                // Texture coordinates (you can adjust these as needed)
                vertex.TexCoord.x = static_cast<float>(i) / static_cast<float>(numSegments);
                vertex.TexCoord.y = static_cast<float>(j) / static_cast<float>(numSegments);

                // You may compute tangent and bitangent vectors later

                vertices.push_back(vertex);
            }
        }

        for (int i = 0; i < numSegments; ++i)
        {
            for (int j = 0; j < numSegments; ++j)
            {
                int topLeft = (i * (numSegments + 1)) + j;
                int topRight = topLeft + 1;
                int bottomLeft = ((i + 1) * (numSegments + 1)) + j;
                int bottomRight = bottomLeft + 1;

                Index index1{ topLeft,bottomLeft,topRight };
                indices.push_back(index1);
                //indices.push_back(topLeft);
                //indices.push_back(bottomLeft);
                //indices.push_back(topRight);

                Index index2{ topRight,bottomLeft,bottomRight };
                indices.push_back(index2);
                // indices.push_back(topRight);
                // indices.push_back(bottomLeft);
                // indices.push_back(bottomRight);

                if (doubleSided)
                {
                    Index index3{ topLeft + 1,bottomLeft + 1,topRight + 1 };
                    indices.push_back(index3);

                    Index index4{ topRight + 1,bottomLeft + 1,bottomRight + 1 };
                    indices.push_back(index4);
                }

            }
        }
        ChangeUpAxis(3, 2, vertices);
        return Count<class Mesh>::Create("Plane", vertices, indices);
    }
    
}