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
	
	float GetHeight(float x, float z, FastNoiseLite& base, FastNoiseLite& warpXNoise, FastNoiseLite& warpZNoise)
	{
		// Generate warp offsets (same as warp_x and warp_z in the image)
		float warpZ = warpZNoise.GetNoise(x / 7.4f, z / 5.4f);
		float warpX = warpXNoise.GetNoise(x / 10.f, z / 10.f);

		// Apply domain warping manually
		float warpedX = x / 10.f + warpX * 160.f;
		float warpedZ = z / 10.f + warpZ * 160.f;

		// Final noise height
		return base.GetNoise(warpedX, warpedZ);
	}

	std::vector<std::vector<float>> GenerateNoiseMap(
		int mapWidth, int mapHeight, int seed, float scale, int octaves,
		float persistence, float lacunarity, float offsetX, float offsetY)
	{
		std::vector<std::vector<float>> noiseMap(mapWidth, std::vector<float>(mapHeight));

		FastNoiseLite noise;
		noise.SetSeed(seed);
		noise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
		noise.SetFrequency(1.0f / scale);
		noise.SetFractalOctaves(octaves);
		noise.SetFractalLacunarity(lacunarity);
		noise.SetFractalGain(persistence);
		noise.SetFractalType(FastNoiseLite::FractalType_FBm);

		float minVal = std::numeric_limits<float>::max();
		float maxVal = std::numeric_limits<float>::lowest();

		for (int y = 0; y < mapHeight; ++y) {
			for (int x = 0; x < mapWidth; ++x) {
				float sampleX = x + offsetX;
				float sampleY = y + offsetY;

				float val = noise.GetNoise(sampleX, sampleY);
				noiseMap[x][y] = val;

				minVal = std::min(minVal, val);
				maxVal = std::max(maxVal, val);
			}
		}

		// Normalize to [0, 1]
		for (int y = 0; y < mapHeight; ++y) {
			for (int x = 0; x < mapWidth; ++x) {
				noiseMap[x][y] = (noiseMap[x][y] - minVal) / (maxVal - minVal);
			}
		}

		return noiseMap;
	}
	TerrainRenderer::TerrainRenderer()
	{

		const int size = 1024;
#if 1
		// Create and configure FastNoise object
		FastNoiseLite noise;
		noise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);

		std::vector<float> heightmap(size * size);

		for (int y = 0; y < size; y++) {
			for (int x = 0; x < size; x++) {
				float height = noise.GetNoise((float)x, (float)y);
				height = (height + 1.0f) * 0.5f;               // Remap to [0, 1]
				heightmap[y * size + x] = height;
				//heightmap[y * size + x] = glm::pow(height,4) * 200;
			}
		}
#else
		/*
		float scale = 20;

		FastNoiseLite noise;
		noise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
		noise.SetSeed(1341);
		noise.SetFrequency(0.01f);

		// Fractal settings
		noise.SetFractalType(FastNoiseLite::FractalType_FBm);
		noise.SetFractalOctaves(3);
		noise.SetFractalLacunarity(2.17f);
		noise.SetFractalGain(0.62f);

		// Domain warp settings
		noise.SetDomainWarpType(FastNoiseLite::DomainWarpType_BasicGrid);
		noise.SetDomainWarpAmp(2.5f);

		std::vector<float> heightmap(size * size);

		for (int y = 0; y < size; y++) {
			for (int x = 0; x < size; x++) {
				float height = noise.GetNoise((float)x, (float)y);
				heightmap[y * size + x] = glm::pow(height, 4) * 200;
			}
		}
		*/


		FastNoiseLite base;
		base.SetSeed(1341);
		base.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
		base.SetFrequency(0.01f);
		base.SetFractalType(FastNoiseLite::FractalType_FBm);
		base.SetFractalOctaves(3);
		base.SetFractalLacunarity(2.17f);
		base.SetFractalGain(0.62f);

		FastNoiseLite warpX;
		warpX.SetSeed(1341 + 100); // different seed for X
		warpX.SetNoiseType(FastNoiseLite::NoiseType_Perlin);

		FastNoiseLite warpZ;
		warpZ.SetSeed(1341 + 200); // different seed for Z
		warpZ.SetNoiseType(FastNoiseLite::NoiseType_Perlin);

		std::vector<float> heightmap(size * size);
		for (int y = 0; y < size; ++y) {
			for (int x = 0; x < size; ++x) {
				float fx = static_cast<float>(x);
				float fy = static_cast<float>(y);
				float h = GetHeight(fx, fy, base, warpX, warpZ);

				h = (h + 1.0f) * 0.5f;               // Remap to [0, 1]

				heightmap[y * size + x] = h; // You can scale this later in your mesh
			}
		}


#endif

		GenerateMesh(heightmap, size);
	}

	glm::vec3 CalculateNormal(uint32_t x, uint32_t y, const std::vector<float>& heightmap, uint32_t size,float scaleY)
	{
		auto getHeight = [&](uint32_t ix, uint32_t iy) {
			ix = std::clamp(ix, 0u, size - 1);
			iy = std::clamp(iy, 0u, size - 1);
			return heightmap[iy * size + ix];
			};

		float heightL = getHeight(x - 1, y);
		float heightR = getHeight(x + 1, y);
		float heightD = getHeight(x, y - 1);
		float heightU = getHeight(x, y + 1);

		glm::vec3 dx = glm::vec3(2.0f, (heightR - heightL) * scaleY, 0.0f);
		glm::vec3 dz = glm::vec3(0.0f, (heightU - heightD) * scaleY, 2.0f);

		glm::vec3 normal = glm::normalize(glm::cross(dz, dx));
		return normal;
	}

	void TerrainRenderer::GenerateMesh(const std::vector<float>& heightMap, uint32_t size)
	{

		float ScaleY = 30.0f;
		std::vector<Vertex> vertices;
		std::vector<Index> indices;

		for (uint32_t y = 0; y < size; y++)
		{
			for (uint32_t x = 0; x < size; x++)
			{
				float h = heightMap[y * size + x];
				vertices.push_back({
					glm::vec3(x, 30 * h, y),
					CalculateNormal(x,y,heightMap,size,30), // you can calculate normals later
					glm::vec2(x / (float)size, y / (float)size)
					});
			}
		}


		for (int y = 0; y < size - 1; y++) {
			for (int x = 0; x < size - 1; x++) {
				uint32_t i = y * size + x;
				uint32_t iRight = i + 1;
				uint32_t iBelow = i + size;
				uint32_t iBelowRight = i + size + 1;

				// Triangle 1
				indices.push_back(Index{ i, iBelow, iRight });

				// Triangle 2
				indices.push_back(Index{ iRight, iBelow, iBelowRight });
			}
		}

		if (m_TerrainMesh == nullptr)
		{
			m_TerrainMesh = Count<Mesh>::Create("Terrain Mesh", vertices, indices);
			AssetManager::CreateRuntimeAsset(m_TerrainMesh, "Terrain Mesh");

		}
		else
		{
			m_TerrainMesh->Reset("Terrain Mesh", vertices, indices);
		}

		std::vector<uint32_t> colourMap(size * size);

		for (int y = 0; y < size; ++y) {
			for (int x = 0; x < size; ++x) {
				float value = heightMap[y * size + x]; // assumes row-major layout
				glm::vec3 color = Math::Lerp(Colors::Black, Colors::White, value); // grayscale
				uint32_t packed = ConvertToBytes(color);
				colourMap[x + y * size] = packed; // fixed index
			}
		}

		Buffer buffer(colourMap.data(), colourMap.size() * sizeof(uint32_t), true);
		TextureConfiguration config;
		config.DebugName = "Noise Texture";
		config.Width = size;
		config.Height = size;
		config.Format = ImageFormat::RGBA;
		config.GenerateMips = false;
		m_NoiseTexture = Texture2D::Create(config, buffer);
		m_TerrainMesh->GetMaterialTable()->GetMaterial(0)->SetAlbedo(glm::vec3(0.0, 1.0, 0));

		buffer.Release();
	}
}
