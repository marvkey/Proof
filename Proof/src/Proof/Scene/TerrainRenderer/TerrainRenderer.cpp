#include "Proofprch.h"
#include "TerrainRenderer.h"
#include "Proof/Renderer/Image.h"
#include "Proof/Renderer/Vertex.h"
#include "Proof/Scene/Mesh.h"
#include "Proof/Scene/Material.h"
#include "Proof/Asset/AssetManager.h"
#include "Proof/Renderer/Texture.h"
#include "Proof/Renderer/Colors.h"

#include <FastNoise/FastNoiseLite.h>
namespace Proof
{

	std::vector<float> GenerateNoiseMap(int mapWidth, int mapHeight, int seed, const TerrainRenderer::NoiseSettings& settings)
	{
		std::vector<float> noiseMap(mapWidth * mapHeight);

		FastNoiseLite noise;
		noise.SetSeed(seed);
		noise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
		noise.SetFractalType(FastNoiseLite::FractalType_FBm);
		noise.SetFrequency(1.0f / settings.Scale);
		noise.SetFractalOctaves(settings.Octaves);
		noise.SetFractalLacunarity(settings.Lacunarity);
		noise.SetFractalGain((float)settings.Persistence);

		float minVal = std::numeric_limits<float>::max();
		float maxVal = std::numeric_limits<float>::lowest();

		for (int y = 0; y < mapHeight; ++y) {
			for (int x = 0; x < mapWidth; ++x) {
				float sampleX = x + settings.Offset.x;
				float sampleY = y + settings.Offset.y;

				float val = noise.GetNoise(sampleX, sampleY);
				int index = y * mapWidth + x;
				noiseMap[index] = val;

				minVal = std::min(minVal, val);
				maxVal = std::max(maxVal, val);
			}
		}

		for (int i = 0; i < mapWidth * mapHeight; ++i) {
			noiseMap[i] = (noiseMap[i] - minVal) / (maxVal - minVal);
		}

		return noiseMap;
	}
	TerrainRenderer::TerrainRenderer()
	{
		RegenerateTerrainMesh();
	}

	class TerrainMeshBuilderData
	{

	public:
		TerrainMeshBuilderData(uint32_t width, uint32_t height)
		{
			Vertices.resize(width * height);
			Indices.resize(((width - 1) * (height - 1) * 2)); // 2 triangles per quad
		}

		void AddTriangle(uint32_t a, uint32_t b, uint32_t c) {
			Indices[m_TriangleIndex] = Index{ a, b, c };
			m_TriangleIndex++;
		}

		Count<Mesh> GenerateMesh()
		{
			RecalculateNormals(Vertices, Indices);
			return Count<Mesh>::Create("Terrain Mesh", Vertices, Indices);
		}

		void RecalculateNormals(std::vector<Vertex>& vertices, const std::vector<Index>& triangles) {
			// Clear existing normals
			for (auto& vertex : vertices) {
				vertex.Normal = glm::vec3(0.0f);
			}

			// Accumulate face normals
			for (const auto& tri : triangles) {
				const glm::vec3& v0 = vertices[tri.V1].Position;
				const glm::vec3& v1 = vertices[tri.V2].Position;
				const glm::vec3& v2 = vertices[tri.V3].Position;

				glm::vec3 edge1 = v1 - v0;
				glm::vec3 edge2 = v2 - v0;
				glm::vec3 faceNormal = glm::normalize(glm::cross(edge1, edge2));

				vertices[tri.V1].Normal += faceNormal;
				vertices[tri.V2].Normal += faceNormal;
				vertices[tri.V3].Normal += faceNormal;
			}

			// Normalize all normals
			for (auto& vertex : vertices) {
				vertex.Normal = glm::normalize(vertex.Normal);
			}
		}
		std::vector<Vertex> Vertices;
		std::vector<Index> Indices;
	private:

		uint32_t m_TriangleIndex = 0;
	};
	void TerrainRenderer::RegenerateTerrainMesh()
	{
		auto heightmap = GenerateNoiseMap(MapChunkSize, MapChunkSize, Seed, NoiseParams);
		GenerateMesh(heightmap, MapChunkSize, MapChunkSize);
	}

	struct TerrainType
	{
		float Height;
		glm::vec4 colour;
	};

	std::vector<TerrainType> regions = {
		{ 0.08f, glm::vec4(0.0f, 0.0f, 0.6f, 1.0f) }, // Deep Water
		{ 0.15f, glm::vec4(0.2f, 0.4f, 0.8f, 1.0f) }, // Shallow Water
		{ 0.20f, glm::vec4(0.85f, 0.8f, 0.5f, 1.0f) }, // Sand
		{ 0.5f,  glm::vec4(0.3f, 0.6f, 0.2f, 1.0f) }, // Grass
		{ 0.6f,  glm::vec4(0.2f, 0.5f, 0.2f, 1.0f) }, // Darker Grass
		{ 0.75f, glm::vec4(0.3f, 0.2f, 0.2f, 1.0f) }, // Rock
		{ 0.9f,  glm::vec4(0.2f, 0.15f, 0.15f, 1.0f) }, // Dark Rock
		{ 1.0f,  glm::vec4(1.0f, 1.0f, 1.0f, 1.0f) }  // Snow
	};


	void TerrainRenderer::GenerateMesh(const std::vector<float>& heightMap, uint32_t width, uint32_t height)
	{
		std::vector<uint32_t> colourMap(width * height);

		TerrainMeshBuilderData meshBuilderData = TerrainMeshBuilderData(width, height);
		float ScaleY = TerrainScale;

		// just to make the pivot at the center
		float topLeftX = (width - 1) / -2.0f;
		// just to make the pivot at the center
		float topLeftZ = (height - 1) / 2.0f;

		int meshSimplificationIncrement =(LevelOfDetail ==0) ? 1 :  LevelOfDetail * 2;
		int verticesPerLine = (width - 1) / meshSimplificationIncrement + 1; //https://www.youtube.com/watch?v=417kJGPKwDg&list=PLFt_AvWsXl0eBW2EiBtl_sxmDtSgZBxB3&index=6

		
		uint32_t vertexIndex = 0;
		for (uint32_t y = 0; y < height; y+= meshSimplificationIncrement)
		{
			for (uint32_t x = 0; x < width; x+= meshSimplificationIncrement)
			{
				Vertex v;
				v.Position = glm::vec3(topLeftX + x, Curve.Evaluate(heightMap[y * width + x]) * ScaleY, topLeftZ - y);

				meshBuilderData.Vertices[vertexIndex] = v;
				meshBuilderData.Vertices[vertexIndex].TexCoord = glm::vec2(
					x / float(width - 1),
					1.0f - (y / float(height - 1)) // Flip Y
				);
				if (x < width - 1 && y < height - 1)
				{
					meshBuilderData.AddTriangle(vertexIndex, vertexIndex + verticesPerLine + 1, vertexIndex + verticesPerLine);
					meshBuilderData.AddTriangle(vertexIndex + verticesPerLine + 1, vertexIndex, vertexIndex + 1);
				}

				vertexIndex++;
			}
		}


		if (m_TerrainMesh == nullptr)
		{
			m_TerrainMesh = meshBuilderData.GenerateMesh();
			AssetManager::CreateRuntimeAsset(m_TerrainMesh, "Terrain Mesh");
		}
		else
		{
			meshBuilderData.RecalculateNormals(meshBuilderData.Vertices, meshBuilderData.Indices);
			m_TerrainMesh->Reset("Terrain Mesh", meshBuilderData.Vertices, meshBuilderData.Indices);
		}

		std::vector<uint32_t> noiseMapData(width * height);

		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++) {
				float value = heightMap[y * width + x]; // assumes row-major layout
				glm::vec3 color = Math::Lerp(Colors::Black, Colors::White, value); // grayscale
				uint32_t packed = ConvertToBytes(color);
				noiseMapData[x + y * width] = packed;

				for (int i = 0; i < regions.size(); i++) {
					if (value <= regions[i].Height) {
						colourMap[y * width + x] = ConvertToBytes(regions[i].colour);
						break;
					}
				}
			}
		}
		
		Buffer buffer(noiseMapData.data(), noiseMapData.size() * sizeof(uint32_t), true);
		TextureConfiguration config;
		config.DebugName = "Noise Texture";
		config.Width = width;
		config.Height = height;
		config.Format = ImageFormat::RGBA;
		config.GenerateMips = false;
		m_NoiseTexture = Texture2D::Create(config, buffer);

		buffer.Release();


		Buffer colorBuffer(colourMap.data(), colourMap.size() * sizeof(uint32_t), true);
		config.DebugName = "Color Texture";
		config.Width = width;
		config.Height = height;
		config.Format = ImageFormat::RGBA;
		config.GenerateMips = true;
		m_ColorTexture = Texture2D::Create(config, colorBuffer, SamplerFactory::GetPoint());
		m_TerrainMesh->GetMaterialTable()->GetMaterial(0)->SetAlbedoMap(m_ColorTexture);
		m_TerrainMesh->GetMaterialTable()->GetMaterial(0)->SetAlbedo(glm::vec3(1));
		m_TerrainMesh->GetMaterialTable()->GetMaterial(0)->SetEmission(0);

		colorBuffer.Release();

	}
}