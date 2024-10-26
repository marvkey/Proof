#include "Proofprch.h"
#include "ClipMapMeshBuilder.h"
#include "Proof/Math/Math.h"
#include "Proof/Scene/Mesh.h"
namespace Proof
{

    struct CombineInstance
    {
        Count<Mesh> Mesh;
        uint32_t SubMeshIndex;
        glm::mat4 Transform;

    };
    glm::mat4 TRS(const glm::vec3& translation, const glm::quat& rotation, const glm::vec3& scale)
    {
        // Create translation matrix
        glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), translation);

        // Create rotation matrix from quaternion
        glm::mat4 rotationMatrix = glm::mat4_cast(rotation);

        // Create scaling matrix
        glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), scale);

        // Combine translation, rotation, and scaling matrices
        return translationMatrix * rotationMatrix * scaleMatrix;
    }

    static Count<Mesh> CombineMeshes(const std::vector<CombineInstance>& combine, const std::string& newMeshName,bool mergeSubMeshes,bool useMatrices = false) 
    {
        std::vector<Vertex> combinedVertices;
        std::vector<Index> combinedIndices;
        uint32_t vertexOffset = 0;  // Offset for indices

        // Loop through each CombineInstance
        for (const auto& instance : combine) {
            const auto& sourceMesh = instance.Mesh;

            // Get the vertices and indices of the specific submesh
            std::vector<Vertex> vertices = sourceMesh->GetMeshSource()->GetVertices();
            std::vector<Index> indices = sourceMesh->GetMeshSource()->GetIndices();

            glm::mat4 transform = useMatrices ? instance.Transform : glm::mat4(1.0f);
            glm::mat4 normalTransform = glm::transpose(glm::inverse(transform));
            // Transform and combine vertices
            for (auto& vertex : vertices) {
                vertex.Position = glm::vec3(transform * glm::vec4(vertex.Position, 1.0f));

                // Transform normal, tangent, and bitangent using the inverse transpose
                vertex.Normal = glm::normalize(glm::vec3(normalTransform * glm::vec4(vertex.Normal, 0.0f)));
                vertex.Tangent = glm::normalize(glm::vec3(normalTransform * glm::vec4(vertex.Tangent, 0.0f)));
                vertex.Bitangent = glm::normalize(glm::vec3(normalTransform * glm::vec4(vertex.Bitangent, 0.0f)));

                combinedVertices.push_back(vertex);
            }

            // Combine indices, adjusting with vertex offset
            for (auto& index : indices) {
                index.V1 += vertexOffset;
                index.V2 += vertexOffset;
                index.V3 += vertexOffset;
                combinedIndices.push_back(index);
            }

            vertexOffset += vertices.size();
        }

        return Count<Mesh>::Create(newMeshName, combinedVertices, combinedIndices);
    }
    static const int Overlap = 2;

    Count<class Mesh> ClipMeshBuilder::BuildClipMap(int vertexDensity, int clipMapLevels)
    {
        int clipLevelHalfSize = ClipLevelHalfSize(vertexDensity);

        std::array<CombineInstance,2> combine;

        combine[0].Mesh = BuildPlane(2 * clipLevelHalfSize + Overlap, 2 * clipLevelHalfSize + Overlap,
            (Math::GetRightVector() + Math::GetFowardVector()) * (float)(clipLevelHalfSize + 1), true);

        
        combine[0].Transform = TRS(glm::vec3(), glm::quat(), glm::vec3(1));

        Count<Mesh> ring = BuildRing(clipLevelHalfSize);

        for (int i = 1; i < clipMapLevels + 1; i++)
        {               
            combine[i].Mesh = ring;
            combine[i].Transform = TRS(glm::vec3(0), glm::quat(), glm::vec3(1) * (float)glm::pow(2, i));
        }

        combine[clipMapLevels + 1].Mesh = BuildSkirt(clipLevelHalfSize, 10);
        combine[clipMapLevels + 1].Transform = TRS(
            glm::vec3(0), glm::quat(), glm::vec3(1) * (float)glm::pow(2, clipMapLevels));

        return CombineMeshes(std::vector(combine.begin(), combine.end()), "ClipMapSkirt", true);
    }


    Count<Mesh> ClipMeshBuilder::BuildRing(int clipLevelHalfSize)
    {
        int k = clipLevelHalfSize;

        int shortSide = (k + 1) / 2 + Overlap;
        int longSide = k - 1;
        int sum = longSide + shortSide;

        bool shortMorphShift = (shortSide / 2) % 2 == 1;

        std::array<CombineInstance,8> combine;

        glm::vec3 pivot = (Math::GetRightVector() + Math::GetFowardVector()) * (float)(k + 1);

        // bottom left
        combine[0].Mesh = BuildPlane(shortSide, shortSide, pivot, true, false, false);
        combine[0].Transform = TRS(
            glm::vec3(0), glm::quat(), glm::vec3(1));

        // middle left
        combine[1].Mesh = BuildPlane(shortSide, longSide, pivot, true, false, shortMorphShift);
        combine[1].Transform = TRS(
            Math::GetFowardVector() * (float)shortSide, glm::quat(), glm::vec3(1));

        // top left
        combine[2].Mesh = BuildPlane(shortSide, shortSide, pivot, true, false, !shortMorphShift);
        combine[2].Transform = TRS(
            Math::GetFowardVector() * (float) sum, glm::quat(), glm::vec3(1));

        // top middle
        combine[3].Mesh = BuildPlane(longSide, shortSide, pivot, true, shortMorphShift, !shortMorphShift);
        combine[3].Transform = TRS(
            Math::GetFowardVector() * (float)sum + Math::GetRightVector() * (float)shortSide, glm::quat(), glm::vec3(1));

        // top right
        combine[4].Mesh = BuildPlane(shortSide, shortSide, pivot, true, !shortMorphShift, !shortMorphShift);
        combine[4].Transform = TRS(
            Math::GetFowardVector() * (float)sum + Math::GetRightVector() * (float)sum, glm::quat(), glm::vec3(1));

        // middle right
        combine[5].Mesh = BuildPlane(shortSide, longSide, pivot, true, !shortMorphShift, shortMorphShift);
        combine[5].Transform = TRS(
            Math::GetFowardVector() * (float)shortSide + Math::GetRightVector() * (float)sum, glm::quat(), glm::vec3(1));

        // bottom right
        combine[6].Mesh = BuildPlane(shortSide, shortSide, pivot, true, !shortMorphShift, false);
        combine[6].Transform = TRS(
            Math::GetRightVector() * (float)sum, glm::quat(), glm::vec3(1));

        // bottom middle
        combine[7].Mesh = BuildPlane(longSide, shortSide, pivot, true, shortMorphShift, false);
        combine[7].Transform = TRS(
            Math::GetRightVector() * (float)shortSide, glm::quat(), glm::vec3(1));

        return CombineMeshes(std::vector(combine.begin(),combine.end()), "ClipMapRing", true);
    }

    Count<Mesh> ClipMeshBuilder::BuildSkirt(int clipLevelHalfSize, float outerBorderScale)
    {
        std::array<CombineInstance, 8> combine;

        int borderVertCount = clipLevelHalfSize + Overlap;
        int scale = 2;

        glm::vec3 pivot = glm::vec3(-1.f, 0, -1.f) * (float)borderVertCount * (1 + 2 * outerBorderScale) + glm::vec3(1, 0, 1);

        Count<Mesh> quad = BuildPlane(1, 1, glm::vec3(0), false);
        Count<Mesh> hStrip = BuildPlane(borderVertCount, 1, glm::vec3(0), false);
        Count<Mesh> vStrip = BuildPlane(1, borderVertCount, glm::vec3(0), false);

        outerBorderScale *= borderVertCount * scale;
        glm::vec3 cornerQuadScale = glm::vec3(outerBorderScale, 1, outerBorderScale);
        glm::vec3 stripScaleVert = glm::vec3(scale, 1, outerBorderScale);
        glm::vec3 stripScaleHor = glm::vec3(outerBorderScale, 1, scale);

        combine[0].Mesh = quad;
        combine[0].Transform = TRS(pivot + glm::vec3(0), glm::quat(), cornerQuadScale);

        combine[1].Mesh = hStrip;
        combine[1].Transform = TRS(pivot + Math::GetRightVector() * outerBorderScale, glm::quat(), stripScaleVert);

        combine[2].Mesh = quad;
        combine[2].Transform = TRS(pivot + Math::GetRightVector() * (outerBorderScale + borderVertCount * scale), glm::quat(), cornerQuadScale);

        combine[3].Mesh = vStrip;
        combine[3].Transform = TRS(pivot + Math::GetFowardVector() * outerBorderScale, glm::quat(), stripScaleHor);

        combine[4].Mesh = vStrip;
        combine[4].Transform = TRS(pivot + Math::GetRightVector() * (outerBorderScale + borderVertCount * scale)
            + Math::GetFowardVector() * outerBorderScale, glm::quat(), stripScaleHor);

        combine[5].Mesh = quad;
        combine[5].Transform = TRS(pivot + Math::GetFowardVector() * (outerBorderScale + borderVertCount * scale), glm::quat(), cornerQuadScale);

        combine[6].Mesh = hStrip;
        combine[6].Transform = TRS(pivot + Math::GetRightVector() * outerBorderScale
            + Math::GetFowardVector() * (outerBorderScale + borderVertCount * scale), glm::quat(), stripScaleVert);

        combine[7].Mesh = quad;
        combine[7].Transform = TRS(pivot + Math::GetRightVector() * (outerBorderScale + borderVertCount * scale)
            + Math::GetFowardVector() * (outerBorderScale + borderVertCount * scale), glm::quat(), cornerQuadScale);

        return CombineMeshes(std::vector(combine.begin(), combine.end()), "ClipMapSkirt", true);

    }

    Count<class Mesh> ClipMeshBuilder::BuildPlane(int width, int height, glm::vec3 pivot, bool geomorphOffsetInUv, bool morphShiftX, bool morphShiftZ, int trianglesShift)
    {
        //if ((width + 1) * (height + 1) >= 256 * 256)
        //    mesh.indexFormat = UnityEngine.Rendering.IndexFormat.UInt32;


       // std::vector<glm::vec3> vertices((width + 1) * (height + 1));
       // std::vector<glm::vec2> uvs((width + 1) * (height + 1));
       // std::vector<int> triangles(width * height * 2 * 3);
       // std::vector<glm::vec3> normals((width + 1) * (height + 1));

        std::vector<Vertex> vertex((width + 1) * (height + 1));
        std::vector<int> triangles(width * height * 2 * 3);


        for (int i = 0; i < height + 1; i++)
        {
            for (int j = 0; j < width + 1; j++)
            {
                int x = j;
                int z = i;

                glm::vec3 normalPosition = glm::vec3(x, 1, z);

                if (x % 2 != 0)
                    x += morphShiftX ^ x % 4 == 3 ? 1 : -1;

                if (z % 2 != 0)
                    z += morphShiftZ ^ z % 4 == 3 ? 1 : -1;

                vertex[j + i * (width + 1)].Position = normalPosition - pivot;

                if (geomorphOffsetInUv)
                    vertex[j + i * (width + 1)].TexCoord = glm::vec2(x - normalPosition.x, z - normalPosition.z);

                vertex[j + i * (width + 1)].Normal = Math::GetUpVector();
            }
        }

        int tris = 0;
        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j++)
            {
                int k = j + i * (width + 1);
                if ((i + j + trianglesShift) % 2 == 0)
                {
                    triangles[tris++] = k;
                    triangles[tris++] = k + width + 1;
                    triangles[tris++] = k + width + 2;

                    triangles[tris++] = k;
                    triangles[tris++] = k + width + 2;
                    triangles[tris++] = k + 1;
                }
                else
                {
                    triangles[tris++] = k;
                    triangles[tris++] = k + width + 1;
                    triangles[tris++] = k + 1;

                    triangles[tris++] = k + 1;
                    triangles[tris++] = k + width + 1;
                    triangles[tris++] = k + width + 2;
                }
            }
        }
        std::vector<Index> trianglesIndex(width * height * 2);

        int indexCount = 0;
        for (size_t i = 0; i < triangles.size(); i += 3)
        {
            trianglesIndex[indexCount++] = { (uint32_t)triangles[i], (uint32_t)triangles[i + 1], (uint32_t)triangles[i + 2] };
        }

        return Count<Mesh>::Create("ClipMap plane", vertex, trianglesIndex);
    }
}
