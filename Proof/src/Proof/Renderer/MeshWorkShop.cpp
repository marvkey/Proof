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
    Count<Mesh> MeshWorkShop::GenerateCube()
    {

        //https://github.com/kidrigger/Blaze/blob/7e76de71e2e22f3b5e8c4c2c50c58e6d205646c6/Blaze/Primitives.cpp
        std::vector<Vertex> vertices = {
            {{1.0f, 1.0f, -1.0f}, {1.0f, 1.0f, 0.2f}, {1.0f, 1.0f}},
            {{1.0f, -1.0f, -1.0f}, {1.0f, 0.2f, 0.2f}, {1.0f, 0.0f}},
            {{-1.0f, -1.0f, -1.0f}, {0.2f, 0.2f, 0.2f}, {0.0f, 0.0f}},
            {{-1.0f, 1.0f, -1.0f}, {0.2f, 1.0f, 0.2f}, {0.0f, 1.0f}},
            {{-1.0f, -1.0f, 1.0f}, {0.2f, 0.2f, 1.0f}, {0.0f, 0.0f}},
            {{1.0f, -1.0f, 1.0f}, {1.0f, 0.2f, 1.0f}, {1.0f, 0.0f}},
            {{1.0f, 1.0f, 1.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}},
            {{-1.0f, 1.0f, 1.0f}, {0.2f, 1.0f, 1.0f}, {0.0f, 1.0f}},
        };

        std::vector<uint32_t> indices = {
        0, 1, 2, 0, 2, 3, 4, 5, 6, 4, 6, 7, 1, 0, 6, 1, 6, 5, 7, 6, 0, 7, 0, 3, 7, 3, 2, 7, 2, 4, 4, 2, 1, 4, 1, 5,
        };
        return Count<Mesh>::Create("Cube", vertices, indices);

    }
  
    Count<Mesh> MeshWorkShop::GenerateSphere(float radius, float sectors, float stacks)
    {
        constexpr int MIN_SECTOR_COUNT = 3;
        constexpr int MIN_STACK_COUNT = 2;
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
                vertex.Vertices = { x,y,z };
                // normalized vertex normal
                nx = x * lengthInv;
                ny = y * lengthInv;
                nz = z * lengthInv;
                vertex.Normal = { nx, ny, nz };
                // vertex tex coord between [0, 1]
                s = (float)j / sectors;
                t = (float)i / stacks;
                vertex.TexCoords = { s,t };
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
                    indices.insert(indices.end(), { k1, k2, k1 + 1 });// k1---k2---k1+1
                }

                if (i != (stacks - 1))
                {
                    indices.insert(indices.end(), { k1 + 1, k2, k2 + 1 });  // k1+1---k2---k2+1
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
            vertex.Vertices = Vector(actualRadius * x, actualRadius * y + height * dy, actualRadius * z);
            float texCoordX = static_cast<float>(s) / (segments - 1); // Calculate texture coordinate X
            float texCoordY = static_cast<float>(y + dy + 0.5f) / height; // Calculate texture coordinate Y

            vertex.TexCoords = Vector2(texCoordX, texCoordY); // Store texture coordinates

       // Calculate normals using the vertex position
            glm::vec3 normal = glm::normalize(glm::vec3(x, y, z));
            vertex.Normal= Vector(normal.x, normal.y, normal.z);
        }
    }
    Count<class Mesh> MeshWorkShop::GenerateCapsule(float radius, float height, uint32_t segments, uint32_t subdivisionsHeight)
    {
        const uint32_t ringsBody = subdivisionsHeight + 1;
        const uint32_t ringsTotal = subdivisionsHeight + ringsBody;
        const size_t numSegments = segments;
        constexpr float radiusModifier = 0.021f; // Needed to ensure that the wireframe is always visible

        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;

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

        for (uint32_t r = 0; r < ringsTotal - 1; r++)
        {
            for (uint32_t s = 0; s < numSegments - 1; s++)
            {
                indices.insert(indices.end(), { (uint32_t)(r * numSegments + s + 1) ,(uint32_t)(r * numSegments + s + 0),(uint32_t)((r + 1) * numSegments + s + 1) });
                indices.insert(indices.end(), { (uint32_t)((r + 1) * numSegments + s + 0) ,(uint32_t)((r + 1) * numSegments + s + 1),(uint32_t)(r * numSegments + s) });
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
            // transform vertices
            vx = vertices[i].Vertices.X;
            vy = vertices[i].Vertices.Y;
            vz = vertices[i].Vertices.Z;

            vertices[i].Vertices.X = tx[0] * vx + ty[0] * vy + tz[0] * vz;   // x
            vertices[i].Vertices.Y = tx[1] * vx + ty[1] * vy + tz[1] * vz;   // y
            vertices[i].Vertices.Z = tx[2] * vx + ty[2] * vy + tz[2] * vz;   // z

            // transform normals
            nx = vertices[i].Normal.X;
            ny = vertices[i].Normal.Y;
            nz = vertices[i].Normal.Z;

            vertices[i].Normal.X = tx[0] * nx + ty[0] * ny + tz[0] * nz;   // nx
            vertices[i].Normal.Y = tx[1] * nx + ty[1] * ny + tz[1] * nz;   // ny
            vertices[i].Normal.Z = tx[2] * nx + ty[2] * ny + tz[2] * nz;   // nz
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
        std::vector<Vertex> vertices; std::vector<uint32_t> indices;

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
                vertex.Vertices.X = x * radius;
                vertex.Vertices.Y = y * radius;
                vertex.Vertices.Z = z;

                vertex.Normal.X = unitCircleVertices[j].Normal.x;
                vertex.Normal.Y = unitCircleVertices[j].Normal.y;
                vertex.Normal.Z = unitCircleVertices[j].Normal.z;

                vertex.TexCoords.X = j / sectorCount;
                vertex.TexCoords.Y = t;

                vertices.push_back(vertex);
            }
        }


        uint32_t baseVertexIndex = vertices.size();

            float z = -height * 0.5f;
        {

            Vertex& vertex = vertices.emplace_back();
            vertex.Vertices = { 0,0,z };
            vertex.Normal = { 0,0,-1 };
            vertex.TexCoords = { 0.5f,0.5f };
        }
        for (int i = 0; i < sectorCount; ++i)
        {
            float x = unitCircleVertices[i].Position.x;
            float y = unitCircleVertices[i].Position.y;
            Vertex& vertex = vertices.emplace_back();
            vertex.Vertices = { x * baseRadius, y * baseRadius, z };
            vertex.Normal = { 0, 0, -1 };
            vertex.TexCoords = { -x * 0.5f + 0.5f, -y * 0.5f + 0.5f }; // flip horizontal
        }
        // put indices for base
        for (uint32_t i = 0, k = baseVertexIndex + 1; i < sectorCount; ++i, ++k)
        {
            if (i < sectorCount - 1)
                indices.insert(indices.end(), { baseVertexIndex, k + 1, k });
            else
                indices.insert(indices.end(), { baseVertexIndex, baseVertexIndex + 1, k });
        }
        // put vertices of top of cylinder
        unsigned int topVertexIndex = (unsigned int)vertices.size();
        z = height * 0.5f;

        {

            Vertex& vertex = vertices.emplace_back();
            vertex.Vertices = { 0,0,z };
            vertex.Normal = { 0,0,1 };
            vertex.TexCoords = { 0.5f,0.5f };
        }
        for (int i = 0; i < sectorCount; ++i)
        {
            float x = unitCircleVertices[i].Position.x;
            float y = unitCircleVertices[i].Position.y;
            Vertex& vertex = vertices.emplace_back();
            vertex.Vertices = { x * topRadius, y * topRadius, z };
            vertex.Normal = { 0, 0, 1 };
            vertex.TexCoords = { x * 0.5f + 0.5f, -y * 0.5f + 0.5f }; // flip horizontal
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
                indices.insert(indices.end(), { k1, k1 + 1, k2 });
                indices.insert(indices.end(), { k2, k1 + 1, k2 + 1 });

            }
        }

        // remember where the base indices start

        // put indices for base
        for (uint32_t i = 0, k = baseVertexIndex + 1; i < sectorCount; ++i, ++k)
        {
            if (i < (sectorCount - 1))
                indices.insert(indices.end(), { baseVertexIndex, k + 1, k });
            else    // last triangle
                indices.insert(indices.end(), { baseVertexIndex, baseVertexIndex + 1, k });
        }


        for (uint32_t i = 0, k = topVertexIndex + 1; i < sectorCount; ++i, ++k)
        {
            if (i < (sectorCount - 1))
                indices.insert(indices.end(), { topVertexIndex, k, k + 1 });
            else
                indices.insert(indices.end(), { topVertexIndex, k, topVertexIndex + 1 });
        }
        ChangeUpAxis(3, 2,vertices);
        return Count<class Mesh>::Create("Cylinder", vertices, indices);
    }

    Count<class Mesh> MeshWorkShop::GenerateTorus(uint32_t numSegments, uint32_t numRings, float majorRadius, float minorRadius)
    {
             //chatgpt
        std::vector<Vertex> vertices; std::vector<uint32_t> indices;
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
                vertex.Vertices.X = (majorRadius + minorRadius * cosTheta) * cosPhi;
                vertex.Vertices.Y = (majorRadius + minorRadius * cosTheta) * sinPhi;
                vertex.Vertices.Z = minorRadius * sinTheta;

                bool smoothNormals = true;
                if (smoothNormals)
                {
                    // Calculate smooth normals
                    float nx = cosTheta * cosPhi;
                    float ny = cosTheta * sinPhi;
                    float nz = sinTheta;
                    vertex.Normal.X = nx;
                    vertex.Normal.Y = ny;
                    vertex.Normal.Z = nz;
                }
                else
                {
                    // Use a constant normal (flat shading)
                    vertex.Normal.X = 0.0f;
                    vertex.Normal.Y = 0.0f;
                    vertex.Normal.Z = 1.0f;
                }

                vertex.TexCoords.X = static_cast<float>(segment) / static_cast<float>(numSegments);
                vertex.TexCoords.Y = static_cast<float>(ring) / static_cast<float>(numRings);

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

                indices.push_back(top1);
                indices.push_back(bottom1);
                indices.push_back(bottom2);

                indices.push_back(top1);
                indices.push_back(bottom2);
                indices.push_back(top2);
            }
        }
        return Count<class Mesh>::Create("Torus", vertices, indices);
    }

    Count<class Mesh> MeshWorkShop::GeneratePlane(uint32_t numSegments, float planeSize)
    {
          //chatgpt
        std::vector<Vertex> vertices; std::vector<uint32_t> indices;
        float segmentSize = planeSize / static_cast<float>(numSegments);

        for (int i = 0; i <= numSegments; ++i)
        {
            for (int j = 0; j <= numSegments; ++j)
            {
                Vertex vertex;

                // Vertex positions
                vertex.Vertices.X = static_cast<float>(i) * segmentSize - (planeSize / 2.0f);
                vertex.Vertices.Y = static_cast<float>(j) * segmentSize - (planeSize / 2.0f);
                vertex.Vertices.Z = 0;; // Adjust the Z-coordinate as needed

                // Normals (for a one-sided plane, all normals point in the same direction)
                vertex.Normal.X = 0.0f;
                vertex.Normal.Y = 0.0f;// Normal points along the positive Y-axis
                vertex.Normal.Z = 1.0f;

                // Texture coordinates (you can adjust these as needed)
                vertex.TexCoords.X = static_cast<float>(i) / static_cast<float>(numSegments);
                vertex.TexCoords.Y = static_cast<float>(j) / static_cast<float>(numSegments);

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

                indices.push_back(topLeft);
                indices.push_back(bottomLeft);
                indices.push_back(topRight);

                indices.push_back(topRight);
                indices.push_back(bottomLeft);
                indices.push_back(bottomRight);
            }
        }
        ChangeUpAxis(3, 2, vertices);
        return Count<class Mesh>::Create("Plane", vertices, indices);
    }
    
}