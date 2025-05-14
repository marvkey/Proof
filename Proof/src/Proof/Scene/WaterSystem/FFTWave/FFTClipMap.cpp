#include "Proofprch.h"
#include "FFTClipMap.h"
#include "Proof/Scene/SceneUtils.h"

namespace Proof
{
    static inline const int Overlap = 2;

    Count<Mesh> FFTClipMap::BuildClipMapPlane(int vertexDensity, int clipMapLevels)
    {
        int clipLevelHalfSize = ClipLevelHalfSize(vertexDensity);

        std::vector<Count<Mesh>> combineMeshes(clipMapLevels + 2);

        combineMeshes[0] = BuildPlane(2 * clipLevelHalfSize + Overlap, 2 * clipLevelHalfSize + Overlap,
            (Math::GetRightVector() + -Math::GetForwardVector()) * (float)(clipLevelHalfSize + 1), true);

        combineMeshes[0]->SetTransform(Utils::MatrixTRS(glm::vec3(0), Math::GetQuaternionIdentity(), glm::vec3(1)));

       Count<Mesh> ring = BuildRing(clipLevelHalfSize);

       for (int i = 1; i < clipMapLevels + 1; i++)
       {
           combineMeshes[i] = ring;
           combineMeshes[i]->SetTransform(Utils::MatrixTRS(glm::vec3(0), Math::GetQuaternionIdentity(), glm::vec3(1) * (float)glm::pow(2.0f, i)));
       }

       combineMeshes[clipMapLevels + 1] = BuildSkirt(clipLevelHalfSize, 10);
       combineMeshes[clipMapLevels + 1]->SetTransform(Utils::MatrixTRS(
           glm::vec3(0), Math::GetQuaternionIdentity(), glm::vec3(1) * (float)glm::pow(2, clipMapLevels)));

        return Mesh::CombineMeshes("GeoClipmap", combineMeshes);

    }
    Count<Mesh> FFTClipMap::BuildRing(int clipLevelHalfSize)
    {

        int k = clipLevelHalfSize;

        int shortSide = (k + 1) / 2 + 2; // Overlap = 2
        int longSide = k - 1;
        int sum = longSide + shortSide;

        bool shortMorphShift = (shortSide / 2) % 2 == 1;

        glm::vec3 pivot = (glm::vec3(1, 0, 0) + glm::vec3(0, 0, 1)) * float(k + 1);

        std::vector<Count<Mesh>> combineMeshes(8);

        // bottom left
        combineMeshes[0] = BuildPlane(shortSide, shortSide, pivot, true, false, false);
        combineMeshes[0]->SetTransform(Utils::MatrixTRS(glm::vec3(0.0f), Math::GetQuaternionIdentity(), glm::vec3(1.0f)));

        // middle left
        combineMeshes[1] = BuildPlane(shortSide, longSide, pivot, true, false, shortMorphShift);
        combineMeshes[1]->SetTransform(Utils::MatrixTRS(-Math::GetForwardVector() * float(shortSide), Math::GetQuaternionIdentity(), glm::vec3(1.0f)));

        // top left
        combineMeshes[2] = BuildPlane(shortSide, shortSide, pivot, true, false, !shortMorphShift);
        combineMeshes[2]->SetTransform(Utils::MatrixTRS(-Math::GetForwardVector() * float(sum), Math::GetQuaternionIdentity(), glm::vec3(1.0f)));

        // top middle
        combineMeshes[3] = BuildPlane(longSide, shortSide, pivot, true, shortMorphShift, !shortMorphShift);
        combineMeshes[3]->SetTransform(Utils::MatrixTRS(-Math::GetForwardVector() * float(sum) + Math::GetRightVector() * float(shortSide), Math::GetQuaternionIdentity(), glm::vec3(1.0f)));

        // top right
        combineMeshes[4] = BuildPlane(shortSide, shortSide, pivot, true, !shortMorphShift, !shortMorphShift);
        combineMeshes[4]->SetTransform(Utils::MatrixTRS(-Math::GetForwardVector() * float(sum) + Math::GetRightVector() * float(sum), Math::GetQuaternionIdentity(), glm::vec3(1.0f)));

        // middle right
        combineMeshes[5] = BuildPlane(shortSide, longSide, pivot, true, !shortMorphShift, shortMorphShift);
        combineMeshes[5]->SetTransform(Utils::MatrixTRS(-Math::GetForwardVector() * float(shortSide) + Math::GetRightVector() * float(sum), Math::GetQuaternionIdentity(), glm::vec3(1.0f)));

        // bottom right
        combineMeshes[6] = BuildPlane(shortSide, shortSide, pivot, true, !shortMorphShift, false);
        combineMeshes[6]->SetTransform(Utils::MatrixTRS(Math::GetRightVector() * float(sum), Math::GetQuaternionIdentity(), glm::vec3(1.0f)));

        // bottom middle
        combineMeshes[7] = BuildPlane(longSide, shortSide, pivot, true, shortMorphShift, false);
        combineMeshes[7]->SetTransform(Utils::MatrixTRS(Math::GetRightVector() * float(shortSide), Math::GetQuaternionIdentity(), glm::vec3(1.0f)));

        return Mesh::CombineMeshes("Clipmap Ring", combineMeshes);
    }

    Count<Mesh> FFTClipMap::BuildSkirt(int clipLevelHalfSize, float outerBorderScale)
    {
        const int overlap = 2;
        int borderVertCount = clipLevelHalfSize + overlap;
        int scale = 2;

        glm::vec3 pivot = glm::vec3(-1.f, 0, -1.f) * float(borderVertCount) * (1.0f + 2.0f * outerBorderScale) + glm::vec3(1, 0, 1);

        Count<Mesh> quad = BuildPlane(1, 1, glm::vec3(0.0f), false);
        Count<Mesh> hStrip = BuildPlane(borderVertCount, 1, glm::vec3(0.0f), false);
        Count<Mesh> vStrip = BuildPlane(1, borderVertCount, glm::vec3(0.0f), false);

        outerBorderScale *= float(borderVertCount) * scale;
        glm::vec3 cornerQuadScale = glm::vec3(outerBorderScale, 1.0f, outerBorderScale);
        glm::vec3 stripScaleVert = glm::vec3(float(scale), 1.0f, outerBorderScale);
        glm::vec3 stripScaleHor = glm::vec3(outerBorderScale, 1.0f, float(scale));


        std::vector<Count<Mesh>> combineMeshes;
        combineMeshes.resize(8);

        combineMeshes[0] = quad;
        combineMeshes[0]->SetTransform(Utils::MatrixTRS(pivot + glm::vec3(0), Math::GetQuaternionIdentity(), cornerQuadScale));

        combineMeshes[1] = hStrip;
        combineMeshes[1]->SetTransform(Utils::MatrixTRS(pivot + Math::GetRightVector() * outerBorderScale, Math::GetQuaternionIdentity(), stripScaleVert));

        combineMeshes[2] = quad;
        combineMeshes[2]->SetTransform(Utils::MatrixTRS(pivot + Math::GetRightVector() * (outerBorderScale + borderVertCount * scale), Math::GetQuaternionIdentity(), cornerQuadScale));

        combineMeshes[3] = vStrip;
        combineMeshes[3]->SetTransform(Utils::MatrixTRS(pivot + -Math::GetForwardVector() * outerBorderScale, Math::GetQuaternionIdentity(), stripScaleHor));

        combineMeshes[4] = vStrip;
        combineMeshes[4]->SetTransform(Utils::MatrixTRS(pivot + Math::GetRightVector() * (outerBorderScale + borderVertCount * scale) + -Math::GetForwardVector() * outerBorderScale, Math::GetQuaternionIdentity(), stripScaleHor));

        combineMeshes[5] = quad;
        combineMeshes[5]->SetTransform(Utils::MatrixTRS(pivot + -Math::GetForwardVector() * (outerBorderScale + borderVertCount * scale), Math::GetQuaternionIdentity(), cornerQuadScale));

        combineMeshes[6] = hStrip;
        combineMeshes[6]->SetTransform(Utils::MatrixTRS(pivot + Math::GetRightVector() * outerBorderScale + -Math::GetForwardVector() * (outerBorderScale + borderVertCount * scale), Math::GetQuaternionIdentity(), stripScaleVert));

        combineMeshes[7] = quad;
        combineMeshes[7]->SetTransform(Utils::MatrixTRS(pivot + Math::GetRightVector() * (outerBorderScale + borderVertCount * scale) + -Math::GetForwardVector() * (outerBorderScale + borderVertCount * scale), Math::GetQuaternionIdentity(), cornerQuadScale));


        return Mesh::CombineMeshes("Clipmap_Skirt", combineMeshes);

    }
    Count<Mesh> FFTClipMap::BuildPlane(int width, int height, glm::vec3 pivot, bool geomorphOffsetInUv, bool morphShiftX, bool morphShiftZ, int trianglesShift)
    {
        std::vector<Vertex> vertices;
        std::vector<Index> indices;

        int vertexWidth = width + 1;
        int vertexHeight = height + 1;

        vertices.reserve(vertexWidth * vertexHeight);

        for (int z = 0; z < vertexHeight; z++)
        {
            for (int x = 0; x < vertexWidth; x++)
            {
                int modifiedX = x;
                int modifiedZ = z;

                glm::vec3 normalPosition = glm::vec3((float)x, 1.0f, (float)z);

                if (x % 2 != 0)
                    modifiedX += (morphShiftX ^ (x % 4 == 3)) ? 1 : -1;

                if (z % 2 != 0)
                    modifiedZ += (morphShiftZ ^ (z % 4 == 3)) ? 1 : -1;

                glm::vec3 position = normalPosition - pivot;
                glm::vec2 texCoord = geomorphOffsetInUv ? glm::vec2(modifiedX - normalPosition.x, modifiedZ - normalPosition.z) : glm::vec2(0.0f);

                Vertex v;
                v.Position = position;
                v.Normal = glm::vec3(0.0f, 1.0f, 0.0f);
                v.TexCoord = texCoord;
                v.Tangent = glm::vec3(1.0f, 0.0f, 0.0f); // Default values
                v.Bitangent = glm::vec3(0.0f, 0.0f, 1.0f);

                vertices.push_back(v);
            }
        }

        //indices.reserve(height * width * 2);
        for (uint32_t i = 0; i < height; i++)
        {
            for (uint32_t j = 0; j < width; j++)
            {
                uint32_t k = j + i * vertexWidth;

                if ((i + j + trianglesShift) % 2 == 0)
                {
                    indices.push_back(Index{ k, k + vertexWidth, k + vertexWidth + 1 });
                    indices.push_back(Index{ k, k + vertexWidth + 1, k + 1 });
                }
                else
                {
                    indices.push_back(Index{ k, k + vertexWidth, k + 1 });
                    indices.push_back(Index{ k + 1, k + vertexWidth, k + vertexWidth + 1 });
                }
            }
        }

        return Count<Mesh>::Create("ClipMapPlane", vertices, indices);
    }
}